#ifndef INCLUDE_ARGPARSER_H_
#define INCLUDE_ARGPARSER_H_
#include "filebase.h"
#include "filter.h"

#include "logsmodel.h"

// 时间字符串结构体，用于暂存时间字符串以供解析为time_t
struct TimeString
{
    std::string from_accessTime;
    std::string to_accessTime;
    std::string from_createTime;
    std::string to_createTime;
    std::string from_modifyTime;
    std::string to_modifyTime;
};

// 参数解析工具类
class ArgParser
{
private:
    TimeString& time_string;
    BackupInfo& backup_info;
    FilterInfo& filter_info;

public:
    ArgParser(TimeString& time_string, BackupInfo& backup_info, FilterInfo& filter_info);

    // 解析参数
    int parse();

    // 检查正则表达式
    int check_reg_exp();

    // 检查备份路径
    int check_backup_paths();

private:
    // 将时间字符串转换为time_t
    int convert_time(std::string str_time, time_t &time);
    time_t str_to_time(std::string str_time);
};

#endif // INCLUDE_ARGPARSER_H_
