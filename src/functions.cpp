#include "functions.h"

IndexFile makeIndexFromFile(std::string str, size_t docID)
{
    IndexFile inxFile(str, docID);
    return inxFile;
}

void makeIndex(std::deque<IndexFile>& deqIndex)
{
    deqIndex.clear();
    Config& json = Config::GetLink();
    nlohmann::json cfg = json.getConfig();

    size_t docID {0};
    std::vector <std::future<IndexFile> > asyncFuture;
    for (std::string  str : cfg["files"])
    {
        asyncFuture.push_back(std::async(makeIndexFromFile, str, docID++));
    }
    for (auto & el : asyncFuture)
    {
        deqIndex.push_back(el.get());
    }

    if (deqIndex.empty())
    {
        std::wstring msg {L"Something went wrong. Index is empty."};
        sbse::translate(msg, sbse::lang);
        std::wcout << msg << std::endl;  
        exit(4);      
    }

}

void makeCommonIndex(std::deque<IndexFile>& deqIndex, std::map<std::wstring, std::map<size_t, size_t> >& commonIndex) // commonIndex: [word][docID, count]
{
    if(deqIndex.empty())
    {    
        std::wstring msg {L"Index is empty. You have to build index first."};
        sbse::translate(msg, sbse::lang);
        std::wcout << msg << std::endl;
        return;
    }

    for (auto & obj : deqIndex)
    {
        for (const auto & word : obj.GetCountWords())
        {
            commonIndex[word.first][obj.GetDocID()] += word.second;
        }
    }
}

void makeSearch(const std::map<std::wstring, std::map<size_t, size_t> > & commonIndex, std::deque<Request> & deqRequest)
{
    if (commonIndex.empty())
    {   
        std::wcout << L"CommonIndex is empty." << std::endl; 
        return;
    }

    Config& json = Config::GetLink();
    nlohmann::json requests = json.getRequest();
    size_t reqID {0};
    for (auto & reqStr : requests["requests"])
    {
        Request req(reqStr, reqID);
        req.MakeSearch(commonIndex);
        ++reqID;
        deqRequest.push_back(req);
    }
    
}

void saveResult(const std::deque<Request> & deqRequest, char& lang )
{
    std::string j {"{ \"answers\": [ " };
    for(size_t i = 0; i < deqRequest.size(); ++i)
    {
        const Request & req = deqRequest[i];

        j += "{ \"reqID\" : " + sbse::toString(req.GetReqID()) + ", ";
        j += "\"request\" : \"" + req.GetReqString() +"\", ";
        if (!req.GetReqDone())
            j += "\"reqDone\" : false, \"result\" : false ";
        else
        {
            j += "\"reqDone\" : true, ";
            if (req.GetReqResult().empty())
                j += "\"result\" : false ";
            else
            {
                j += "\"result\" : true, \"relevance\" : {";
                size_t k = 0;
                for (auto & p : req.GetReqResult())
                {
                    j += "\"" + sbse::toString(p.first) + "\" : " + sbse::toString(p.second);
                    ++k;
                    if (k != req.GetReqResult().size())
                        j += ", ";
                }
                j += "}";
            }

        }
        j += "}";
        if ( i < deqRequest.size() - 1) 
            j += ", ";
    }
    j += "]} ";

    std::string fName = std::string{ANSWER_FILE};
    std::ofstream o(fName);
    try
    {
        if (!o.is_open())
        {
            std::string str {"ERROR: Can't open " + fName + " for writing."};
            std::wstring ex {str.begin(), str.end()};
            throw ex;
        }
        o << j;
        o.close();

        std::wstring wstr {L"Answer saved in file: "};
        sbse::translate(wstr, lang);
        wstr += std::wstring{fName.begin() , fName.end()};
        std::wcout << wstr << "\n-----------------------" << std::endl;
    }

    catch (std::wstring& error_message)
    {
        std::wcout << error_message << std::endl;
        std::exit(5);
    }
}

void readConfig()
{
    std::wcout << "-- Reading config" << std::endl;
    Config& json = Config::GetLink();
    nlohmann::json cfg = json.getConfig();

    std::wstring wstr;
    wstr = sbse::utf8_utf16(cfg["version"]);
    std::wcout << "Config version: " << wstr << std::endl;

    int digit {cfg["max_responses"]};
    std::wcout << "max_responses: " << std::to_wstring(digit) << std::endl;
    digit = {cfg["update_interval"]};
    std::wcout << "update_interval: " << std::to_wstring(digit) << std::endl;

    std::wcout << "\nrequests: " << std::endl;
    nlohmann::json req = json.getRequest();
    for (std::string  str : req["requests"])
    {
        std::wstring wstr = sbse::utf8_utf16(str);
        std::wcout << "   " << wstr << std::endl;
    }

    std::wcout << "\nfiles to search in: "<< std::endl;
    for (std::string  str : cfg["files"])
    {
        std::wstring wstr(str.begin(), str.end());
        std::wcout << "   " << wstr << std::endl;
    }
    std::wcout << "\n-- End of Config." << std::endl;
}