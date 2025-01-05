#ifndef COMANDEX_H
#define COMANDEX_H

#include <vector>
#include <iostream>
#include <fstream>
#include <limits>

#include "hnode.h"
#include "huffmancode_node.h"

using namespace std;

#include "../compressionalgorithm.h"

class Huffman : public ICompressionAlgorithm // 压缩解压类的头文件
{
public:
	// 压缩
	int compress(const std::string &file, const std::string &targetFile);

	// 解压
	int decompress(const std::string &file, const std::string &targetFile);

private:
	// 扫描源文件种字符的种类及个数
	int ScanCharacter(string file);

	// 建立哈夫曼树
	void CreateHuffmanTree();

	// 生成哈夫曼编码
	void CreateHuffmanCode();

protected:
	// 存储哈夫曼树的数组
	vector<Hnode> HuffmanTree;

	// 存储哈夫曼编码的数组
	vector<Huffmancode_node> Huffmancode;

	// 字符的种类数
	int H_number;
};

#endif
