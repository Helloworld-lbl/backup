#include "logsmodel.h"
#include <qdebug.h>

LogsModel::LogsModel(QObject *parent)
    : QObject{parent}
{}

int LogsModel::add_log(QString log) {
    logs.insertRows(logs.rowCount(), 1);
    logs.setData(logs.index(logs.rowCount() - 1), log);
    return 0;
}
