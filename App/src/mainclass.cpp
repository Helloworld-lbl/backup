#include "mainclass.h"
#include <QDebug>

MainClass::MainClass(QObject *parent)
    : QObject{parent}, engine((QQmlApplicationEngine *)parent)
{
}

int MainClass::set_engine(QQmlApplicationEngine *engine)
{
    this->engine = engine;
    return 0;
}
int MainClass::set_info_list(vector<BackupInfo> info_list)
{
    this->info_list = info_list;
    return 0;
}
int MainClass::set_table(TopTableModel *table)
{
    this->table = table;
    return 0;
}
int MainClass::set_backup_logs(LogsModel *logs)
{
    this->backup_logs = logs;
    return 0;
}
int MainClass::set_restore_logs(LogsModel *logs)
{
    this->restore_logs = logs;
    return 0;
}

int MainClass::do_backup()
{
    try
    {
        backup_logs->add_log("开始备份...");
        QObject *rootObject = engine->rootObjects().first();
        BackupArgsStructUI *args = new BackupArgsStructUI(backup_logs);
        if (args->get_args(rootObject))
        {
            return -1;
        }

        BackupInfo backup_info = args->get_backup_info();
        // 若目标路径已有文件
        if (std::filesystem::exists(backup_info.bak_path))
        {
            throw std::runtime_error("错误：备份路径已存在文件！");
        }

        Filter *filter = new Filter(args->get_filter_info());

        Task task(backup_info, backup_logs, filter);

        if (!task.backup())
            return -1;
        else
            backup_logs->add_log("备份完成！");
    }
    catch (std::exception &e)
    {
        backup_logs->add_log(e.what());
    }
    return 0;
}

int MainClass::do_restore()
{
    try
    {
        restore_logs->add_log("开始还原...");
        QObject *rootObject = engine->rootObjects().first();
        RestoreArgsStructUI *args = new RestoreArgsStructUI(restore_logs);
        if (args->get_args(rootObject))
        {
            return -1;
        }

        BackupInfo info = info_list[args->id];
        FileBase file(info.bak_path);
        file.open_file(std::ios::in | std::ios::out | std::ios::binary);
        file.read_out_backup_info(info);
        file.close();

        info.src_path = args->path;
        info.key = args->key;

        Task task(info, restore_logs);

        if (!task.restore())
            return -1;
        else
            restore_logs->add_log("还原完成！");
    }
    catch (std::exception &e)
    {
        restore_logs->add_log(e.what());
    }
    return 0;
}

int MainClass::show_table()
{
    try
    {
        info_list.clear();
        Database::read_from_database(info_list);
        table->showVector(info_list);
    }
    catch (std::exception &e)
    {
        restore_logs->add_log(e.what());
    }
    return 0;
}

int MainClass::set_db_path()
{
    try
    {
        QObject *rootObject = engine->rootObjects().first();
        QObject *config_dbField_textField = rootObject->findChild<QObject *>("config_dbField_textField", Qt::FindChildrenRecursively);
        std::string path = config_dbField_textField->property("text").toString().toStdString();
        Database::set_db_path(path);
    }
    catch (std::exception &e)
    {
        restore_logs->add_log(e.what());
    }
    return 0;
}

int MainClass::add_to_db()
{
    try
    {
        QObject *rootObject = engine->rootObjects().first();
        QObject *add_row_textField = rootObject->findChild<QObject *>("add_row_textField", Qt::FindChildrenRecursively);
        std::string path = std::filesystem::absolute(add_row_textField->property("text").toString().toStdString());
        Database::add_to_database(path);
    }
    catch (std::exception &e)
    {
        restore_logs->add_log(e.what());
    }
    return 0;
}

int MainClass::delete_from_db()
{
    try
    {
        QObject *rootObject = engine->rootObjects().first();
        QObject *delete_row_textField = rootObject->findChild<QObject *>("delete_row_textField", Qt::FindChildrenRecursively);
        int id = delete_row_textField->property("text").toInt();
        Database::delete_by_id(id);
    }
    catch (std::exception &e)
    {
        restore_logs->add_log(e.what());
    }
    return 0;
}
