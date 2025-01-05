#ifndef FILTER_H
#define FILTER_H

#include "filebase.h"
#include <string>
#include <regex>

// 过滤信息结构体
struct FilterInfo
{
    bool from_fileSize_checkBox;
    int from_fileSize;
    bool to_fileSize_checkBox;
    int to_fileSize;
    bool fileType_checkBox;
    int file_type;
    bool namePatten_checkBox;
    std::string namePatten_textField;
    bool pathPatten_checkBox;
    std::string pathPatten_textField;
    bool from_accessTime_checkBox;
    time_t from_accessTime;
    bool to_accessTime_checkBox;
    time_t to_accessTime;
    bool from_createTime_checkBox;
    time_t from_createTime;
    bool to_createTime_checkBox;
    time_t to_createTime;
    bool from_modifyTime_checkBox;
    time_t from_modifyTime;
    bool to_modifyTime_checkBox;
    time_t to_modifyTime;
};

// 文件过滤工具类，用于判定文件是否满足给定的过滤规则
class Filter
{
private:
    // 过滤信息
    FilterInfo filter_info;

public:
    Filter(FilterInfo filter_info_);
    
    // 判定文件是否满足给定的过滤规则
    bool check(const FileHeader &file_header_);
};

#endif // FILTER_H
