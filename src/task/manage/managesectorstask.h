#ifndef MANAGESECTORSTASK_H
#define MANAGESECTORSTASK_H

#include <QObject>
#include <memory>

#include "configurable.h"
#include "task.h"

class TaskManager;
class ManageSectorsTaskWidget;
class SavedFile;
class Sector;
class OGRPolygon;
class OGRLinearRing;

class ManageSectorsTask : public Task, public Configurable
{
public:
    ManageSectorsTask(const std::string& class_id, const std::string& instance_id,
                      TaskManager& task_manager);
    virtual ~ManageSectorsTask();

    virtual void generateSubConfigurable(const std::string& class_id,
                                         const std::string& instance_id);

    virtual TaskWidget* widget();
    virtual void deleteWidget();

    bool canImportFile();
    void importFile ();

    virtual bool checkPrerequisites();
    virtual bool isRecommended() { return false; }
    virtual bool isRequired() { return false; }

    const std::map<std::string, SavedFile*>& fileList() { return file_list_; }
    bool hasFile(const std::string& filename) { return file_list_.count(filename) > 0; }
    void addFile(const std::string& filename);
    void removeCurrentFilename();
    void removeAllFiles ();
    void currentFilename(const std::string& filename);
    const std::string& currentFilename() { return current_filename_; }

    std::string parseMessage() const;
    //std::vector<std::shared_ptr<Sector>>& parsedData() const;

protected:
    std::map<std::string, SavedFile*> file_list_;
    std::string current_filename_;

    std::unique_ptr<ManageSectorsTaskWidget> widget_;

    unsigned int found_sectors_num_{0};
    std::string parse_message_;

    virtual void checkSubConfigurables() {}

    void parseCurrentFile (bool import);

    void addPolygon (const std::string& layer_name, OGRPolygon& polygon, bool import);
    void addLinearRing (const std::string& layer_name, const std::string& polygon_name, OGRLinearRing& ring,
                        bool import);

    void addSector (const std::string& sector_name, const std::string& layer_name,
                    std::vector<std::pair<double,double>> points);
};

#endif // MANAGESECTORSTASK_H
