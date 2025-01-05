#ifndef INCLUDE_PACKER_H_
#define INCLUDE_PACKER_H_

#include "filter.h"
#include <unordered_map>

#define FILE_SUFFIX_PACK ".pak"

class Packer
{
private:
    std::filesystem::path root_path; // 需要备份或恢复的文件路径
    std::filesystem::path bak_path;  // 打包文件的路径
    std::unordered_map<ino_t, std::string> inode_table;

    Filter* filter;

    // 深度优先遍历文件
    void dfs_file(FileBase &bak_file, std::filesystem::path cur_path);

public:
    Packer(std::string root_path_, std::string pack_path_, Filter* filter_ = nullptr);

    // 打包
    bool pack();

    // 解包
    bool unpack();
};

#endif // INCLUDE_PACKER_H_