#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_DISABLE_PERFCRIT_LOCKS

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

#ifndef _MSC_VER
#define _FILE_OFFSET_BITS 64
#define _ftelli64 ftello
#endif

#include "ulz.h"
#include "../compressionalgorithm.h"

#define ULZ_MAGIC 0x215A4C55 // "ULZ!"

#define BLOCK_SIZE (1 << 24)

class LZ77 : public ICompressionAlgorithm // 压缩解压类的头文件
{
public:
	// 压缩
	int compress(const std::string &file, const std::string &targetFile);

	// 解压
	int decompress(const std::string &file, const std::string &targetFile);
};
