#include "request.h"

Request::Request(const std::string & n_reqString, size_t n_reqID) : reqString(n_reqString), reqID(n_reqID), reqDone(false)
{
    std::wstring reqWString = sbse::utf8_utf16(reqString);

    for (size_t i = 0; i < reqWString.length(); ++i)
    {
        std::towlower(reqWString[i]);    // Все латинские символы в нижний регистр
        sbse::toLowerRu(reqWString[i]);    // Все русские символы в нижний регистр
    }
    reqWords.clear();
    sbse::string2Words(reqWString, reqWords);
    
}
Request::~Request(){}

const bool & Request::GetReqDone() const
{
    return reqDone;
}
const std::vector<std::wstring> & Request::GetReqWords() const
{
    return reqWords;
}
const size_t & Request::GetReqID() const
{
    return reqID;
}

const std::string & Request::GetReqString() const
{
    return reqString;
}

void Request::MakeSearch(const std::map<std::wstring, std::map<size_t, size_t> >& commonIndex)
{
    //std::wstring wstr = sbse::utf8_utf16(reqString);
    //std::wcout << wstr << std::endl;
    if(commonIndex.empty())
    {   
        std::wcout << L"** CommonIndex is empty" << std::endl; 
        return;
    }

    std::map<size_t,size_t> reqSum; // docID, count
    for (const std::wstring w : reqWords)
    {
        if (commonIndex.count(w))
        {
            for (const auto & [docID, count] : commonIndex.at(w))
            {
                if (!reqSum.count(docID)) reqSum[docID] = count;
                else reqSum[docID] += count;
            }
        }

    }

    size_t countMax {1};
    for (auto & p : reqSum)
    {
        reqSummary.push(std::pair<size_t,size_t>{p.second, p.first}); // priority_queue of pair(count,docID)
        if (countMax < p.second) countMax = p.second;
    }

    static_cast<double>(countMax);
    while (!reqSummary.empty())
    {
        auto p = reqSummary.top();
        reqSummary.pop();
        double rel = (static_cast<double>(p.first) / countMax) * double{DEFAULT_PRECISION} + 0.5;
        int roundRel = static_cast<int>(rel);
        rel = static_cast<double>(roundRel);
        rel /= double{DEFAULT_PRECISION};
        reqSumRel.push_back(std::pair(p.second, rel));

    }
    reqDone = true;
}

const std::vector<std::pair<size_t, double> > & Request::GetReqResult() const
{
    return Request::reqSumRel;
}