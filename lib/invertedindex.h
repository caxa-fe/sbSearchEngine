#pragma once
#include "lib.h"
#include "textdoc.h"

/*
*   Класс подготовки инвертированного индекса текстов, полученных из файлов.
*   С помощью класса методов класса TextDocs считываются тексты из файлов,
*   тексты разбиваются на слова, слова помещаются в векторы.
*   Здесь делается словарь. Ключи - это слова из векторов TextDocs,
*   значения ключей - опять словарь. Ключ - ID документа, значение - количество вхождений
*   слова в документе.
*/
class InvertedIndex
{

public:
    InvertedIndex() = default; 

    InvertedIndex(std::vector<std::string>);

    void UpdateDocumentBase(const std::vector<std::string>);

    const std::map<std::wstring, std::map<size_t,size_t> > GetInvertedIndex();
    
    /**
     * Метод возвращает вектор пар [docID] [count] для заданного слова
     */
    const std::vector<std::pair<size_t,size_t>> GetWordPairVec(std::wstring);   
    
    /**
     * Отладочный метод. Ищет в индексе заданное слово и выводит в консоль 
     * из индекса docID и количество вхождений
     */
    void ShowWord(std::wstring wstr);

private:
    static std::map<std::wstring, std::map<size_t,size_t> > invIndex; // [word][docID] = count
    std::vector<std::string> docFileNames;  // список имен файлов с текстами для индексации


};