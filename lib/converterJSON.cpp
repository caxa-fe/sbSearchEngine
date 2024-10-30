#include "converterJSON.h"


nlohmann::json ConverterJSON::config = ConverterJSON::MakeConfig();
nlohmann::json ConverterJSON::MakeConfig()
{
    try
    {
        std::string cFile = std::string{CURRENT_SOURCE_DIR} + "/" + std::string{CONFIG_FILE};
        std::ifstream in(cFile);       
        if (!in.is_open())
        {
            throw std::string{"ERROR: Can't open " + std::string{CONFIG_FILE} + " for reading."};
        }
        in >> config;
        in.close();
    }

    catch (std::string error_message)
    {
        std::cout << error_message << std::endl;
        std::exit(1);
    }

    try
    {
        if (!config.contains("version"))
        {
            throw "ERROR: Version number not define in config file.";
        }
    }
    catch (const char* error_message)
    {
        std::cout << error_message << std::endl;
        std::exit(2);
    }

    try
    {
        if (config["version"] != std::string{PROJECT_VERSION})
        {
            throw "ERROR: Version numbers of config file and application are not equiale.";
        }
    }
    catch (const char* error_message)
    {
        std::cout << error_message << std::endl;
        std::cout << "Config file version:\t" << config["version"] << std::endl;
        std::cout << "Application version:\t" << PROJECT_VERSION << std::endl;
        std::cout << "Stopped" << std::endl;;
        std::exit(2);
    }

    try
    {
        if (!config.contains("files") || config["files"].empty())
        {
            throw "ERROR: Files list is empty. Stopped.";
        }
    }
    catch (const char* error_message)
    {
        std::cout << error_message << std::endl;
        exit(3);
    }

    if (!config.contains("max_responses") || config["max_responses"] < 1)
    {
        std::cout << "max_responses not define in config file. Assigned max_responses = "<< std::string{DEFAULT_MAX_RESPONSES} << std::endl;;
        config["max_responses"] = DEFAULT_MAX_RESPONSES;
    }
    if (!config.contains("update_interval") || config["update_interval"] < 1)
    {
        std::cout << "update_interval not define in config file. Assigned update_interval = " << std::string{DEFAULT_UPDATE_INTERVAL} << std::endl;;
        config["update_interval"] = DEFAULT_UPDATE_INTERVAL;
    }  
    return config;
}

    /**
    * Метод получения содержимого файлов, по которым ведется поиск
    * Возвращает вектор строк с содержимым файлов, перечисленных
    * в config.json в разделе "files"
    * 
    */
std::vector<std::string> ConverterJSON::textDocuments = ConverterJSON::GetTextDocuments();
std::vector<std::string> ConverterJSON::GetTextDocuments()
{
    int i=0;
    for (nlohmann::json::iterator it = ConverterJSON::config["files"].begin(); it != ConverterJSON::config["files"].end(); ++it)
    {
        std::cout << config["files"][i] << "\n";
        std::string line {};
        try
        {
            std::ifstream in(std::string{*it});
            if (!in.is_open())
            {
                throw std::string{"WARNING: Can't open " + std::string{*it} + " for reading. Skipped."};
            }
            
            std::string ln {};
            while (std::getline(in, ln))
            {
                line += "\n" + ln;
            }
            in.close();
            
        }

        catch (std::string error_message)
        {
            std::cout << error_message << std::endl;
        }

        textDocuments.push_back(line);
        ++i;
    }
    return textDocuments;
}

/**
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
*/
int ConverterJSON::GetResponsesLimit()
{
    return ConverterJSON::config["max_responses"];
}
/**
 * Найти подстроку в документе docID = index
 * Отладочный метод
 */
size_t ConverterJSON::FindSubstring(int index, std::string str)
{
    return textDocuments[index].find(str);
}
/**
 * Вывод первых count символов текстовых документов в консоль
 * Отладочный метод
 */
void ConverterJSON::ShowContent(int start = 0, int count = 100) // Отладочный метод
{
    for( auto &el : ConverterJSON::textDocuments)
    {
        std::cout << el.substr(start, count) << std::endl;
    }
}


/**
* Метод выводит содержимое объекта nlohmann::json в консоль
* Отладочный метод
*/
void ConverterJSON::showConfig()
{
    for (nlohmann::json::iterator it = ConverterJSON::config.begin(); it != ConverterJSON::config.end(); ++it) 
    {
        std::cout << it.key() << ": " << it.value() << "\n";
    } 
}

/**
 * Метод возвращает список имен файлов из раздела "files"
 * объекта config.
 * В дальнейшем этим именам будут присвоены docID, см. класс TextDocs
 */
std::vector<std::string> ConverterJSON::GetDocsFileNames()
{
    std::vector<std::string> resultVector;
    for (nlohmann::json::iterator it = ConverterJSON::config["files"].begin(); it != ConverterJSON::config["files"].end(); ++it)
    {
        resultVector.push_back(std::string{*it});
    }
    return resultVector;
}

nlohmann::json ConverterJSON::request = ConverterJSON::MakeRequest();
nlohmann::json ConverterJSON::MakeRequest()
{
    try
    {
        std::string rFile = std::string{CURRENT_SOURCE_DIR} + "/" + std::string{REQUEST_FILE};
        std::ifstream in(rFile);       
        if (!in.is_open())
        {
            throw std::string{"ERROR: Can't open " + std::string{REQUEST_FILE} + " for reading."};
        }
        in >> request;
        in.close();
    }

    catch (std::string error_message)
    {
        std::cout << error_message << std::endl;
        std::exit(1);
    }

    try
    {
        if (!request.contains("requests") || request["requests"].empty())
        {
            throw std::string{ "ERROR: \"requests\" section not found or it is empty in " + std::string{REQUEST_FILE} + " file."};
        }
    }
    catch (std::string error_message)
    {
        std::cout << error_message << std::endl;
        std::exit(10);
    }

    
    return request;
}

/**
 * Метод возвращает вектор запросов.
 * Каждый элемент - один запрос.
 * В классе Request каждому запросу присваивается ID
 */
std::vector<std::string> ConverterJSON::GetReqRawVector()
{
    std::vector<std::string> reqVec {};

    for (nlohmann::json::iterator it = ConverterJSON::request["requests"].begin(); it != ConverterJSON::request["requests"].end(); ++it)
    {
        if ( !(*it).empty() )
            reqVec.push_back(*it);
    }
    
    return reqVec;
}