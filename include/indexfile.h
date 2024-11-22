#pragma once
#include "lib.h"


class IndexFile
{
public:
    IndexFile(std::string &, size_t);   // "Doc file name", docID

    ~IndexFile();

    const std::map<std::wstring, size_t >& GetCountWords() const;
    const size_t & GetDocID() const;
    const bool & IsFileReadOK() const;
    const std::string & GetFileName() const;
    
private:
    size_t docID;
    std::string fName;
    std::map<std::wstring, size_t > countWords; // [word][count]
    std::string ReadFile(std::string &);
    bool fileReadOK;
};