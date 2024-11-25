#include <iostream>
#include <string>
#include <locale>
#include <queue>
#include <thread>
#include <future>
#include "lib.h"
#include "functions.h"



// Функции sbse:: см. lib.cpp

// Эти функции см. functions.cpp
// void readConfig();
// void makeIndex(std::deque<IndexFile>&);
// IndexFile makeIndexFromFile(std::string, size_t);
// void makeCommonIndex(std::deque<IndexFile> &, std::map<std::wstring, std::map<size_t, size_t> > &); // [word][docID,count]
// void makeSearch( const std::map<std::wstring, std::map<size_t, size_t> > &, std::deque<Request> &); // CommonIndex&, Request&
// void saveResult(const std::deque<Request> &);


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
        else if (choice == '1')
        {
            readConfig();
            std::wcout << L"----------------------------" << std::endl;
            choice = ' ';
        }
        else if (choice == '2')
        {
            msg = L"Building index...";
            sbse::translate(msg, sbse::lang);   //  lib.cpp
            std::wcout << msg << std::endl;
            makeIndex(deqIndex);    //  functions.cpp
            makeCommonIndex(deqIndex, commonIndex);     //  functions.cpp

            msg = L"Done.";
            sbse::translate(msg, sbse::lang);
            std::wcout << msg << std::endl;
            choice = ' ';
        }
        else if (choice == '3')
        {
            if (commonIndex.empty())
            {
                msg = L"Index is empty. You have to build index first.";
                sbse::translate(msg, sbse::lang);
                std::wcout << "** " << msg << "\n" << std::endl;
                choice = ' ';
                continue;
            }
            makeSearch(commonIndex, deqRequest);    //  functions.cpp
            saveResult(deqRequest, sbse::lang);     //  functions.cpp
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
        menuItem = L"1 - Show configuration";
        sbse::translate(menuItem, sbse::lang);
        std::wcout << menuItem << std::endl;
        menuItem = L"2 - Build a new index";
        sbse::translate(menuItem, sbse::lang);
        std::wcout << menuItem << std::endl;
        menuItem = L"3 - Make search";
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



