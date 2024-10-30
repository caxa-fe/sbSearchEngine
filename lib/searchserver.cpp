#include "searchserver.h"

void SearchServer::MakeSearch(InvertedIndex & invIdx)
{

    RequestRelRaw reqRelRaw;
        
    ConverterJSON::MakeRequest();   // Считали содержимое файла request.json в JSON объект
    std::vector<Request> requestVector {}; // Вектор запросов, разбитых на слова

    for( auto &v : ConverterJSON::GetReqRawVector()) // Получаем строки с запросами из JSON объекта.
    {
        Request r(v);   // Кодирование каждого запроса в wstring и разбиение на слова.
        requestVector.push_back(r);
    }

    /**
     * Заполняем структуру reqRelRaw
     */
    for (auto & r : requestVector)  // Бежим по вектору запросов
    {
        //std::wcout << L"Request vector: " << L"reqID: " << r.GetReqID() << " " << r.GetRequest() << std::endl;
        
        std::map<size_t, size_t> wordCount; // [docID][count]
        for (auto & w : r.GetReqWords()) // Перебираем слова в запросе reqID
        {
            //std::wcout << L"слово: "<< w << std::endl;
            
            //invIdx.ShowWord(w);
            auto wordPairVec = invIdx.GetWordPairVec(w); // Вектор пар [docID][count] для слова w
            
            if (!wordPairVec.empty())
            {
                for (const auto & p : wordPairVec)
                {
                    //std::cout << "\t" << "docID: " << p.first << " count: " << p.second << std::endl;
                    if (wordCount.count(p.first))
                        wordCount[p.first] += p.second;
                    
                    else wordCount[p.first] = p.second;
                }
            }
        }
        /*
        Структура reqRelRaw :
        std::map<size_t, std::map<size_t,size_t>> entry; [reqID][docID][count] 
        std::vector<std::map<size_t, std::map<size_t,size_t>>> entryVector; 
                         // [reqID]          [docID][count] 
        */
        reqRelRaw.entry[r.GetReqID()] = wordCount;
        
        reqRelRaw.entryVector.push_back(reqRelRaw.entry);

        reqRelRaw.entry.clear();

        //std::wcout << std::endl;
        
    }
/*
        std::map<size_t, std::map<size_t,size_t>> entry;
        std::vector<std::map<size_t, std::map<size_t,size_t>>> entryVector;
                         // [reqID]          [docID][count]    
*/
    //std::cout << "--- reqRelRaw ---" << std::endl;
/*     
    // Для отладки.
    // Вывод в консоль содержимого структуры. 
    for (auto & v : reqRelRaw.entryVector)
    {
        for ( auto & e : v)
        {
            std::cout << "reqID: " << e.first << std::endl;
            for ( auto & p : e.second)
            {
                std::cout << "\tdocID: " << p.first << " count: " << p.second << std::endl;
            }
        }
    }
 */
    SortRel sortRel = MakeSortRel(reqRelRaw);

    //std::cout << "--- Rank calculation ---" << std::endl;

    for (auto &m : sortRel.entry)
    {
       // std::cout << "SortRel: reqID: " << m.first << std::endl;
       // std::cout << "\tmax: " << sortRel.max[m.first] << std::endl;

        std::priority_queue< std::pair<size_t,size_t>>pq = m.second;
        std::deque<std::pair<double,size_t>> dq;
        while (!pq.empty())
        {
            size_t docID = pq.top().second;
            size_t count = pq.top().first;
            double rank = static_cast<double>(count) / static_cast<double>(sortRel.max[m.first]);
            rank = round(rank * 1000)/1000;
            dq.emplace_back(std::pair<double,size_t>{rank,docID});
            pq.pop();
        }
        sortRel.entryRank[m.first] = dq;
        //dq.clear();
    } 
    /**
     * Отладка функции saveAnswers
     */
/* 
    SortRel sr;
    sr.max[1000] = 9;
    sr.max[1001] = 8;
    sr.max[1002] = 7;

    sr.rankQue.push(0.97);
    sr.rankQue.push(0.90);
    sr.rankQue.push(0.75);
    sr.rankQue.push(0.65);

    std::priority_queue< std::pair<size_t,size_t>> pq;
    pq.emplace(std::pair<size_t,size_t>{8,10});
    pq.emplace(std::pair<size_t,size_t>{7,11});
    pq.emplace(std::pair<size_t,size_t>{6,12});
    pq.emplace(std::pair<size_t,size_t>{5,13});
    sr.entry[1000] = pq;
    pq.pop();pq.pop();pq.pop();pq.pop();

    pq.emplace(std::pair<size_t,size_t>{4,10});
    pq.emplace(std::pair<size_t,size_t>{3,11});
    pq.emplace(std::pair<size_t,size_t>{2,12});
    pq.emplace(std::pair<size_t,size_t>{1,13});
    sr.entry[1001] = pq;
    pq.pop();pq.pop();pq.pop();pq.pop();

    pq.emplace(std::pair<size_t,size_t>{12,10});
    pq.emplace(std::pair<size_t,size_t>{11,11});
    pq.emplace(std::pair<size_t,size_t>{10,12});
    pq.emplace(std::pair<size_t,size_t>{13,13});
    sr.entry[1002] = pq;
    pq.pop();pq.pop();pq.pop();pq.pop();


    
    saveAnswers(sr);
 */
    SaveAnswers(sortRel);
}


SearchServer::SortRel SearchServer::MakeSortRel(const RequestRelRaw & raw)
{
    
    SortRel que;
    /*
    struct SortRel
    {
        std::map<size_t, size_t> max; // [reqID][max_count]
        std::map<size_t, std::priority_queue< std::pair<size_t,size_t>>> entry;
            // [reqID]                                 [count] [docID]
        
        std::map<size_t, std::deque< std::pair<double,size_t>>> entryRank;
             // [reqID]                       [rank] [docID]
    };    
    */
    /*
        Структура reqRelRaw :
        std::vector<std::map<size_t, std::map<size_t,size_t>>> entryVector; 
                         // [reqID]          [docID][count] 
    */

   
    for (auto & entry : raw.entryVector) // std::map<size_t, std::map<size_t,size_t>>
                                        // [reqID][docID][count] 
    {
        for (auto &e : entry)
        {
            size_t maxCount {0};
            std::priority_queue< std::pair<size_t, size_t>> pq;
            for ( auto & p : e.second)
            {
                pq.emplace(std::pair{p.second, p.first});
                if (p.second > maxCount) maxCount = p.second;
            }
            que.entry[e.first] = pq;
            que.max[e.first] = maxCount;
        }
    }
    return que;
}

void SearchServer::SaveAnswers(SortRel & sortRel)
{
    //nlohmann::json j;
    std::string j {"{\"answers\": {" };
    /*
    struct SortRel
        std::map<size_t, size_t> max; // [reqID][max_count]
        std::map<size_t, std::priority_queue< std::pair<size_t,size_t>>> entry;
            // [reqID]                                 [count] [docID]
        
        std::map<size_t, std::deque< std::pair<double,size_t>>> entryRank;
             // [reqID]                       [rank] [docID]
    */
   
    size_t k = sortRel.entry.size();
    for (auto & m : sortRel.entryRank)
    {
        --k;
        j += "\"request" + sbse::toString(m.first) + "\" : { \"result\" : ";
        if (m.second.empty())
        {
            j += "false }";
        } 
        else 
        {
            j += "true, \"relevance\" : {";

            for (size_t i = m.second.size(); i > 0; --i)
            {
                std::pair<double,size_t>p = m.second.front();
                j += "\"" + sbse::toString(p.second) + "\" : ";
                j += sbse::toString((p.first));
                if ( i > 1 ) j += ", ";
                m.second.pop_front();

            }
            j += "}";
        }
        j += "}";
        if ( k ) j += ",";
        
    }
    j += "}}";

    std::ofstream o(std::string{CURRENT_SOURCE_DIR} + "/" + std::string{ANSWER_FILE});
    try
    {
        if (!o.is_open())
        {
            throw std::string{"ERROR: Can't open " + std::string{ANSWER_FILE} + " for writing."};
        }
        o << j;
        o.close();
        std::cout << "Anwer saved in file: " + std::string{CURRENT_SOURCE_DIR} + "/" + std::string{ANSWER_FILE} << std::endl;
    }

    catch (std::string error_message)
    {
        std::cout << error_message << std::endl;
        std::exit(99);
    }
    o << j;
    
}

