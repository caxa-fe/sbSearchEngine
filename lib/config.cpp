#include "config.h"


Config::Config() 
{
    // Reading config.json
    try
    {
        std::string cFile = std::string{CURRENT_SOURCE_DIR} + "/" + std::string{CONFIG_FILE};
        std::ifstream in(cFile);       
        if (!in.is_open())
        {
            throw std::string{"ERROR: Can't open " + std::string{CONFIG_FILE} + " for reading."};
        }
        in >> Config::config;
        in.close();
    }

    catch (std::wstring error_message)
    {
        std::wcout << error_message << std::endl;
        std::exit(1);
    }

    try
    {
        if (!Config::config.contains("version"))
        {
            throw "ERROR: Version number not define in config file.";
        }
    }
    catch (const char* error_message)
    {
        std::wcout << error_message << std::endl;
        std::exit(2);
    }
    try
    {
        if (!Config::config.contains("files") || Config::config["files"].empty())
        {
            throw "ERROR: Files list is empty. Stopped.";
        }
    }
    catch (const char* error_message)
    {
        std::wcout << error_message << std::endl;
        exit(3);
    }
    if (!Config::config.contains("max_responses") || Config::config["max_responses"] < 1)
    {
        std::wcout << "WARNING: max_responses not define in config file. Assigned max_responses = "<< DEFAULT_MAX_RESPONSES << std::endl;;
        Config::config["max_responses"] = DEFAULT_MAX_RESPONSES;
    }
    if (!Config::config.contains("update_interval") || Config::config["update_interval"] < 1)
    {
        std::wcout << "WARNING: update_interval not define in config file. Assigned update_interval = " << DEFAULT_UPDATE_INTERVAL << std::endl;;
        Config::config["update_interval"] = DEFAULT_UPDATE_INTERVAL;
    } 

// Reading request.json
    try
    {
        std::string cFile = std::string{CURRENT_SOURCE_DIR} + "/" + std::string{REQUEST_FILE};
        std::ifstream in(cFile);       
        if (!in.is_open())
        {
            std::string str {"ERROR: Can't open " + std::string{REQUEST_FILE} + " for reading."};
            throw std::wstring{str.begin(),str.end()};
        }
        in >> Config::request;
        in.close();
    }

    catch (std::wstring error_message)
    {
        std::wcout << error_message << std::endl;
        std::exit(1);
    }

    try
    {
        if (!Config::request.contains("requests") || Config::request["requests"].empty())
        {
            throw "ERROR: Request list is empty. Stopped.";
        }
    }
    catch (const char* error_message)
    {
        std::wcout << error_message << std::endl;
        exit(3);
    }
}

const nlohmann::json& Config::getConfig() const
{
    return Config::config;
}

const nlohmann::json& Config::getRequest() const
{
    return Config::request;
}
//Config::~Config() {}


