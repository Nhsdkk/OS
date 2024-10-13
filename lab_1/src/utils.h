#pragma once

#include <string>
#include <vector>
#include <functional>

void ReadData(const std::function<void(const std::string&)>& handler, std::basic_istream<char>& stream);
std::string Modify(const std::string& str);
void OpenPipe(int *p);