#pragma once
#include<string>
#include<unordered_map>
#include<unordered_set>
#include"md5.h"

class FileManager {
public:
	//ɨ�������Ϣ
	void scanDir(const string& path);
	//��ȡ�ظ��ļ�
	void getCopyList();
	//��ȡ�ļ���MD5ֵ
	void getMD5toFiles();
	//ɾ����ͬMD5ֵ���ļ�ֻ��һ��
	void deleteByName(const string& name);
	void deleteByMD5(const string& md5);
	void deleteAllCopy();
	//ģ��ɾ����ɾ�����������ַ��е��ļ��� �ظ����ļ�
	void deleteByMatchName(const string& MatchName);
	//��ӡ
	void showCopyList();
	void showAllFile();
	void showMD5Map();
private:
	//�����ļ���
	std::unordered_set<string> _file;
	//����MD5���ļ���ӳ��
	std::unordered_multimap<std::string, std::string> _md5toFiles;
	//�ļ���MD5��ӳ��
	std::unordered_map<std::string, std::string> _filestoMd5;
	MD5 _md;
};