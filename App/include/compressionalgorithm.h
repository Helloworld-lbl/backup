#ifndef COMPRESSIONALGORITHM_H
#define COMPRESSIONALGORITHM_H

#include <string>

// IEncryptionAlgorithm 接口
class ICompressionAlgorithm {
public:
    // 压缩
    virtual int compress(const std::string &file, const std::string &targetFile) = 0;

    // 解压
    virtual int decompress(const std::string &file, const std::string &targetFile) = 0;
};

#endif // COMPRESSIONALGORITHM_H
