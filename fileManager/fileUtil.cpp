#include"fileUtil.h"

void searchDir(const std::string& path, std::unordered_set<std::string>& subFile)
{
	std::string mathpath = path + "\\" + "*.*";
	//struct _finddata_t 是存储文件信息的结构体
	/*
		struct _finddata_t
		{
			unsigned attrib;		//文件属性
			time_t time_create;
			time_t time_access;
			time_t time_write;
			_fsize_t size;
			char name[_MAX_FNAME];
		};
	*/
	_finddata_t fileAttr;
	long handle = _findfirst(mathpath.c_str(), &fileAttr);
	if (handle == -1)
	{
		perror("search failed!");
		std::cout << mathpath << std::endl;
		return;
	}
	
	do {
		//相与为0表明不是文件，是文件夹
		//_A_ARCH（存档）、_A_HIDDEN（隐藏）、_A_NORMAL（正常）、_A_RDONLY（只读）、 _A_SUBDIR（文件夹）、_A_SYSTEM（系统）
		if (fileAttr.attrib & _A_SUBDIR)
		{
			if (strcmp(fileAttr.name, ".") != 0 && strcmp(fileAttr.name, "..") != 0)
			{
				std::string newPath = path + "\\" + fileAttr.name;
				searchDir(newPath, subFile);
			}
		}
		else
		{
			subFile.insert(path + "\\" + fileAttr.name);
		}
	} while (_findnext(handle,&fileAttr) == 0);
	_findclose(handle);
}

//删除文件
void deleteFile(const char* filename) {
	if (remove(filename) == 0)
	{
		std::cout << "delete file:" << filename << " success\n";
	}
	else
	{
		perror("delete file failed!");
	}
}