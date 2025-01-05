#ifndef BACKUPARGSSTRUCTUI_H
#define BACKUPARGSSTRUCTUI_H

#include <QObject>
#include <QVariant>

#include "filebase.h"
#include "filter.h"
#include "argparser.h"
#include "logsmodel.h"

// 获取图形界面备份参数的工具类
class BackupArgsStructUI : public QObject
{
    Q_OBJECT
public:
    explicit BackupArgsStructUI(QObject *parent = nullptr);

    int get_args(QObject *rootObject);

    BackupInfo get_backup_info();
    FilterInfo get_filter_info();

private:
    // 备份参数
    BackupInfo backup_info;

    // 过滤参数
    FilterInfo filter_info;

    // 时间字符串
    TimeString time_string;

    int get_backup(QObject *rootObject);
    int get_filter(QObject *rootObject);
};

#endif // BACKUPARGSSTRUCTUI_H
