#include <iostream>
#include <string>
#include <locale>
#include <queue>
#include <thread>
#include <future>
#include "lib/lib.h"
#include "lib/config.h"
#include "lib/indexfile.h"
#include "lib/request.h"



void readConfig();
void makeIndex(std::deque<IndexFile>&);
IndexFile makeIndexFromFile(std::string, size_t);
void makeCommonIndex(std::deque<IndexFile> &, std::map<std::wstring, std::map<size_t, size_t> > &); // [word][docID,count]
void makeSearch( const std::map<std::wstring, std::map<size_t, size_t> > &, std::deque<Request> &); // CommonIndex&, Request&
void saveResult(const std::deque<Request> &);

int main()
{
    setlocale(LC_ALL, "");
    std::wcout << "--- Skillbox Search Engine ---" << std::endl;
    std::wcout << "         version " << PROJECT_VERSION << std::endl; 
    std::wcout << std::endl; 
    sbse::lang = '1' ; // Language code
    char choice = ' '; // Main menu choice
    readConfig();
    std::deque<IndexFile> deqIndex;
    std::deque<Request> deqRequest;
    std::map<std::wstring, std::map<size_t, size_t> > commonIndex; // [word][docID,count]
    commonIndex.clear();
    std::wstring msg ;
    std::wcout << L"----------------------------" << std::endl;

    while (true)
    {
        if (choice == '8')
        {
            if ( sbse::lang == '1' ) sbse::lang = '2';
            else sbse::lang = '1';
            choice = ' ';
        }
        else if (choice == '3')
        {
            readConfig();
            std::wcout << L"----------------------------" << std::endl;
            choice = ' ';
        }
        else if (choice == '4')
        {
            msg = L"Building index...";
            sbse::translate(msg, sbse::lang);
            std::wcout << msg << std::endl;
            makeIndex(deqIndex);
            makeCommonIndex(deqIndex, commonIndex);

            msg = L"Done.";
            sbse::translate(msg, sbse::lang);
            std::wcout << msg << std::endl;
            choice = ' ';
        }
        else if (choice == '5')
        {
            if (commonIndex.empty())
            {
                msg = L"Index is empty. You have to build index first.";
                sbse::translate(msg, sbse::lang);
                std::wcout << "** " << msg << "\n" << std::endl;
                choice = ' ';
                continue;
            }
            makeSearch(commonIndex, deqRequest);
            saveResult(deqRequest);
            choice = ' ';
        }
        else if (choice == '9')
        {
            std::wcout << L"Exiting..." << std::endl;
            exit(0);
        }
        std::wstring menuItem;
        menuItem = L"    -- MAIN MENU --";
        sbse::translate(menuItem, sbse::lang);
        std::wcout << menuItem << std::endl;
        menuItem = L"3 - Show configuration";
        sbse::translate(menuItem, sbse::lang);
        std::wcout << menuItem << std::endl;
        menuItem = L"4 - Build a new index";
        sbse::translate(menuItem, sbse::lang);
        std::wcout << menuItem << std::endl;
        menuItem = L"5 - Make search";
        sbse::translate(menuItem, sbse::lang);
        std::wcout << menuItem << std::endl;
        menuItem = L"8 - Переключиться на русский";
        sbse::translate(menuItem, sbse::lang);
        std::wcout << menuItem << std::endl;
        menuItem = L"9 - Quit";
        sbse::translate(menuItem, sbse::lang);
        std::wcout << menuItem << std::endl;

        
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.clear();
        sbse::showTitle(sbse::lang);
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


IndexFile makeIndexFromFile(std::string str, size_t docID)
{
    IndexFile inxFile(str, docID);
    return inxFile;
}

void makeIndex(std::deque<IndexFile>& deqIndex)
{
    deqIndex.clear();
    std::vector <std::future<IndexFile> > ftVec;
    Config& json = Config::GetLink();
    nlohmann::json cfg = json.getConfig();
    size_t docID {0};
    for (std::string  str : cfg["files"])
    {
        std::future<IndexFile> asyncFuture = std::async(makeIndexFromFile, str, docID);
        ++docID;
        deqIndex.push_back(asyncFuture.get());
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

void saveResult(const std::deque<Request> & deqRequest )
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
            throw std::wstring{str.begin(), str.end()};
        }
        o << j;
        o.close();

        std::wstring str = L"Answer saved in file: ";
        sbse::translate(str, sbse::lang);
        str += std::wstring{fName.begin() , fName.end()};
        std::wcout << str << std::endl;
    }

    catch (std::wstring error_message)
    {
        std::wcout << error_message << std::endl;
        std::exit(5);
    }
}