#include "request.h"

size_t Request::count {0};

Request::Request(std::string a_requestTxt)
{
    reqID = count;
    ++count;

    requestTxt = sbse::utf8_utf16(a_requestTxt);
    std::vector<std::wstring> reqWordsVec;
    sbse::String2Words(requestTxt, reqWordsVec);
    for (auto & el : reqWordsVec)
    {
        reqWords.emplace(el);
    }
}

const std::wstring &Request::GetRequest()
{
    return requestTxt;
}

const std::set<std::wstring>  &Request::GetReqWords()
{
    return reqWords;
}

const size_t &Request::GetReqID()
{
    return reqID;
}
