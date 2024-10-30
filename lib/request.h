#pragma once
#include "lib.h"
#include "converterJSON.h"

/**
 * Класс для работы с запросами.
 * Список запросов содержится в объекте nlohmann::json request
 * Здесь запрос получает reqID ,
 * конвертируется в wstring и разбивается на слова
 */
class Request
{
public:
    Request(std::string);
    const size_t &GetReqID();
    const std::wstring &GetRequest();
    const std::set<std::wstring>  &GetReqWords();
/**
 * Отладочные методы
 */

/*************************** */
private:
    static size_t count; // Для формирования ID
    size_t reqID;   // ID запроса
    std::wstring requestTxt;
    std::set<std::wstring> reqWords; // Набор слов из текстового файла    
};