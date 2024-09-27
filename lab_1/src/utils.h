#pragma once

#include <string>
#include <vector>

std::string Concat(std::vector<std:: string>& strings, char sep = '\n');
std::vector<std::string> ReadData();
std::string Modify(const std::string& str);
void OpenPipe(int *p);