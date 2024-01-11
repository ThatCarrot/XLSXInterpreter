#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <filesystem>
#include <iostream>
#include <regex>
#include <ctime>
#include <fstream>
#include "OpenXLSX.hpp"

// v v v v v v Old Funcs with old lib v v v v v v
//Book* loadFile(std::string name);
//std::vector<std::string>* getColummnNames(Sheet* sheet);

// v v v v v v Used for preliminar study and filtering v v v v v v
int XLSXGrouper(std::vector<std::string>* files);

std::vector<std::string>* filterType1(std::vector<std::string>* input);
std::vector<std::string>* filterType2(std::vector<std::string>* input);
std::vector<std::string>* groupByMonth_T1(std::vector<std::string>* input, int month);
std::vector<std::string>* groupByMonth_T2(std::vector<std::string>* input, int month);
std::vector<std::string>* groupByYear_T1(std::vector<std::string>* input, int month);
std::vector<std::string>* groupByYear_T2(std::vector<std::string>* input, int month);


// v v v v v v Now used v v v v v v
std::vector<std::string>* getXlsxFiles(std::string folder);
std::vector<std::string>* getEntries_t1(std::string fileName, std::ofstream& logFile, int* totalRows, int* readRows);
std::vector<std::string>* getEntries_t2(std::string fileName, std::ofstream& logFile, int* totalRows, int* readRows);

std::string extractUnixDateTime(const std::string& inputString);
void logInvalidRow(std::ofstream& logFile, int state, std::string filename, int row);