/*
 * This file is part of ATSDB.
 *
 * ATSDB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ATSDB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ATSDB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"

#include "atsdb.h"
#include "config.h"
#include "configurationmanager.h"
#include "dbobject.h"
#include "dbobjectmanager.h"
#include "files.h"
#include "filtermanager.h"
#include "global.h"
#include "jobmanager.h"
#include "logger.h"
#include "managementwidget.h"
#include "stringconv.h"
#include "taskmanager.h"
#include "taskmanagerwidget.h"
#include "viewmanager.h"
#include "viewpointswidget.h"

#include <QApplication>
#include <QCloseEvent>
#include <QSettings>
#include <QStackedWidget>
#include <QTabWidget>
#include <QLocale>
#include <QMessageBox>

using namespace Utils;
using namespace std;

MainWindow::MainWindow()
{
    logdbg << "MainWindow: constructor";

    QLocale::setDefault(QLocale::c());
    setLocale(QLocale::c());

    const char* appdir = getenv("APPDIR");
    if (appdir)
        QCoreApplication::setAttribute(Qt::AA_DontUseNativeDialogs); // disable native since they cause crashes

    setMinimumSize(QSize(1200, 900));

    QIcon atsdb_icon(Files::getIconFilepath("atsdb.png").c_str());
    setWindowIcon(atsdb_icon);  // for the glory of the empire

    QSettings settings("ATSDB", "Client");
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());

    assert(ATSDB::instance().config().existsId("version"));
    std::string title = "ATSDB v" + ATSDB::instance().config().getString("version");

    if (ATSDB::instance().config().existsId("save_config_on_exit"))
    {
        save_configuration_ = ATSDB::instance().config().getBool("save_config_on_exit");
        loginf << "MainWindow: constructor: save configuration on exit " << save_configuration_;
    }

    QWidget::setWindowTitle(title.c_str());

    tab_widget_ = new QTabWidget();

    TaskManager& task_man = ATSDB::instance().taskManager();

    task_manager_widget_ = task_man.widget();
    tab_widget_->addTab(task_manager_widget_, "Tasks");

    connect(&task_man, &TaskManager::startInspectionSignal, this, &MainWindow::startSlot);
    connect(&task_man, &TaskManager::quitRequestedSignal, this, &MainWindow::quitRequestedSlot, Qt::QueuedConnection);

    // management widget
    management_widget_ = new ManagementWidget();

    setCentralWidget(tab_widget_);

    tab_widget_->setCurrentIndex(0);

    QObject::connect(this, &MainWindow::startedSignal, &ATSDB::instance().filterManager(),
                     &FilterManager::startedSlot);

}

MainWindow::~MainWindow()
{
    logdbg << "MainWindow: destructor";

    // remember: this not called! insert deletes into closeEvent function
}

void MainWindow::disableConfigurationSaving()
{
    logdbg << "MainWindow: disableConfigurationSaving";
    save_configuration_ = false;
}

void MainWindow::databaseOpenedSlot() { logdbg << "MainWindow: databaseOpenedSlot"; }

void MainWindow::startSlot()
{
    loginf << "MainWindow: startSlot";

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QMessageBox* msg_box = new QMessageBox(this);
    msg_box->setWindowTitle("Starting");
    msg_box->setText("Please wait...");
    msg_box->setStandardButtons(0);
    msg_box->show();

    boost::posix_time::ptime start_time = boost::posix_time::microsec_clock::local_time();
    while ((boost::posix_time::microsec_clock::local_time()-start_time).total_milliseconds() < 50)
    {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        QThread::msleep(1);
    }

    emit startedSignal();

    assert(task_manager_widget_);
    tab_widget_->removeTab(0);

    // close any opened dbobject widgets
    for (auto& obj_it : ATSDB::instance().objectManager())
        obj_it.second->closeWidget();

    assert(management_widget_);
    tab_widget_->addTab(management_widget_, "Management");

    ATSDB::instance().viewManager().init(tab_widget_); // adds view points widget

    tab_widget_->setCurrentIndex(0);

    emit JobManager::instance().databaseIdle();  // to enable ViewManager add button, slightly HACKY

    msg_box->close();
    delete msg_box;

    QApplication::restoreOverrideCursor();
}

void MainWindow::quitRequestedSlot()
{
    shutdown();
    QApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    logdbg << "MainWindow: closeEvent: start";

    shutdown();
    event->accept();

    logdbg << "MainWindow: closeEvent: done";
}


void MainWindow::shutdown()
{
    QSettings settings("ATSDB", "Client");
    settings.setValue("MainWindow/geometry", saveGeometry());

    ATSDB::instance().viewManager().unsetCurrentViewPoint(); // needed to remove temporary stuff

    if (save_configuration_)
        ConfigurationManager::getInstance().saveConfiguration();
    else
        loginf << "MainWindow: closeEvent: configuration not saved";

    ATSDB::instance().shutdown();

    if (tab_widget_)
    {
        delete tab_widget_;
        tab_widget_ = nullptr;
    }
}

// void MainWindow::keyPressEvent ( QKeyEvent * event )
//{
//    logdbg << "MainWindow: keyPressEvent '" << event->text().toStdString() << "'";
//}
