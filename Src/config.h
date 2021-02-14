#pragma once

#include <string>
#include "gl_const.h"

class Config
{
    public:
        Config();
        Config(const Config& orig);
        ~Config();
        bool PrepareConfig(const char *FileName);

    public:
        double          SearchParams[SEARCH_PARAMS_NUM];
        std::string     LogParams[LOG_PARAMS_NUM];
};
