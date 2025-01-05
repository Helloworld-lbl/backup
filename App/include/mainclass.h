#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QStringListModel>

#include "task.h"
#include "backupargsstructui.h"
#include "restoreargsstructui.h"
#include "toptablemodel.h"
#include "logsmodel.h"

// 主类，用于处理图形界面事件
class MainClass : public QObject
{
    Q_OBJECT
public:
    explicit MainClass(QObject *parent = nullptr);

private:

    // 图形界面引擎
    QQmlApplicationEngine* engine;

    // 备份信息列表
    vector<BackupInfo> info_list;

    // 图形界面表格模型
    TopTableModel* table;

    // 备份日志
    LogsModel* backup_logs;

    // 还原日志
    LogsModel* restore_logs;

public slots:
    // 按下“备份”按钮
    int do_backup();

    // 按下“还原”按钮
    int do_restore();

    // 更新表格
    int show_table();

    // 按下设定数据库路径按钮
    int set_db_path();

    // 按下“添加”按钮
    int add_to_db();

    // 按下“删除”按钮
    int delete_from_db();

public:
    // 设置各个属性
    int set_engine(QQmlApplicationEngine* engine);
    int set_info_list(vector<BackupInfo> info_list);
    int set_table(TopTableModel* table);
    int set_backup_logs(LogsModel* logs);
    int set_restore_logs(LogsModel* logs);
};

#endif // MAINCLASS_H
