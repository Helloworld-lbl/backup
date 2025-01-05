#ifndef INCLUDE_FILEBASE_H_
#define INCLUDE_FILEBASE_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <fcntl.h>
#include <iostream>
#include <openssl/md5.h>
#include <iomanip>
#include <sstream>

#define MAX_PACK_PATH_LEN 256   // 最大路径长度
#define BLOCK_BUFFER_SIZE 4096  // 读写文件的缓冲区大小
#define BACKUP_COMMENT_SIZE 255 // 备份时的备注信息长度
#define CHECKSUM_LENGTH MD5_DIGEST_LENGTH * 2 + 1

// 文件类型
#define FILE_TYPE_NORMAL 1
#define FILE_TYPE_HARD_LINK 2
#define FILE_TYPE_SYMBOLIC_LINK 4
#define FILE_TYPE_DIRECTORY 8
#define FILE_TYPE_FIFO 16
#define FILE_TYPE_OTHER 32

#define EPT_NONE 0
#define EPT_AES 1
#define EPT_DES 2

#define CPS_NONE 0
#define CPS_Huffman 1
#define CPS_LZ77 2

typedef unsigned char FileType;

struct BackupInfo
{
    time_t timestamp;          // 时间戳
    std::string checksum;      // 校验和
    std::string src_path;      // 备份路径
    std::string bak_path;      // 目标路径
    int encryption_algorithm;  // 加密算法
    int compression_algorithm; // 压缩算法
    std::string key;           // 密码
};

struct FileInfo
{
    time_t timestamp;                 // 时间戳
    char checksum[CHECKSUM_LENGTH];   // 校验和
    char src_path[MAX_PACK_PATH_LEN]; // 备份路径
    uint8_t encryption_algorithm;     // 加密算法
    uint8_t compression_algorithm;    // 压缩算法
};

struct FileHeader
{
    char name[MAX_PACK_PATH_LEN];
    struct stat metadata;

    // 对于硬链接，只有其链接目标在备份文件范围内该项才有效
    char linkname[MAX_PACK_PATH_LEN];
};

#define FILE_HEADER_SIZE (sizeof(FileHeader))

class FileBase : public std::fstream
{
private:
    FileHeader fileheader;           // 文件元数据
    std::filesystem::path file_path; // 文件路径

public:
    // 传入文件元数据 创建文件
    FileBase(FileHeader fileheader_);
    // 传入文件路径 获取文件元数据
    FileBase(std::filesystem::path filepath_);

    bool open_file(std::ios_base::openmode mode_);

    // 从当前文件中读一个文件头信息
    FileHeader read_file_header();

    // 从备份文件中读取备份信息
    int read_backup_info(BackupInfo &backup_info);

    // 从备份文件中读取备份信息并去掉信息头
    int read_out_backup_info(BackupInfo &backup_info);

    // 向打包好的文件写入备份信息，形成备份文件
    void write_backup_info(BackupInfo &info, std::string output_path);

    size_t get_file_size();
    FileType get_file_type();
    bool is_hard_link();
    FileHeader get_file_header();

    void restore_metadata();

    static FileType get_file_type(const FileHeader &file_header_);
    std::string calculate_md5(bool skip);
};

#endif // INCLUDE_FILEBASE_H_
