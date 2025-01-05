#include "database.h"

std::filesystem::path Database::db_path = std::filesystem::absolute("backup.csv");

int Database::write_to_database(const BackupInfo &info)
{
    // 打开CSV文件
    FILE *file = fopen(db_path.c_str(), "a");
    if (file == NULL)
    {
        throw std::runtime_error("错误：无法打开数据库文件！");
    }

    // 写入数据
    fprintf(file, "%ld,%s,%s,%s,%u,%u\n",
            (long)info.timestamp,
            info.checksum.c_str(),
            info.src_path.c_str(),
            info.bak_path.c_str(),
            info.encryption_algorithm,
            info.compression_algorithm);

    // 关闭文件
    fclose(file);

    return 0;
}

int Database::read_from_database(std::vector<BackupInfo> &info_list)
{
    // 打开CSV文件
    FILE *file = fopen(db_path.c_str(), "r");
    if (file == NULL)
    {
        throw std::runtime_error("错误：无法打开数据库文件！");
    }

    char line[MAX_LINE_LENGTH];

    // 逐行读取数据
    while (fgets(line, sizeof(line), file) != NULL)
    {
        BackupInfo info;
        char *token = strtok(line, ",");
        if (token == NULL)
            continue;

        // 解析时间戳
        info.timestamp = atol(token);
        token = strtok(NULL, ",");
        if (token == NULL)
            continue;

        // 解析校验和
        info.checksum = token;
        token = strtok(NULL, ",");
        if (token == NULL)
            continue;

        // 解析备份路径
        info.src_path = token;
        token = strtok(NULL, ",");
        if (token == NULL)
            continue;

        // 解析目标路径
        info.bak_path = token;
        token = strtok(NULL, ",");
        if (token == NULL)
            continue;

        // 解析加密算法
        info.encryption_algorithm = strtoul(token, NULL, 10);
        token = strtok(NULL, ",");
        if (token == NULL)
            continue;

        // 解析压缩算法
        info.compression_algorithm = strtoul(token, NULL, 10);

        // 将解析出的数据存储到向量中
        info_list.push_back(info);
    }

    // 关闭文件
    fclose(file);

    return 0;
}

int Database::delete_by_id(int id)
{
    std::ifstream inputFile(db_path);
    if (!inputFile.is_open())
    {
        throw std::runtime_error("错误：无法打开数据库文件！");
    }

    std::ofstream outputFile(std::filesystem::absolute("temp.csv"));
    if (!outputFile.is_open())
    {
        throw std::runtime_error("错误：无法打开临时文件！");
    }

    int i = 0;
    std::string line;
    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        std::string cell;
        std::vector<std::string> row;
        while (std::getline(iss, cell, ','))
        {
            row.push_back(cell);
        }

        if (row.size() > 0 && i != id)
        {
            outputFile << line << "\n";
        }
        i++;
    }

    inputFile.close();
    outputFile.close();

    // 替换原文件
    remove(db_path.c_str());
    rename(std::filesystem::absolute("temp.csv"), db_path.c_str());

    return 0;
}

int Database::add_to_database(const std::string &file_path)
{
    std::filesystem::path path = std::filesystem::absolute(file_path);
    FileBase file(path);
    file.open_file(std::ios::in | std::ios::out | std::ios::binary);
    BackupInfo info;
    file.read_backup_info(info);
    file.close();
    info.bak_path = file_path; // 更新备份路径
    write_to_database(info);
    return 0;
}

QString Database::get_db_path()
{
    return QString::fromStdString(db_path);
}

int Database::set_db_path(const std::string &path)
{
    db_path = std::filesystem::absolute(path);
    return 0;
}
