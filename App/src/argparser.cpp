#include "argparser.h"

ArgParser::ArgParser(TimeString& time_string, BackupInfo& backup_info, FilterInfo& filter_info) :
    time_string{time_string}, backup_info{backup_info}, filter_info{filter_info}
{}

int ArgParser::check_reg_exp()
{
    try
    {
        if (filter_info.pathPatten_checkBox)
            std::regex reg(filter_info.pathPatten_textField);
        if (filter_info.namePatten_checkBox)
            std::regex reg(filter_info.namePatten_textField);
    }
    catch (const std::exception &e)
    {
        std::string error_log = "错误：非法的正则表达式：";
        error_log.append(e.what());
        throw std::runtime_error(error_log);
        return -1;
    }
    return 0;
}

int ArgParser::check_backup_paths()
{
    if (backup_info.src_path.empty())
    {
        throw std::runtime_error("错误：未输入原路径！");
        return -1;
    }
    if (backup_info.bak_path.empty())
    {
        throw std::runtime_error("错误：未输入备份路径！");
        return -1;
    }
    return 0;
}

// bool ArgParser::convert_time(std::string str_time, time_t &time_start, time_t &time_end)
// {
//     // 格式: 2000-11-11 23:20:21 2022-10-11 20:10:51
//     std::string time_format("(\\d{1,4}-(?:1[0-2]|0?[1-9])-(?:0?[1-9]|[1-2]\\d|30|31)) ((?:[01]\\d|2[0-3]):[0-5]\\d:[0-5]\\d)");
//     std::regex reg("^" + time_format + " +" + time_format + "$");
//     if (!std::regex_match(str_time, reg))
//     {
//         std::cout << "error: invalid time: " << str_time << std::endl;
//         return false;
//     }
//     reg.assign("(\\d{1,4}-(?:1[0-2]|0?[1-9])-(?:0?[1-9]|[1-2]\\d|30|31)) ((?:[01]\\d|2[0-3]):[0-5]\\d:[0-5]\\d)");
//     std::sregex_iterator it(str_time.begin(), str_time.end(), reg);
//     time_start = str_to_time(it.str());
//     ++it;
//     time_end = str_to_time(it.str());
//     return true;
// }

int ArgParser::convert_time(std::string str_time, time_t &time)
{
    // 格式: 2000-11-11 23:20:21 2022-10-11 20:10:51
    std::string time_format("(\\d{1,4}-(?:1[0-2]|0?[1-9])-(?:0?[1-9]|[1-2]\\d|30|31)) ((?:[01]\\d|2[0-3]):[0-5]\\d:[0-5]\\d)");
    std::regex reg("^" + time_format + "$");
    if (!std::regex_match(str_time, reg))
    {
        std::string error_log = "错误：非法的时间：" + str_time;
        throw std::runtime_error(error_log);
        return -1;
    }
    reg.assign("(\\d{1,4}-(?:1[0-2]|0?[1-9])-(?:0?[1-9]|[1-2]\\d|30|31)) ((?:[01]\\d|2[0-3]):[0-5]\\d:[0-5]\\d)");
    std::sregex_iterator it(str_time.begin(), str_time.end(), reg);
    time = str_to_time(it->str());
    return 0;
}

int ArgParser::parse() {
    if (check_backup_paths())
    {
        return -1;
    }
    if (filter_info.namePatten_checkBox)
    {
        if (check_reg_exp())
        {
            return -1;
        }
    }
    if (filter_info.from_accessTime_checkBox)
    {
        if (convert_time(time_string.from_accessTime, filter_info.from_accessTime))
        {
            return -1;
        }
    }
    if (filter_info.to_accessTime_checkBox)
    {
        if (convert_time(time_string.to_accessTime, filter_info.to_accessTime))
        {
            return -1;
        }
    }
    if (filter_info.from_createTime_checkBox)
    {
        if (convert_time(time_string.from_createTime, filter_info.from_createTime))
        {
            return -1;
        }
    }
    if (filter_info.to_createTime_checkBox)
    {
        if (convert_time(time_string.to_createTime, filter_info.to_createTime))
        {
            return -1;
        }
    }
    if (filter_info.from_modifyTime_checkBox)
    {
        if (convert_time(time_string.from_modifyTime, filter_info.from_modifyTime))
        {
            return -1;
        }
    }
    if (filter_info.to_modifyTime_checkBox)
    {
        if (convert_time(time_string.to_modifyTime, filter_info.to_modifyTime))
        {
            return -1;
        }
    }
    return 0;
}

time_t ArgParser::str_to_time(std::string str_time)
{
    char *cha = (char *)str_time.data();
    tm tm_;
    int year, month, day, hour, minute, second;
    sscanf(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
    tm_.tm_year = year - 1900; // 年
    tm_.tm_mon = month - 1;    // 月
    tm_.tm_mday = day;         // 日
    tm_.tm_hour = hour;        // 时
    tm_.tm_min = minute;       // 分
    tm_.tm_sec = second;       // 秒
    tm_.tm_isdst = 0;          // 非夏令时
    time_t t_ = mktime(&tm_);  // 将tm结构体转换成time_t格式
    return t_;
}
