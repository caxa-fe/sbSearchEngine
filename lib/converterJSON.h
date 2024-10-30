#pragma once
#include "lib.h"
#include "textdoc.h"

/**
* Класс для работы с config.json
*/

class ConverterJSON 
{
    friend class TextDoc;
    
public:
    ConverterJSON() = default; 

    /**
    * Метод читает config.json
    * Возвращает статический объект
    * nlohmann::json config
    */
    static nlohmann::json MakeConfig();

    /**
    * Метод получения содержимого файлов, по которым ведется поиск
    * Возвращает список с содержимым файлов перечисленных в config.json
    * static std::vector<std::string> textDocuments
    */
    static std::vector<std::string> GetTextDocuments();
    
    /**
    * Метод считывает поле max_responses для определения предельного
    * количества ответов на один запрос
    * 
    */
    static int GetResponsesLimit();

    /**
    * Вывод содержимого config в консоль
    * отладочный метод
    */
    static void showConfig();
    
    /**
     * Вывод первых count символов текстовых документов в консоль
     * Отладочный метод
     */
    static void ShowContent(int,int); 
    
    /**
     * Найти подстроку в документе 
     * Отладочный метод
     */
    static size_t FindSubstring(int, std::string);

    /**
     * Метод возвращает список имен файлов, перечисленных в разделе "files"
     * файла config.json
     */
    static std::vector<std::string> GetDocsFileNames();

    /**
    * Метод читает файл request.json
    * Возвращает статический объект
    * nlohmann::json request
    */
    static nlohmann::json MakeRequest();

    /**
     * Метод возвращает вектор строк из объекта nlohmann::json request.
     * Каждая строка - запрос.
     */
    static std::vector<std::string> GetReqRawVector();


private:
    static nlohmann::json config; // Объект nlohmann с содержимым файла config.json
    static std::vector<std::string> textDocuments; // тексты, в которых ведется поиск

    static nlohmann::json request; // Объект nlohmann с содержимым файла request.json
  
};



