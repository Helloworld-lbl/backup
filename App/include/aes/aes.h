#ifndef AES_H
#define AES_H

#include <openssl/evp.h>
#include <openssl/md5.h>

#include "../encryptionalgorithm.h"
#include "../filebase.h"

// 定义AES加密块大小
const int AES_BLOCK_SIZE = 16;

class AES : public IEncryptionAlgorithm
{
private:
    // 加密上下文
    EVP_CIPHER_CTX *m_encryptCtx;

    // 解密上下文
    EVP_CIPHER_CTX *m_decryptCtx;

    // MD5摘要
    unsigned char md5[MD5_DIGEST_LENGTH];

public:
    // AES加密函数
    int encrypt(const std::string &file, const std::string &targetFile, const std::string &key);

    // AES解密函数
    int decrypt(const std::string &file, const std::string &targetFile, const std::string &key);
};

#endif // AES_H
