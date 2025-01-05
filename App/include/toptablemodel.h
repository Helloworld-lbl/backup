#ifndef TOPTABLEMODEL_H
#define TOPTABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QtQml>

#include <task.h>
#include <vector>

// 图形界面表格模型工具类
class TopTableModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
    enum TableRoles{
        TableDataRole = Qt::UserRole + 1,
        HeadingRole,
        FirstNameRole,
        LastNameRole,
    };
public:
    explicit TopTableModel(QObject *parent = nullptr);

    int showVector(vector<BackupInfo> v);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    Q_INVOKABLE QVariant get_display_data(const QModelIndex& index);

private:
    QVector<QVector<QString>> table;
};

#endif // TOPTABLEMODEL_H

