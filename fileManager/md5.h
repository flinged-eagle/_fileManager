#pragma once

#include<string>

using std::string;

typedef unsigned int uint32;

//所需初始ABCD
//循环移位的位数s[i]
//k[i] = floor(2^(32) * abs(sin(i+1))
//数据块：64byte
//总字节数：填充在数据块末尾
//最后一块数据的字节数


//函数：
//位运算函数：FGHI
//循环左移

class MD5 {
public:
	MD5();
	//初始化
	void init();
	//FGHI 内联函数：类中的函数默认为内联函数
	uint32 F(uint32 b, uint32 c, uint32 d)
	{
		return (b & c) | ((~b) & d);
	}
	uint32 G(uint32 b, uint32 c, uint32 d)
	{
		return (b & d) | (c & (~d));
	}
	uint32 H(uint32 b, uint32 c, uint32 d)
	{
		return b ^ c ^ d;
	}
	uint32 I(uint32 b, uint32 c, uint32 d)
	{
		return c ^ (b |(~d));
	}
	//循环左移函数
	uint32 leftShift(uint32 number, int leftNumber)
	{
		return (number << leftNumber) | (number >> (32 - leftNumber));
	}
	//重置
	void reset();
	//计算一个数据块的MD5值
	void callMD5(uint32* chunk);
	//计算最后一个数据块的MD5值
	void callFinalMD5();
	//将整数转换为对应的16进制字符串
	string changeHex(uint32 n);
	string getStringMD5(const string& str);
	string getFileMD5(const char* path);
private:
	//循环移位的位数 s[i]
	static int _leftshift[64];
	//k[i]
	uint32 _k[64];
	//数据块64byte
	char _chunk[64];
	//总字节数 64bit  所加在填充后的64bit
	unsigned long long _totalByte;
	//最后一块数据的字节数  用于填充字节
	uint32 _lastByte;
	//md5值
	uint32 _a;
	uint32 _b;
	uint32 _c;
	uint32 _d;
};