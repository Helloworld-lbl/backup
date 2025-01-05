#ifndef RESTOREARGSSTRUCTUI_H
#define RESTOREARGSSTRUCTUI_H

#include <QObject>
#include <QVariant>

#include "filebase.h"
#include "logsmodel.h"

// 获取图形界面还原参数的工具类
class RestoreArgsStructUI : public QObject
{
    Q_OBJECT
public:
    explicit RestoreArgsStructUI(QObject *parent = nullptr);

    int id;
    std::string path;
    std::string key;

    int get_args(QObject *rootObject);

signals:
};

#endif // RESTOREARGSSTRUCTUI_H
