#pragma once
#include<string>
#include<unordered_map>
#include<unordered_set>
#include"md5.h"

class FileManager {
public:
	//扫描磁盘信息
	void scanDir(const string& path);
	//获取重复文件
	void getCopyList();
	//获取文件的MD5值
	void getMD5toFiles();
	//删除相同MD5值的文件只留一份
	void deleteByName(const string& name);
	void deleteByMD5(const string& md5);
	void deleteAllCopy();
	//模糊删除，删除包含输入字符中的文件中 重复的文件
	void deleteByMatchName(const string& MatchName);
	//打印
	void showCopyList();
	void showAllFile();
	void showMD5Map();
private:
	//保存文件名
	std::unordered_set<string> _file;
	//建立MD5对文件的映射
	std::unordered_multimap<std::string, std::string> _md5toFiles;
	//文件对MD5的映射
	std::unordered_map<std::string, std::string> _filestoMd5;
	MD5 _md;
};