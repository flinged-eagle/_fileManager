#pragma once
#include"fileManager.h"

class fileManagerTool {
public:
	void scanner();
	void deleteByName();
	void deleteByMD5();
	void deleteAllCopy();
	void deleteMatchCopy();
	void showCopy();
	void showAllFiles();
private:
	FileManager _fm;
};