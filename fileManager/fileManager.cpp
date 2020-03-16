#include"fileManager.h"
#include"fileUtil.h"
//ɨ�������Ϣ
void FileManager::scanDir(const string& path)
{
	//��������
	_file.clear();
	searchDir(path, _file);
	getMD5toFiles();
	getCopyList();
}

//��ȡ�ļ���MD5ֵ
void FileManager::getMD5toFiles()
{
	_md5toFiles.clear();
	for (auto& e : _file)
	{
		//��ʼ��MD5ֵ
		_md.reset();
		_md5toFiles.insert(make_pair(_md.getFileMD5(e.c_str()), e));
	}
}


//��ȡ�ظ��ļ�
void FileManager::getCopyList()
{
	_filestoMd5.clear();
	auto it = _md5toFiles.begin();
	while (it != _md5toFiles.end())
	{
		//�鿴MD5ֵ��ͬ���ļ������Ƿ�Ϊ1
		if (_md5toFiles.count(it->first) > 1)
		{
			//����ÿһ��MD5�ļ���Ӧ���ļ�����
			auto pairIt = _md5toFiles.equal_range(it->first);
			auto pairfirst = pairIt.first;
			while (pairfirst != pairIt.second)
			{
				//�����ظ��ļ���ӳ���ϵ
				_filestoMd5.insert(make_pair(pairfirst->second, pairfirst->first));
				pairfirst++;
			}
			//�ƶ�����һ����ͬMD5���ļ���λ��
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

//��������ɾ���ļ���ֻ����һ��
void FileManager::deleteByName(const string& name)
{
	if (_filestoMd5.count(name) == 0)
	{
		std::cout << name << "not exist!\n";
		return;
	}
	//��ǰMD5
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

//ɾ����Ӧmd5ֵ���ļ�
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

//�ظ��ļ�ֻ����һ��
void FileManager::deleteAllCopy()
{
	std::unordered_set<string> md5set;
	//�ҳ����е�MD5ֵ
	for (const auto& p : _md5toFiles)
	{
		md5set.insert(p.first);
	}
	for (const auto& p : md5set)
	{
		deleteByMD5(p);
	}
}

//ģ��ɾ����ɾ�����������ַ��е��ļ��е��ظ��ļ�
void FileManager::deleteByMatchName(const string& MatchName)
{
	std::unordered_set<string> allFiles;
	//���������ļ�
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

//��ʾ���Ƶ��ļ��б�
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
		//�жϼ����ڵ������ļ�
		while (curIt != pairIt.second)
		{
			std::cout << "\t��" << idx << "���ļ�" << std::endl;
			std::cout << curIt->second << std::endl;
			count++;
			idx++;
			curIt++;
		}
		it = pairIt.second;
	}
	std::cout << "�ļ�����Ϊ" << total <<"\t" <<count<< std::endl;
} 

void FileManager::showAllFile()
{
	for (const auto& f : _file)
	{
		std::cout << f << std::endl;
	}
	std::cout << "the number of file : " << _file.size() << std::endl;
}