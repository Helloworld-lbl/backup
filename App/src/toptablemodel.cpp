#include "toptablemodel.h"
#include <QObject>
#include <QAbstractTableModel>
#include <QString>

TopTableModel::TopTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    table.append({"编号", "备份时间", "原路径", "备份路径", "压缩算法", "加密算法"});
}

int TopTableModel::showVector(vector<BackupInfo> v) {
    beginResetModel();
    table.clear();
    table.append({"编号", "备份时间", "原路径", "备份路径", "压缩算法", "加密算法"});
    int row = 0;
    for (auto i : v) {
        char buf_time[100];
        strftime(buf_time, sizeof(buf_time), "%Y-%m-%d %H:%M:%S", localtime(&i.timestamp));
        QString compression_algorithm = QString();
        QString encryption_algorithm = QString();
        if (i.compression_algorithm & CPS_Huffman)
            compression_algorithm = "Huffman";
        if (i.compression_algorithm & CPS_LZ77)
            compression_algorithm = "LZ77";
        if (i.encryption_algorithm & EPT_AES)
            encryption_algorithm = "AES";
        if (i.encryption_algorithm & EPT_DES)
            encryption_algorithm = "DES";

        table.append({QString::number(row),
                      QString(buf_time),
                      QString::fromStdString(i.src_path),
                      QString::fromStdString(i.bak_path),
                      compression_algorithm,
                      encryption_algorithm});

        row++;
    }
    endResetModel();
    return 0;
}

QVariant TopTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    qDebug()<<role<<"accessed";
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    return QVariant();
}
QHash<int, QByteArray> TopTableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TableDataRole] = "tabledata";
    roles[HeadingRole] = "heading";
    roles[FirstNameRole] = "FirstName";
    roles[LastNameRole] = "LastName";
    return roles;
}
int TopTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return table.size();
    // FIXME: Implement me!
}

int TopTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return table.at(0).size();
    // FIXME: Implement me!
}
QVariant TopTableModel::get_display_data(const QModelIndex &index){
    return data(index,TableDataRole );
}
bool TopTableModel::hasChildren(const QModelIndex &parent) const
{
    // FIXME: Implement me!
    return false;
}

bool TopTableModel::canFetchMore(const QModelIndex &parent) const
{
    // FIXME: Implement me!
    return false;
}

void TopTableModel::fetchMore(const QModelIndex &parent)
{
    // FIXME: Implement me!
}

QVariant TopTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role) {
    case TableDataRole:
    {
        return  table.at(index.row()).at(index.column());
    }
    case HeadingRole:{
        if(index.row()==0){
            return true;
        }
        else{
            return false;
        }
    }
    case FirstNameRole:{
        if(index.column()==0){
            return true;
        }
        return false;
    }
    case LastNameRole:{
        if(index.column()==1){
            return true;
        }
        return false;
    }
    default:
        break;
    }
    // FIXME: Implement me!
    return QVariant();
}

bool TopTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool TopTableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

bool TopTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

bool TopTableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return true;
}
