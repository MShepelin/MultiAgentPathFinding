#pragma once

#include <string>

class Config
{
    public:
        Config();
        Config(const Config& orig);
        ~Config();
        bool PrepareConfig(const char *FileName);

    public:
        double*         SearchParams;
        std::string*    LogParams;
        unsigned int    N;

};
