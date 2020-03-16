#include"md5.h"
#include<iostream>
#include<math.h>
#include<fstream>

using std::ifstream;

int MD5::_leftshift[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7,
	12, 17, 22, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10,
	15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

MD5::MD5()
{
	init();
}

//初始化
void MD5::init()
{
	for (int i = 0; i < 64; ++i)
	{
		_k[i] = static_cast<uint32>(abs(sin(i + 1.0)) * pow(2.0, 32));
	}
	reset();
}

//重置
void MD5::reset()
{
	//初始化a b c d
	_a = 0x67452301;
	_b = 0xefcdab89;
	_c = 0x98badcfe;
	_d = 0x10325476;
	//初始化chunk
	memset(_chunk, 0, 64);
	//重置
	_lastByte = _totalByte = 0;
}

//计算一个数据块的MD5值
void MD5::callMD5(uint32* chunk)
{
	int a = _a;
	int b = _b;
	int c = _c;
	int d = _d;
	int f, g;
	//每四个字节每个函数处理一次共处理64次；
	for (int i = 0; i < 64; ++i)
	{
		//位运算 F G H I
		//F 0 ~ 15
		if (i >= 0 && i <= 15)
		{
			f = F(b, c, d);
			g = i;
		}
		//G 16 ~ 31
		else if (i >= 16 && i <= 31)
		{
			f = G(b, c, d);
			g = (5 * i + 1) % 16;
		}
		//H 32 ~ 47
		else if (i >= 32 && i <= 47)
		{
			f = H(b, c, d);
			g = (3 * i + 5) % 16;
		}
		//I 48 ~ 63
		else
		{
			f = I(b, c, d);
			g = (7 * i) % 16;
		}
		//加法运算，循环左移
		int tmp = d;
		d = c;
		c = b;
		b = b + leftShift(a + f + chunk[g] + _k[i], _leftshift[i]);
		a = tmp;
	}
	//更新a,b,c,d
	_a += a;
	_b += b;
	_c += c;
	_d += d;
}

void MD5::callFinalMD5()
{
	//填充冗余信息，冗余信息至少一个字节
	//第一个bit为1，后为0
	char* p = _chunk + _lastByte;
	//0x80 : 1000 0000
	*p++ = 0x80;
	//填充剩余信息
	int remainByte = 64 - _lastByte - 1;
	//判断剩余长度是否够64bit（8个字节），用于填充源文件长度
	if (remainByte < 8)
	{
		//先将剩余全部补0
		memset(p, 0, remainByte);
		//计算当前数据块的MD5值
		callMD5((uint32*)_chunk);
		//开辟下一数据块
		memset(_chunk, 0, 64);
	}
	else
	{
		//填充其他位全为0
		memset(p, 0, remainByte-8);
	}
	//给数据块最后64位填充文件原始长度信息
	unsigned long long totaBits = _totalByte;
	totaBits *= 8;
	((unsigned long long*)_chunk)[7] = totaBits;
	callMD5((uint32*)_chunk);
}

//将整数转化为16进制字符串
string MD5::changeHex(uint32 n)
{
	string num = "0123456789abcdef";
	string strNum;
	string tmp = "00";
	//二进制每四个数字转化为一个16进制数字
	for (int i = 0; i < 8; ++i)
	{
		tmp[1] = num[n >> (i++ * 4) & 0x0f];
		tmp[0] = num[n >> (i * 4) & 0x0f];
		strNum += tmp;
	}
	return strNum;
}

//获取MD5值的字符串形式
string MD5::getStringMD5(const string& str)
{
	if (str.empty())
	{
		return changeHex(_a).append(changeHex(_b)).append(changeHex(_c)).append(changeHex(_d));
	}
	_totalByte = str.size();
	uint32 chunkNum = _totalByte / 64;
	const char* strPtr = str.c_str();
	//计算整块数据的MD5值
	for (int i = 0; i < chunkNum; ++i)
	{
		memcpy(_chunk, strPtr + i * 64, 64);
		callMD5((uint32*)_chunk);
	}
	//计算最后一块数据的MD5值
	_lastByte = _totalByte % 64;
	memcpy(_chunk, strPtr + chunkNum * 64, _lastByte);
	callFinalMD5();
	return changeHex(_a).append(changeHex(_b)).append(changeHex(_c)).append(changeHex(_d));
}

string MD5::getFileMD5(const char* path)
{
	ifstream fin(path, ifstream::binary);
	//判断文件流对象与文件是否绑定，绑定返回true，未绑定返回false;
	if (!fin.is_open())
	{
		std::cout << path << "\n";
		perror("file open failed");
	}
	while (!fin.eof())
	{
		fin.read(_chunk, 64);
		//判断是否为最后一个数据块
		if (fin.gcount() != 64)
		{
			//为最后一块数据
			break;
		}
		_totalByte += 64;
		callMD5((uint32*)_chunk);
	}
	_lastByte = fin.gcount();
	_totalByte += fin.gcount();
	callFinalMD5();
	return changeHex(_a).append(changeHex(_b)).append(changeHex(_c)).append(changeHex(_d));
}