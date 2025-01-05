#ifndef DES_H
#define DES_H

#include <openssl/evp.h>
#include <openssl/md5.h>

#include "../encryptionalgorithm.h"

// 定义DES加密块大小
const int DES_BLOCK_SIZE = 8;

class DES : public IEncryptionAlgorithm
{
public:
    // DES加密函数
    int encrypt(const std::string &file, const std::string &targetFile, const std::string &key);

    // DES解密函数
    int decrypt(const std::string &file, const std::string &targetFile, const std::string &key);

private:
    // 加密上下文
    EVP_CIPHER_CTX *m_encryptCtx;

    // 解密上下文
    EVP_CIPHER_CTX *m_decryptCtx;
};

#endif // DES_H

