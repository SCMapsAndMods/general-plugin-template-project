#pragma once
#include <string>
#include <fstream>

std::string getOutputIniPath(const std::string &inputDatPath);
std::string getOutputDatPath(const std::string &inputIniPath);

unsigned int getFileSize(std::ifstream &file);
