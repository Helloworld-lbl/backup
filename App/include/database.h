#ifndef DATABASE_H
#define DATABASE_H

#include "filebase.h"

#include <vector>
#include <QString>

#define MAX_LINE_LENGTH 1024

// 数据库工具类
class Database
{
private:
    // 数据库文件路径
    static std::filesystem::path db_path;
public:
    // 将一条备份信息写入数据库
    static int write_to_database(const BackupInfo& info);

    // 将数据库中所有备份信息读出
    static int read_from_database(std::vector<BackupInfo>& info_list);

    // 根据id删除一条备份信息
    static int delete_by_id(int id);

    // 根据备份文件路径添加一条备份信息
    static int add_to_database(const std::string& file_path);

    // 获取数据库文件路径
    static QString get_db_path();

    // 设置数据库文件路径
    static int set_db_path(const std::string& path);
};

#endif
