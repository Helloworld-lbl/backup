#include "des/des.h"

// DES加密函数
int DES::encrypt(const std::string &file, const std::string &targetFile, const std::string &key)
{
    // 读取输入文件
    std::ifstream infile(file, std::ios::binary);
    if (!infile.is_open())
    {
        std::cerr << "无法打开输入文件" << std::endl;
        return -1;
    }

    // 创建输出文件
    std::ofstream outfile(targetFile, std::ios::binary);
    if (!outfile.is_open())
    {
        std::cerr << "无法创建输出文件" << std::endl;
        return -1;
    }

    // 哈希密钥
    unsigned char hashed_key[EVP_MAX_KEY_LENGTH];
    MD5((unsigned char *)(key.c_str()), key.length(), hashed_key);

    // 创建并初始化加密上下文
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_des_ede_ecb(), nullptr, hashed_key, nullptr);

    // 加密数据
    unsigned char in_buffer[DES_BLOCK_SIZE];
    unsigned char out_buffer[DES_BLOCK_SIZE];
    int len;
    while (infile.read(reinterpret_cast<char *>(in_buffer), DES_BLOCK_SIZE))
    {
        EVP_EncryptUpdate(ctx, out_buffer, &len, in_buffer, DES_BLOCK_SIZE);
        outfile.write(reinterpret_cast<char *>(out_buffer), len);
    }

    // 如有剩余内容则读入缓冲
    int final_len = infile.gcount();
    if (final_len)
    {
        infile.read(reinterpret_cast<char *>(in_buffer), final_len);
        EVP_EncryptUpdate(ctx, out_buffer, &len, in_buffer, final_len);
        outfile.write(reinterpret_cast<char *>(out_buffer), len);
    }

    // 处理剩余数据
    int res = EVP_EncryptFinal_ex(ctx, out_buffer, &len);
    outfile.write(reinterpret_cast<char *>(out_buffer), len);

    // 清理
    EVP_CIPHER_CTX_free(ctx);
    infile.close();
    outfile.close();

    return 0;
}

// DES解密函数
int DES::decrypt(const std::string &file, const std::string &targetFile, const std::string &key)
{
    // 读取输入文件
    std::ifstream infile(file, std::ios::binary);
    if (!infile.is_open())
    {
        std::cerr << "无法打开输入文件" << std::endl;
        return -1;
    }

    // 创建输出文件
    std::ofstream outfile(targetFile, std::ios::binary);
    if (!outfile.is_open())
    {
        std::cerr << "无法创建输出文件" << std::endl;
        return -1;
    }

    // 哈希密钥
    unsigned char hashed_key[EVP_MAX_KEY_LENGTH];
    MD5((unsigned char *)(key.c_str()), key.length(), hashed_key);

    // 创建并初始化解密上下文
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_des_ede_ecb(), nullptr, hashed_key, nullptr);

    // 解密数据
    unsigned char in_buffer[DES_BLOCK_SIZE];
    unsigned char out_buffer[DES_BLOCK_SIZE];
    int len;
    while (infile.read(reinterpret_cast<char *>(in_buffer), DES_BLOCK_SIZE))
    {
        EVP_DecryptUpdate(ctx, out_buffer, &len, in_buffer, DES_BLOCK_SIZE);
        outfile.write(reinterpret_cast<char *>(out_buffer), len);
    }

    // 处理剩余数据
    int res = EVP_DecryptFinal_ex(ctx, out_buffer, &len);
    outfile.write(reinterpret_cast<char *>(out_buffer), len);

    // 清理
    EVP_CIPHER_CTX_free(ctx);
    infile.close();
    outfile.close();

    return 0;
}
