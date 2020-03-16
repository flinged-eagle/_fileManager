#include"fileManager.h"
#include"fileUtil.h"
//扫描磁盘信息
void FileManager::scanDir(const string& path)
{
	//清理容器
	_file.clear();
	searchDir(path, _file);
	getMD5toFiles();
	getCopyList();
}

//获取文件的MD5值
void FileManager::getMD5toFiles()
{
	_md5toFiles.clear();
	for (auto& e : _file)
	{
		//初始化MD5值
		_md.reset();
		_md5toFiles.insert(make_pair(_md.getFileMD5(e.c_str()), e));
	}
}


//获取重复文件
void FileManager::getCopyList()
{
	_filestoMd5.clear();
	auto it = _md5toFiles.begin();
	while (it != _md5toFiles.end())
	{
		//查看MD5值相同的文件个数是否为1
		if (_md5toFiles.count(it->first) > 1)
		{
			//查找每一个MD5文件对应的文件集合
			auto pairIt = _md5toFiles.equal_range(it->first);
			auto pairfirst = pairIt.first;
			while (pairfirst != pairIt.second)
			{
				//保存重复文件的映射关系
				_filestoMd5.insert(make_pair(pairfirst->second, pairfirst->first));
				pairfirst++;
			}
			//移动到下一个不同MD5的文件的位置
			//????
			it = pairIt.second;
		}
		else
		{
			//_file.erase(it->second);
			it = _md5toFiles.erase(it);
		}
	}
}

//根据名称删除文件，只保留一个
void FileManager::deleteByName(const string& name)
{
	if (_filestoMd5.count(name) == 0)
	{
		std::cout << name << "not exist!\n";
		return;
	}
	//当前MD5
	std::string curMD5 = _filestoMd5[name];
	std::cout << name << "------->" << _md5toFiles.count(curMD5) << std::endl;
	auto pairIt = _md5toFiles.equal_range(curMD5);
	auto curIt = pairIt.first;
	int count = 0;
	while (curIt != pairIt.second)
	{
		if (curIt->second != name)
		{
			_file.erase(curIt->second);
			_filestoMd5.erase(curIt->second);
			deleteFile(curIt->second.c_str());
			++count;
		}
		++curIt;
	}
	curIt = pairIt.first;
	while (curIt != pairIt.second) 
	{
		if (curIt->second != name)
		{
			_md5toFiles.erase(curIt);
			pairIt = _md5toFiles.equal_range(curMD5);
			curIt = pairIt.first;
		}
		++curIt;
	}
	std::cout << "delete file number:" << count << std::endl;
}

//删除对应md5值的文件
void FileManager::deleteByMD5(const string& md5)
{
	if (_md5toFiles.count(md5) == 0)
	{
		std::cout << md5 << " not exist!\n";
		return	;
	}
	auto it = _md5toFiles.find(md5);
	_file.erase(it->second);
	deleteFile(it->second.c_str());
}

//重复文件只保留一份
void FileManager::deleteAllCopy()
{
	std::unordered_set<string> md5set;
	//找出所有的MD5值
	for (const auto& p : _md5toFiles)
	{
		md5set.insert(p.first);
	}
	for (const auto& p : md5set)
	{
		deleteByMD5(p);
	}
}

//模糊删除，删除包含输入字符中的文件中的重复文件
void FileManager::deleteByMatchName(const string& MatchName)
{
	std::unordered_set<string> allFiles;
	//遍历所有文件
	for (const auto& f : _file)
	{
		if (f.find(MatchName) != string::npos)
		{
			allFiles.insert(f);
		}
	}
	for (const auto& f : allFiles)
	{
		if (_filestoMd5.count(f) != 0)
		{
			deleteByName(f);
		}
	}
}

//显示复制的文件列表
void FileManager::showCopyList()
{
	std::unordered_multimap<string, string>::iterator  it = _md5toFiles.begin();
	int total = _md5toFiles.size();
	int count = 0;

	while (it != _md5toFiles.end())
	{
		int idx = 1;
		auto pairIt = _md5toFiles.equal_range(it->first);
		auto curIt = pairIt.first;
		std::cout << "cur MD5:" << it->first << std::endl;
		//判断集合内的所有文件
		while (curIt != pairIt.second)
		{
			std::cout << "\t第" << idx << "个文件" << std::endl;
			std::cout << curIt->second << std::endl;
			count++;
			idx++;
			curIt++;
		}
		it = pairIt.second;
	}
	std::cout << "文件总数为" << total <<"\t" <<count<< std::endl;
} 

void FileManager::showAllFile()
{
	for (const auto& f : _file)
	{
		std::cout << f << std::endl;
	}
	std::cout << "the number of file : " << _file.size() << std::endl;
}