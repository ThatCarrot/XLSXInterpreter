#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <libxl.h>
#include <vector>
#include <filesystem>
#include <iostream>
#include <regex>
#include <ctime>
#include <fstream>

using namespace libxl;

std::vector<std::string>* getXlsxFiles(std::string folder);
Book* loadFile(std::string name);
std::vector<std::string>* getColummnNames(Sheet* sheet);
std::vector<std::string>* XLSXInterpreter();
int XLSXGrouper(std::vector<std::string>* files);
std::vector<std::string>* filterType1(std::vector<std::string>* input);
std::vector<std::string>* filterType2(std::vector<std::string>* input);
std::vector<std::string>* groupByMonth_T1(std::vector<std::string>* input, int month);
std::vector<std::string>* groupByMonth_T2(std::vector<std::string>* input, int month);
std::vector<std::string>* groupByYear_T1(std::vector<std::string>* input, int month);
std::vector<std::string>* groupByYear_T2(std::vector<std::string>* input, int month);
std::string XLSXExtractor_t1(std::vector<std::string>* files);
std::string XLSXExtractor_t2(std::vector<std::string>* files);
std::string extractUnixDateTime(const std::string& inputString);
std::vector<std::string>* getEntries_t1(std::string fileName, std::ofstream& logFile);
std::vector<std::string>* getEntries_t2(std::string fileName, std::ofstream& logFile);
void ignoreFile(std::ofstream& logFile, int state, std::string filename, int row);