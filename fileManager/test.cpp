#include"fileManager.h"
#include<iostream>
int main()
{
	FileManager fw;
	fw.scanDir("C:\\Users\\TopØ¼ıR\\source\\repos\\fileManager\\test");
	fw.showAllFile();
	std::cout << std::endl;
	fw.showCopyList();
	std::cout << std::endl;
	fw.deleteAllCopy();
	std::cout << std::endl;
	fw.showAllFile();
	return 0;
}