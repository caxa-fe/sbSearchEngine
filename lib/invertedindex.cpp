#include "invertedindex.h"
#include <thread>
#include <mutex>

InvertedIndex::InvertedIndex(std::vector<std::string>  docFileNames)
{
    InvertedIndex::UpdateDocumentBase(docFileNames);
}

std::map<std::wstring, std::map<size_t,size_t> > InvertedIndex::invIndex {}; // [word][docID] = count

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>  docFileNames)
{
    //std::vector<TextDoc> TextDocVec;
    std::deque<TextDoc> TextDocVec;

    for(auto &el : docFileNames)
    {
        TextDoc t(el);
        TextDocVec.push_back(t);
        // std::cout << "docID: " << t.GetDocID() << " " << t.GetTextFileName() << std::endl;
    }
    
    std::vector <std::thread> thVec;

    for(auto & doc : TextDocVec)
    {
        thVec.push_back (std::thread([&]()
        {
            std::mutex mtx;
            
            // std::cout << "thread ID:" << std::this_thread::get_id() << " docID: " << doc.GetDocID() << std::endl;
            const std::vector<std::wstring> docWords = doc.GetDocWords();
    
            for (auto &word : docWords)
            {
                mtx.lock();
                if (invIndex.count(word))
                {
                    ++invIndex[word][doc.GetDocID()];
                }
                else
                {
                    invIndex[word][doc.GetDocID()] =  1;
                    //invIndex[word].emplace(std::pair<size_t,size_t> {doc.GetDocID(), 1});
                }
                mtx.unlock();
            }
        
        }));
    } 

    for (auto & th : thVec)
    {
        th.join();
    }
}

const std::map<std::wstring, std::map<size_t,size_t> > InvertedIndex::GetInvertedIndex()
{
    return invIndex;
}

const std::vector<std::pair<size_t,size_t>> InvertedIndex::GetWordPairVec(std::wstring word)
{
    // Пары [docID][count]
    std::vector<std::pair<size_t,size_t>> wordPairVec {};

    if (invIndex.count(word)) {
        for (auto & p : invIndex[word])
        {
            wordPairVec.push_back(p) ;
            // std::wcout << "GetWordPairVec: " << word << L" docID: " << p.first << L" count: " << p.second << std::endl;
        }
    }
    return wordPairVec;
} 

/**
 * Отладочный метод
 */
void InvertedIndex::ShowWord(std::wstring wstr)
{
    for (size_t i = 0; i < wstr.length(); ++i)
    {
        std::towlower(wstr[i]);
        sbse::ToLowerRu(wstr[i]);
    }
    
    for (const auto & [word, property] : invIndex)
    {
        if (word != wstr) continue;
        for (const auto & [docID, count] : property)
            std::wcout << L"ShowWord: " << word << " docID: " << docID << " count: " << count << std::endl;;

        std::cout << std::endl;
 
    }
}
