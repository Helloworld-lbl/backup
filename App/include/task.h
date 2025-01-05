#ifndef TASK_H
#define TASK_H

#include "logsmodel.h"
#include "packer.h"
#include "compressor.h"
#include "aes/aes.h"
#include "des/des.h"
#include "huffman/huffman.h"
#include "lz77/lz77.h"
#include "database.h"

// 核心任务类
class Task
{
private:
    std::filesystem::path src_path; // 待备份的目录
    std::filesystem::path bak_path; // 备份文件路径(存放位置)

    Filter* filter; // 用户自定义规则 过滤文件

    // bool restore_metadata; // 是否恢复文件元信息

    // 日志
    LogsModel* logs;

public:
    // 备份信息
    BackupInfo info;

    // 构造函数
    Task(BackupInfo info, LogsModel* logs, Filter* filter = nullptr);

    // 备份
    bool backup();

    // 还原
    bool restore();

    // bool GetBackupInfo();
    // unsigned char GetBackupMode();

    // // 获取备份文件的基本信息
    // static bool GetBackupInfo(std::string file_path_, BackupInfo &info_);
};

#endif // TASK_H
