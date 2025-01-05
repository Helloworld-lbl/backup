#include "backupargsstructui.h"
#include <QDebug>

BackupArgsStructUI::BackupArgsStructUI(QObject *parent)
    : QObject{parent}
{
}

BackupInfo BackupArgsStructUI::get_backup_info()
{
    return backup_info;
}
FilterInfo BackupArgsStructUI::get_filter_info()
{
    return filter_info;
}

int BackupArgsStructUI::get_args(QObject *rootObject)
{
    get_backup(rootObject);
    get_filter(rootObject);
    ArgParser *parser = new ArgParser(time_string, backup_info, filter_info);
    parser->parse();
    return 0;
}

int BackupArgsStructUI::get_backup(QObject *rootObject)
{
    QObject *path_textField = rootObject->findChild<QObject *>("path_textField", Qt::FindChildrenRecursively);
    backup_info.src_path = std::filesystem::absolute(path_textField->property("text").toString().toStdString());

    QObject *bak_path_textField = rootObject->findChild<QObject *>("bak_path_textField", Qt::FindChildrenRecursively);
    backup_info.bak_path = std::filesystem::absolute(bak_path_textField->property("text").toString().toStdString());

    QObject *compression_checkBox = rootObject->findChild<QObject *>("compression_checkBox", Qt::FindChildrenRecursively);
    bool compressionChecked = compression_checkBox->property("checked").toBool();
    if (!compressionChecked)
    {
        backup_info.compression_algorithm = CPS_NONE;
    }
    else
    {
        QObject *compression_comboBox = rootObject->findChild<QObject *>("compression_comboBox", Qt::FindChildrenRecursively);
        backup_info.compression_algorithm = compression_comboBox->property("currentIndex").toInt() + 1;
    }

    QObject *encryption_checkBox = rootObject->findChild<QObject *>("encryption_checkBox", Qt::FindChildrenRecursively);
    bool encryptionChecked = encryption_checkBox->property("checked").toBool();
    if (!encryptionChecked)
    {
        backup_info.encryption_algorithm = EPT_NONE;
    }
    else
    {
        QObject *encryption_comboBox = rootObject->findChild<QObject *>("encryption_comboBox", Qt::FindChildrenRecursively);
        backup_info.encryption_algorithm = encryption_comboBox->property("currentIndex").toInt() + 1;
    }

    QObject *key_backup_textField = rootObject->findChild<QObject *>("key_backup_textField", Qt::FindChildrenRecursively);
    backup_info.key = key_backup_textField->property("text").toString().toStdString();

    return 0;
}

int BackupArgsStructUI::get_filter(QObject *rootObject)
{
    QObject *from_fileSize_checkBox = rootObject->findChild<QObject *>("from_fileSize_checkBox", Qt::FindChildrenRecursively);
    filter_info.from_fileSize_checkBox = from_fileSize_checkBox->property("checked").toBool();

    if (filter_info.from_fileSize_checkBox)
    {
        QObject *from_fileSize_textField = rootObject->findChild<QObject *>("from_fileSize_textField", Qt::FindChildrenRecursively);
        std::string from_fileSize = from_fileSize_textField->property("text").toString().toStdString();
        try
        {
            filter_info.from_fileSize = std::stoi(from_fileSize);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("错误：未输入最小字节数！");
        }
    }

    QObject *to_fileSize_checkBox = rootObject->findChild<QObject *>("to_fileSize_checkBox", Qt::FindChildrenRecursively);
    filter_info.to_fileSize_checkBox = to_fileSize_checkBox->property("checked").toBool();

    if (filter_info.to_fileSize_checkBox)
    {
        QObject *to_fileSize_textField = rootObject->findChild<QObject *>("to_fileSize_textField", Qt::FindChildrenRecursively);
        std::string to_fileSize = to_fileSize_textField->property("text").toString().toStdString();
        try
        {
            filter_info.to_fileSize = std::stoi(to_fileSize);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("错误：未输入最大字节数！");
        }
    }

    QObject *fileType_checkBox = rootObject->findChild<QObject *>("fileType_checkBox", Qt::FindChildrenRecursively);
    filter_info.fileType_checkBox = fileType_checkBox->property("checked").toBool();

    filter_info.file_type = 0;

    QObject *normal_fileType_checkBox = rootObject->findChild<QObject *>("normal_fileType_checkBox", Qt::FindChildrenRecursively);
    if (normal_fileType_checkBox->property("checked").toBool())
    {
        filter_info.file_type |= FILE_TYPE_NORMAL;
    }

    QObject *hard_link_fileType_checkBox = rootObject->findChild<QObject *>("hard_link_fileType_checkBox", Qt::FindChildrenRecursively);
    if (hard_link_fileType_checkBox->property("checked").toBool())
    {
        filter_info.file_type |= FILE_TYPE_HARD_LINK;
    }

    QObject *symbolic_link_fileType_checkBox = rootObject->findChild<QObject *>("symbolic_link_fileType_checkBox", Qt::FindChildrenRecursively);
    if (symbolic_link_fileType_checkBox->property("checked").toBool())
    {
        filter_info.file_type |= FILE_TYPE_SYMBOLIC_LINK;
    }

    QObject *directory_fileType_checkBox = rootObject->findChild<QObject *>("directory_fileType_checkBox", Qt::FindChildrenRecursively);
    if (directory_fileType_checkBox->property("checked").toBool())
    {
        filter_info.file_type |= FILE_TYPE_DIRECTORY;
    }

    QObject *fifo_fileType_checkBox = rootObject->findChild<QObject *>("fifo_fileType_checkBox", Qt::FindChildrenRecursively);
    if (fifo_fileType_checkBox->property("checked").toBool())
    {
        filter_info.file_type |= FILE_TYPE_FIFO;
    }

    QObject *other_fileType_checkBox = rootObject->findChild<QObject *>("other_fileType_checkBox", Qt::FindChildrenRecursively);
    if (other_fileType_checkBox->property("checked").toBool())
    {
        filter_info.file_type |= FILE_TYPE_OTHER;
    }
    QObject *pathPatten_checkBox = rootObject->findChild<QObject *>("pathPatten_checkBox", Qt::FindChildrenRecursively);
    filter_info.pathPatten_checkBox = pathPatten_checkBox->property("checked").toBool();

    QObject *pathPatten_textField = rootObject->findChild<QObject *>("pathPatten_textField", Qt::FindChildrenRecursively);
    filter_info.pathPatten_textField = pathPatten_textField->property("text").toString().toStdString();

    QObject *namePatten_checkBox = rootObject->findChild<QObject *>("namePatten_checkBox", Qt::FindChildrenRecursively);
    filter_info.namePatten_checkBox = namePatten_checkBox->property("checked").toBool();

    QObject *namePatten_textField = rootObject->findChild<QObject *>("namePatten_textField", Qt::FindChildrenRecursively);
    filter_info.namePatten_textField = namePatten_textField->property("text").toString().toStdString();

    QObject *from_accessTime_checkBox = rootObject->findChild<QObject *>("from_accessTime_checkBox", Qt::FindChildrenRecursively);
    filter_info.from_accessTime_checkBox = from_accessTime_checkBox->property("checked").toBool();

    QObject *from_accessTime_textField = rootObject->findChild<QObject *>("from_accessTime_textField", Qt::FindChildrenRecursively);
    time_string.from_accessTime = from_accessTime_textField->property("text").toString().toStdString();

    QObject *to_accessTime_checkBox = rootObject->findChild<QObject *>("to_accessTime_checkBox", Qt::FindChildrenRecursively);
    filter_info.to_accessTime_checkBox = to_accessTime_checkBox->property("checked").toBool();

    QObject *to_accessTime_textField = rootObject->findChild<QObject *>("to_accessTime_textField", Qt::FindChildrenRecursively);
    time_string.to_accessTime = to_accessTime_textField->property("text").toString().toStdString();

    QObject *from_createTime_checkBox = rootObject->findChild<QObject *>("from_createTime_checkBox", Qt::FindChildrenRecursively);
    filter_info.from_createTime_checkBox = from_createTime_checkBox->property("checked").toBool();

    QObject *from_createTime_textField = rootObject->findChild<QObject *>("from_createTime_textField", Qt::FindChildrenRecursively);
    time_string.from_createTime = from_createTime_textField->property("text").toString().toStdString();

    QObject *to_createTime_checkBox = rootObject->findChild<QObject *>("to_createTime_checkBox", Qt::FindChildrenRecursively);
    filter_info.to_createTime_checkBox = to_createTime_checkBox->property("checked").toBool();

    QObject *to_createTime_textField = rootObject->findChild<QObject *>("to_createTime_textField", Qt::FindChildrenRecursively);
    time_string.to_createTime = to_createTime_textField->property("text").toString().toStdString();

    QObject *from_modifyTime_checkBox = rootObject->findChild<QObject *>("from_modifyTime_checkBox", Qt::FindChildrenRecursively);
    filter_info.from_modifyTime_checkBox = from_modifyTime_checkBox->property("checked").toBool();

    QObject *from_modifyTime_textField = rootObject->findChild<QObject *>("from_modifyTime_textField", Qt::FindChildrenRecursively);
    time_string.from_modifyTime = from_modifyTime_textField->property("text").toString().toStdString();

    QObject *to_modifyTime_checkBox = rootObject->findChild<QObject *>("to_modifyTime_checkBox", Qt::FindChildrenRecursively);
    filter_info.to_modifyTime_checkBox = to_modifyTime_checkBox->property("checked").toBool();

    QObject *to_modifyTime_textField = rootObject->findChild<QObject *>("to_modifyTime_textField", Qt::FindChildrenRecursively);
    time_string.to_modifyTime = to_modifyTime_textField->property("text").toString().toStdString();

    return 0;
}
