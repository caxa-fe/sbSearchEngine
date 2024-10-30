#pragma once
#include "lib.h"
#include "invertedindex.h"
#include "request.h"
#include <sstream>

class SearchServer
{
public:
    SearchServer() = default;
    static void MakeSearch(InvertedIndex &);
private:

    /** Структура RequestRelRaw хранит неотсортированные результаты из индекса
     * количество совпадений по каждому запросу
     * Заполняется в методе MakeSearch()
     */
    struct RequestRelRaw
    {
        std::map<size_t, std::map<size_t,size_t>> entry;
        std::vector<std::map<size_t, std::map<size_t,size_t>>> entryVector;
                         // [reqID]          [docID][count] 
    };

    /** Структура SortRel хранит отсортированные результаты 
     * и относительную релевантность. Заполняется в MakeSortRel()
     */
    struct SortRel
    {
        std::map<size_t, size_t> max; // [reqID][max_count]
        std::map<size_t, std::priority_queue< std::pair<size_t,size_t>>> entry;
            // [reqID]                                 [count] [docID]
        
        std::map<size_t, std::deque< std::pair<double,size_t>>> entryRank;
             // [reqID]                       [rank] [docID]
        
        /* 
        std::vector<std::map<size_t, std::priority_queue< std::pair<size_t,size_t>>>> entryVector;
                          // [reqID]                               [count] [docID]
        */
    };

    static SortRel MakeSortRel(const RequestRelRaw &);

    static void SaveAnswers(SortRel &);

};