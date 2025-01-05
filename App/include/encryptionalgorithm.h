#ifndef ENCRYPTIONALGORITHM_H
#define ENCRYPTIONALGORITHM_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstring>

#define FILE_SUFFIX_ENCRYPT ".ept"

// IEncryptionAlgorithm 接口
class IEncryptionAlgorithm {
public:
    // 加密
    virtual int encrypt(const std::string &file, const std::string &targetFile, const std::string &key) = 0;

    // 解密
    virtual int decrypt(const std::string &file, const std::string &targetFile, const std::string &key) = 0;
};

#endif // ENCRYPTIONALGORITHM_H
