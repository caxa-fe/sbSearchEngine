#pragma once

#include "constants.h"
#include "nlohmann/json.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <locale>
#include <codecvt>
#include <cwctype>


namespace sbse {

const char* getVersion();

const char* getProjectName();

void hello();

std::wstring utf8_utf16(const std::string&);

void ToLowerRu(wchar_t &);

void String2Words(std::wstring &, std::vector<std::wstring>&);

//template<typename T>
std::string toString(size_t);
std::string toString(double);


}

