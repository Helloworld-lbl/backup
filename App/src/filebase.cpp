#include "filebase.h"

FileBase::FileBase(FileHeader fileheader_)
{
    this->fileheader = fileheader_;
    file_path.assign(fileheader.name);


    if (is_hard_link())
    {
        std::filesystem::remove_all(fileheader.name);
        std::filesystem::create_hard_link(fileheader.linkname, fileheader.name);
        return;
    }

    // 创建多级目录
    switch (get_file_type())
    {
    case FILE_TYPE_NORMAL:
        // 创建父目录
        std::filesystem::create_directories(file_path.parent_path());
        break;
    case FILE_TYPE_DIRECTORY:
        std::filesystem::create_directories(file_path.string());
        break;
    case FILE_TYPE_SYMBOLIC_LINK:
        // 需要先删除文件
        std::filesystem::remove_all(fileheader.name);
        std::filesystem::create_symlink(fileheader.linkname, fileheader.name);
        break;
    case FILE_TYPE_FIFO:
        // 需要先删除文件
        std::filesystem::remove_all(fileheader.name);
        mkfifo(fileheader.name, fileheader.metadata.st_mode);
        break;
    default:
        break;
    }
}

FileBase::FileBase(std::filesystem::path filepath_)
{
    file_path = filepath_;

    memset(&fileheader, 0, sizeof(fileheader));

    // 获取文件元信息
    lstat(file_path.string().c_str(), &(fileheader.metadata));
    strcpy(fileheader.name, file_path.string().c_str());

    if (get_file_type() == FILE_TYPE_SYMBOLIC_LINK)
    {
        char link_buf[MAX_PACK_PATH_LEN] = {0};
        readlink(file_path.string().c_str(), link_buf, MAX_PACK_PATH_LEN);
        strcpy(fileheader.linkname, link_buf);
    }
}

bool FileBase::open_file(std::ios_base::openmode mode_)
{
    this->open(file_path.string(), mode_);

    return is_open();
}

FileHeader FileBase::read_file_header()
{
    this->read((char *)&fileheader, FILE_HEADER_SIZE);
    return fileheader;
}

int FileBase::read_backup_info(BackupInfo &backup_info)
{
    FileInfo file_info;

    this->seekg(0, std::ios::beg);
    this->read((char *)&file_info, sizeof(file_info));

    backup_info.timestamp = file_info.timestamp;
    std::string checksum = calculate_md5(true);
    backup_info.checksum = file_info.checksum;
    if (checksum != backup_info.checksum) {
        throw std::runtime_error("校验和不匹配！");
    }
    backup_info.compression_algorithm = file_info.compression_algorithm;
    backup_info.encryption_algorithm = file_info.encryption_algorithm;
    backup_info.src_path = file_info.src_path;
    
    return 0;
}

int FileBase::read_out_backup_info(BackupInfo &backup_info)
{
    read_backup_info(backup_info);

    std::string tail = ".pak";
    if (backup_info.compression_algorithm != CPS_NONE)
    {
        tail += ".cps";
    }
    if (backup_info.encryption_algorithm != EPT_NONE)
    {
        tail += ".ept";
    }

    backup_info.bak_path += tail;

    std::ofstream ofs(backup_info.bak_path, std::ios::binary);

    // 跳过字节
    this->seekg(sizeof(FileInfo), std::ios::beg);

    // 读取并写入剩余内容
    ofs << this->rdbuf();

    ofs.close();

    return 0;
}

void FileBase::write_backup_info(BackupInfo& backup_info, std::string output_path)
{
    FileInfo file_info;
    file_info.timestamp = backup_info.timestamp;

    // 计算校验和
    // backup_info.checksum = 0;
    // uint32_t sum = backup_info.calc_check_sum();
    // file_info.checksum = (~sum) + 1;   // 按位取反后加1
    backup_info.checksum = calculate_md5(false);
    memcpy(file_info.checksum, backup_info.checksum.c_str(), CHECKSUM_LENGTH);
    file_info.checksum[CHECKSUM_LENGTH - 1] = '\0';
    file_info.compression_algorithm = backup_info.compression_algorithm;
    file_info.encryption_algorithm = backup_info.encryption_algorithm;
    memcpy(file_info.src_path, backup_info.src_path.c_str(), std::min(backup_info.src_path.length(), sizeof(file_info.src_path)));
    file_info.src_path[std::min(backup_info.src_path.length(), sizeof(file_info.src_path))] = '\0'; // 确保以空字符结尾

    std::ofstream ofs(output_path, std::ios::binary);
    ofs.write((char *)&file_info, sizeof(file_info));
    ofs << this->rdbuf();
    ofs.close();
}

size_t FileBase::get_file_size()
{
    return fileheader.metadata.st_size;
}

FileType FileBase::get_file_type()
{
    return FileBase::get_file_type(fileheader);
}

bool FileBase::is_hard_link()
{
    return (fileheader.metadata.st_nlink > 1);
}

FileHeader FileBase::get_file_header()
{
    return fileheader;
}

void FileBase::restore_metadata()
{
    struct stat *metadata = &(fileheader.metadata);

    // 还原文件权限信息
    chmod(fileheader.name, metadata->st_mode);

    // 还原文件的用户和组
    lchown(fileheader.name, metadata->st_uid, metadata->st_gid);

    // 还原时间戳(访问时间 修改时间)
    timespec tim[2] = {metadata->st_atim, metadata->st_mtim};
    utimensat(AT_FDCWD, fileheader.name, tim, AT_SYMLINK_NOFOLLOW);
}

FileType FileBase::get_file_type(const FileHeader &file_header_)
{
    switch (file_header_.metadata.st_mode & S_IFMT)
    {
    case S_IFREG:
        return FILE_TYPE_NORMAL;
        break;
    case S_IFDIR:
        return FILE_TYPE_DIRECTORY;
        break;
    case S_IFLNK:
        return FILE_TYPE_SYMBOLIC_LINK;
        break;
    case S_IFIFO:
        return FILE_TYPE_FIFO;
        break;
    default:
        return FILE_TYPE_OTHER;
        break;
    }
}

std::string FileBase::calculate_md5(bool skip) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("无法打开文件");
    }
    if (skip) {
        file.seekg(sizeof(FileInfo), std::ios::beg);
    }
    if (!file) {
        throw std::runtime_error("无法跳转到指定位置");
    }

    MD5_CTX mdContext;
    MD5_Init(&mdContext);

    char buffer[1024];
    while (file.read(buffer, sizeof(buffer))) {
        MD5_Update(&mdContext, buffer, file.gcount());
    }

    MD5_Update(&mdContext, buffer, file.gcount());

    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_Final(digest, &mdContext);

    std::ostringstream md5string;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        md5string << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }

    return md5string.str();
}
