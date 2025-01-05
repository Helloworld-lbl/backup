#include "task.h"

Task::Task(BackupInfo backup_info, LogsModel *logs, Filter *filter)
    : info{backup_info}, logs{logs}, filter{filter}
{
    // if (info.src_path.length() == 0)
    //     info.src_path = ".";

    src_path = std::filesystem::absolute(info.src_path);
    bak_path = std::filesystem::absolute(info.bak_path);

    info.timestamp = std::time(nullptr);
    // info.src_path = src_path;
    // info.bak_path = bak_path;
}

bool Task::backup()
{
    // 判断路径是否存在
    if (!std::filesystem::exists(src_path))
    {
        std::string error_log = "错误：找不到文件或目录：" + src_path.string();
        logs->add_log(QString::fromStdString(error_log));
        return false;
    }

    // 判断文件名是否符合要求
    std::string name = bak_path.filename();
    std::regex reg("^[.]*[\\w]+[\\w.-]*$");
    if (!std::regex_match(name, reg))
    {
        std::string error_log = "错误：非法文件名：" + name;
        logs->add_log(QString::fromStdString(error_log));
        return false;
    }

    // 打包
    logs->add_log("开始打包...");
    Packer packer(src_path, bak_path, filter);
    if (!packer.pack())
    {
        logs->add_log("错误：打包失败！");
        return false;
    }
    bak_path += FILE_SUFFIX_PACK;

    // 压缩
    if (info.compression_algorithm)
    {
        logs->add_log("开始压缩...");

        ICompressionAlgorithm *compression;
        switch (info.compression_algorithm)
        {
        case CPS_Huffman:
            compression = new Huffman();
            break;
        case CPS_LZ77:
            compression = new LZ77();
            break;
        default:
            break;
        }
        std::string file = bak_path.string();
        std::string targetFile = bak_path.string() + ".cps";
        if (compression->compress(file, targetFile))
        {
            logs->add_log("错误：压缩失败！");
            return false;
        }
        std::filesystem::remove_all(bak_path);
        bak_path += ".cps";
    }

    // 加密
    if (info.encryption_algorithm)
    {
        logs->add_log("开始加密...");

        IEncryptionAlgorithm *encryption;
        switch (info.encryption_algorithm)
        {
        case EPT_AES:
            encryption = new AES();
            break;
        case EPT_DES:
            encryption = new DES();
            break;
        default:
            break;
        }
        std::string file = bak_path.string();
        std::string targetFile = bak_path.string() + ".ept";
        if (encryption->encrypt(file, targetFile, info.key))
        {
            logs->add_log("错误：加密失败！");
        }
        std::filesystem::remove_all(bak_path);
        bak_path += ".ept";
    }

    // 写入备份信息
    FileBase file(bak_path);
    file.open_file(std::ios::in | std::ios::out | std::ios::binary);
    file.write_backup_info(info, info.bak_path);
    file.close();

    Database::write_to_database(info);
    return true;
}

bool Task::restore()
{
    // 判断路径是否存在
    if (!std::filesystem::exists(bak_path))
    {
        std::cout << "error: no such file or directory: " << bak_path.string() << std::endl;
        return false;
    }

    // 解密
    if (info.encryption_algorithm)
    {
        logs->add_log("开始解密...");

        IEncryptionAlgorithm *encryption;
        switch (info.encryption_algorithm)
        {
        case EPT_AES:
            encryption = new AES();
            break;
        case EPT_DES:
            encryption = new DES();
            break;
        default:
            break;
        }
        std::string file = bak_path.string();
        std::string targetFile = bak_path.replace_extension("").string();
        if (encryption->decrypt(file, targetFile, info.key))
        {
            logs->add_log("错误：解密失败！");
            return false;
        }
        std::filesystem::remove_all(bak_path.string() + ".ept");
    }

    // 解压
    if (info.compression_algorithm)
    {
        logs->add_log("开始解压...");

        ICompressionAlgorithm *compression;
        switch (info.compression_algorithm)
        {
        case CPS_Huffman:
            compression = new Huffman();
            break;
        case CPS_LZ77:
            compression = new LZ77();
            break;
        default:
            break;
        }
        std::string file = bak_path.string();
        std::string targetFile = bak_path.replace_extension("").string();
        if (compression->decompress(file, targetFile))
        {
            logs->add_log("错误：解压失败！");
            return false;
        }
        std::filesystem::remove_all(bak_path.string() + ".cps");
    }

    // 解包
    logs->add_log("开始解包...");
    Packer packer(src_path, bak_path, filter);
    if (!packer.unpack())
    {
        logs->add_log("错误：解包失败！");
        return false;
    }
    if (info.compression_algorithm)
        std::filesystem::remove_all(bak_path);

    return true;
}
