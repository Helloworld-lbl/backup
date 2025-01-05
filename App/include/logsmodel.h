#ifndef LOGSMODEL_H
#define LOGSMODEL_H

#include <QObject>
#include <QStringListModel>

// 日志工具类，封装了QStringListModel，用于显示日志
class LogsModel : public QObject
{
    Q_OBJECT
public:
    // 封装QStringListModel
    QStringListModel logs;

    // 构造函数
    explicit LogsModel(QObject *parent = nullptr);
    
    // 添加日志
    int add_log(QString log);
};

#endif // LOGSMODEL_H
