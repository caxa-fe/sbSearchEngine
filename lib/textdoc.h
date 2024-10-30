#pragma once
#include "lib.h"
#include "converterJSON.h"

/*
*   Класс для работы с текстовыми файлами.
*   Файл utf8 считывается в строку std::string, 
*   строка конвертируется в std::wstring - utf16.
*   Затем, строка разбивается на слова, слова помещаются в вектор
*/

class TextDoc
{
public:
    TextDoc(std::string);
    size_t GetDocID();
    std::string GetTextFileName();
    const std::vector<std::wstring> &  GetDocWords();


private:
    static size_t count; // Для формирования ID
    size_t docID;   // ID текстового файла
    std::string textFileName;
    std::vector<std::wstring> docWords; // Вектор слов из текстового файла

// Helpers
// reading text from file
    std::string ReadFile(std::string);

// Break string in words
//    void String2Words(std::wstring &);

// Make wchar_ lowercase
//   void ToLowerRu(wchar_t &);
};

