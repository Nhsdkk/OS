#pragma once

#include <string>
#include <vector>
#include <functional>

void ReadData(std::function<void(const std::string&)> handler);
std::string Modify(const std::string& str);
void OpenPipe(int *p);