#pragma once
#include<vector>
#include<string>
#include<unordered_set>
#include<iostream>
#include<io.h>

void searchDir(const std::string& path, std::unordered_set<std::string>& subFile);

void deleteFile(const char* filename);