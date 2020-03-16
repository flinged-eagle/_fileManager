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

//��ʼ��
void MD5::init()
{
	for (int i = 0; i < 64; ++i)
	{
		_k[i] = static_cast<uint32>(abs(sin(i + 1.0)) * pow(2.0, 32));
	}
	reset();
}

//����
void MD5::reset()
{
	//��ʼ��a b c d
	_a = 0x67452301;
	_b = 0xefcdab89;
	_c = 0x98badcfe;
	_d = 0x10325476;
	//��ʼ��chunk
	memset(_chunk, 0, 64);
	//����
	_lastByte = _totalByte = 0;
}

//����һ�����ݿ��MD5ֵ
void MD5::callMD5(uint32* chunk)
{
	int a = _a;
	int b = _b;
	int c = _c;
	int d = _d;
	int f, g;
	//ÿ�ĸ��ֽ�ÿ����������һ�ι�����64�Σ�
	for (int i = 0; i < 64; ++i)
	{
		//λ���� F G H I
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
		//�ӷ����㣬ѭ������
		int tmp = d;
		d = c;
		c = b;
		b = b + leftShift(a + f + chunk[g] + _k[i], _leftshift[i]);
		a = tmp;
	}
	//����a,b,c,d
	_a += a;
	_b += b;
	_c += c;
	_d += d;
}

void MD5::callFinalMD5()
{
	//���������Ϣ��������Ϣ����һ���ֽ�
	//��һ��bitΪ1����Ϊ0
	char* p = _chunk + _lastByte;
	//0x80 : 1000 0000
	*p++ = 0x80;
	//���ʣ����Ϣ
	int remainByte = 64 - _lastByte - 1;
	//�ж�ʣ�೤���Ƿ�64bit��8���ֽڣ����������Դ�ļ�����
	if (remainByte < 8)
	{
		//�Ƚ�ʣ��ȫ����0
		memset(p, 0, remainByte);
		//���㵱ǰ���ݿ��MD5ֵ
		callMD5((uint32*)_chunk);
		//������һ���ݿ�
		memset(_chunk, 0, 64);
	}
	else
	{
		//�������λȫΪ0
		memset(p, 0, remainByte-8);
	}
	//�����ݿ����64λ����ļ�ԭʼ������Ϣ
	unsigned long long totaBits = _totalByte;
	totaBits *= 8;
	((unsigned long long*)_chunk)[7] = totaBits;
	callMD5((uint32*)_chunk);
}

//������ת��Ϊ16�����ַ���
string MD5::changeHex(uint32 n)
{
	string num = "0123456789abcdef";
	string strNum;
	string tmp = "00";
	//������ÿ�ĸ�����ת��Ϊһ��16��������
	for (int i = 0; i < 8; ++i)
	{
		tmp[1] = num[n >> (i++ * 4) & 0x0f];
		tmp[0] = num[n >> (i * 4) & 0x0f];
		strNum += tmp;
	}
	return strNum;
}

//��ȡMD5ֵ���ַ�����ʽ
string MD5::getStringMD5(const string& str)
{
	if (str.empty())
	{
		return changeHex(_a).append(changeHex(_b)).append(changeHex(_c)).append(changeHex(_d));
	}
	_totalByte = str.size();
	uint32 chunkNum = _totalByte / 64;
	const char* strPtr = str.c_str();
	//�����������ݵ�MD5ֵ
	for (int i = 0; i < chunkNum; ++i)
	{
		memcpy(_chunk, strPtr + i * 64, 64);
		callMD5((uint32*)_chunk);
	}
	//�������һ�����ݵ�MD5ֵ
	_lastByte = _totalByte % 64;
	memcpy(_chunk, strPtr + chunkNum * 64, _lastByte);
	callFinalMD5();
	return changeHex(_a).append(changeHex(_b)).append(changeHex(_c)).append(changeHex(_d));
}

string MD5::getFileMD5(const char* path)
{
	ifstream fin(path, ifstream::binary);
	//�ж��ļ����������ļ��Ƿ�󶨣��󶨷���true��δ�󶨷���false;
	if (!fin.is_open())
	{
		std::cout << path << "\n";
		perror("file open failed");
	}
	while (!fin.eof())
	{
		fin.read(_chunk, 64);
		//�ж��Ƿ�Ϊ���һ�����ݿ�
		if (fin.gcount() != 64)
		{
			//Ϊ���һ������
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