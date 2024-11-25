#pragma once
#include "constants.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <locale>
#include <codecvt>
#include <cwctype>




namespace sbse {
static char lang {'1'};  // Language code: 1 - En, 2 - Ru
void showTitle(char);
void selectLanguage(char &);

void translate(std::wstring &, const char &);

std::wstring utf8_utf16(const std::string&);

void toLowerRu(wchar_t &);

void string2Words(std::wstring &, std::vector<std::wstring> &);

//template<typename T>
std::string toString(size_t);
std::string toString(double);

}

