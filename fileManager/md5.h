#pragma once

#include<string>

using std::string;

typedef unsigned int uint32;

//�����ʼABCD
//ѭ����λ��λ��s[i]
//k[i] = floor(2^(32) * abs(sin(i+1))
//���ݿ飺64byte
//���ֽ�������������ݿ�ĩβ
//���һ�����ݵ��ֽ���


//������
//λ���㺯����FGHI
//ѭ������

class MD5 {
public:
	MD5();
	//��ʼ��
	void init();
	//FGHI �������������еĺ���Ĭ��Ϊ��������
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
	//ѭ�����ƺ���
	uint32 leftShift(uint32 number, int leftNumber)
	{
		return (number << leftNumber) | (number >> (32 - leftNumber));
	}
	//����
	void reset();
	//����һ�����ݿ��MD5ֵ
	void callMD5(uint32* chunk);
	//�������һ�����ݿ��MD5ֵ
	void callFinalMD5();
	//������ת��Ϊ��Ӧ��16�����ַ���
	string changeHex(uint32 n);
	string getStringMD5(const string& str);
	string getFileMD5(const char* path);
private:
	//ѭ����λ��λ�� s[i]
	static int _leftshift[64];
	//k[i]
	uint32 _k[64];
	//���ݿ�64byte
	char _chunk[64];
	//���ֽ��� 64bit  �����������64bit
	unsigned long long _totalByte;
	//���һ�����ݵ��ֽ���  ��������ֽ�
	uint32 _lastByte;
	//md5ֵ
	uint32 _a;
	uint32 _b;
	uint32 _c;
	uint32 _d;
};