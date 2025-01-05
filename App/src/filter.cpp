#include "filter.h"

Filter::Filter(FilterInfo filter_info_) 
    : filter_info(filter_info_)
{
}

bool Filter::check(const FileHeader &file_header_)
{
    FileType cur_file_type = FileBase::get_file_type(file_header_);
    // 尺寸匹配
    if (filter_info.from_fileSize_checkBox)
    {
        int cur_file_size = file_header_.metadata.st_size;
        if (cur_file_size < filter_info.from_fileSize)
        {
            return false;
        }
    }
    if (filter_info.to_fileSize_checkBox)
    {
        int cur_file_size = file_header_.metadata.st_size;
        if (cur_file_size > filter_info.to_fileSize)
        {
            return false;
        }
    }

    // 路径匹配
    if (filter_info.pathPatten_checkBox)
    {
        std::regex reg(filter_info.pathPatten_textField);
        std::string dir_path = file_header_.name;
        if (cur_file_type == FILE_TYPE_DIRECTORY)
        {
            std::filesystem::path file_path(file_header_.name);
            dir_path = file_path.parent_path().string();
        }
        if (!std::regex_search(dir_path, reg))
        {
            return false;
        }
    }

    // 文件名匹配
    if ((filter_info.namePatten_checkBox) && (cur_file_type != FILE_TYPE_DIRECTORY))
    {
        std::regex reg(filter_info.namePatten_textField);
        std::filesystem::path file_path(file_header_.name);
        if (!std::regex_search(file_path.filename().string(), reg))
        {
            return false;
        }
    }

    // 类型匹配
    if (filter_info.fileType_checkBox && !(cur_file_type & filter_info.file_type))
    {
        return false;
    }

    // 时间匹配
    if (filter_info.from_accessTime_checkBox)
    {
        time_t cur_file_sec = file_header_.metadata.st_atim.tv_sec;
        if (cur_file_sec < filter_info.from_accessTime)
        {
            return false;
        }
    }
    if (filter_info.to_accessTime_checkBox)
    {
        time_t cur_file_sec = file_header_.metadata.st_atim.tv_sec;
        if (cur_file_sec > filter_info.to_accessTime)
        {
            return false;
        }
    }
    if (filter_info.from_createTime_checkBox)
    {
        time_t cur_file_sec = file_header_.metadata.st_ctim.tv_sec;
        if (cur_file_sec < filter_info.from_createTime)
        {
            return false;
        }
    }
    if (filter_info.to_createTime_checkBox)
    {
        time_t cur_file_sec = file_header_.metadata.st_ctim.tv_sec;
        if (cur_file_sec > filter_info.to_createTime)
        {
            return false;
        }
    }
    if (filter_info.from_modifyTime_checkBox)
    {
        time_t cur_file_sec = file_header_.metadata.st_mtim.tv_sec;
        if (cur_file_sec < filter_info.from_modifyTime)
        {
            return false;
        }
    }
    if (filter_info.to_modifyTime_checkBox)
    {
        time_t cur_file_sec = file_header_.metadata.st_mtim.tv_sec;
        if (cur_file_sec > filter_info.to_modifyTime)
        {
            return false;
        }
    }
    return true;
}
