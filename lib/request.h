#pragma once
#include "lib.h"

class Request
{
public:
    Request(const std::string &, size_t);   // "Request_string", reqID

    ~Request();

    const std::vector<std::wstring>& GetReqWords() const;
    const size_t & GetReqID() const;
    const bool & GetReqDone() const;
    const std::string & GetReqString() const;
    void MakeSearch(const std::map<std::wstring, std::map<size_t, size_t> > &); // commonIndex: [word][docID, count]
    const std::vector<std::pair<size_t, double> > & GetReqResult() const;

    
private:
    size_t reqID;
    bool reqDone;
    std::string reqString;
    std::vector<std::wstring> reqWords;
    std::priority_queue<std::pair<size_t, size_t> >reqSummary; //  count docID,
    std::vector<std::pair<size_t, double> >reqSumRel;   //  docID, relative

};

