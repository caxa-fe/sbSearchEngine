#pragma once
#include "lib.h"
#include "nlohmann/json.hpp"

class Config
{
public:
    static Config& GetLink()
    {
        static Config s ;
        return s;
    }
    const nlohmann::json& getConfig() const;
    const nlohmann::json& getRequest() const;


private:
    Config();
    ~Config(){};

    // Запрещаем копирование
    Config(Config const&) = delete;
    Config& operator= (Config const&)  = delete;
    
    nlohmann::json config;
    nlohmann::json request;
};