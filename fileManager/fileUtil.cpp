#include"fileUtil.h"

void searchDir(const std::string& path, std::unordered_set<std::string>& subFile)
{
	std::string mathpath = path + "\\" + "*.*";
	//struct _finddata_t �Ǵ洢�ļ���Ϣ�Ľṹ��
	/*
		struct _finddata_t
		{
			unsigned attrib;		//�ļ�����
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
		//����Ϊ0���������ļ������ļ���
		//_A_ARCH���浵����_A_HIDDEN�����أ���_A_NORMAL����������_A_RDONLY��ֻ������ _A_SUBDIR���ļ��У���_A_SYSTEM��ϵͳ��
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

//ɾ���ļ�
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