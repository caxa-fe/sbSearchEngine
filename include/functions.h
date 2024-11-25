#pragma once
#include "lib.h"
#include "config.h"
#include "indexfile.h"
#include "request.h"
#include <queue>
#include <thread>
#include <future>

void readConfig();
void makeIndex(std::deque<IndexFile>&);
IndexFile makeIndexFromFile(std::string, size_t);
void makeCommonIndex(std::deque<IndexFile> &, std::map<std::wstring, std::map<size_t, size_t> > &); // [word][docID,count]
void makeSearch( const std::map<std::wstring, std::map<size_t, size_t> > &, std::deque<Request> &); // CommonIndex&, Request&
void saveResult( const std::deque<Request> &, char & );