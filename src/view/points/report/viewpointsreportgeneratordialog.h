#ifndef VIEWPOINTSREPORTGENERATORDIALOG_H
#define VIEWPOINTSREPORTGENERATORDIALOG_H

#include <QDialog>

class ViewPointsReportGenerator;

class QPushButton;
class QLabel;
class QProgressBar;
class QLineEdit;
class QCheckBox;

class ViewPointsReportGeneratorDialog : public QDialog
{
    Q_OBJECT

public slots:
    void setPathSlot ();
    void pathEditedSlot (const QString& text);
    void filenameEditedSlot(const QString& text);

    void authorEditedSlot (const QString& text);
    void abstractEditedSlot(const QString& text);

    void allUnsortedChangedSlot (bool checked);
    void groupByTypeChangedSlot (bool checked);
    void addOverviewTableChangedSlot (bool checked);

    void waitOnMapLoadingEditedSlot(bool checked);
    void addOverviewScreenshotChangedSlot (bool checked);

    void runPDFLatexChangedSlot (bool checked);
    void openPDFChangedSlot (bool checked);

    void runSlot();
    void cancelSlot();

public:
    ViewPointsReportGeneratorDialog(ViewPointsReportGenerator& generator,
                                    QWidget* parent = nullptr, Qt::WindowFlags f = 0);

    void updateFileInfo ();

    void setRunning (bool value);

    void setElapsedTime (const std::string& time_str);
    void setProgress (unsigned int min, unsigned int max, unsigned int value);
    void setStatus (const std::string& status);
    void setRemainingTime (const std::string& time_str);

protected:
    ViewPointsReportGenerator& generator_;

    QWidget* config_container_ {nullptr};

    QLineEdit* directory_edit_ {nullptr};
    QLineEdit* filename_edit_ {nullptr};

    QLineEdit* author_edit_ {nullptr};
    QLineEdit* abstract_edit_ {nullptr};

    QCheckBox* all_unsorted_check_ {nullptr};
    QCheckBox* group_types_check_ {nullptr};
    QCheckBox* add_overview_table_check_ {nullptr};

    QCheckBox* wait_on_map_loading_check_ {nullptr};
    QCheckBox* add_overview_screenshot_check_ {nullptr};

    QCheckBox* pdflatex_check_ {nullptr};
    QCheckBox* open_pdf_check_ {nullptr};

    QPushButton* run_button_{nullptr};

    QLabel* elapsed_time_label_{nullptr};
    QProgressBar* progress_bar_{nullptr};
    QLabel* status_label_{nullptr};
    QLabel* remaining_time_label_{nullptr};

    QPushButton* quit_button_{nullptr};
};

#endif // VIEWPOINTSREPORTGENERATORDIALOG_H
