#pragma once

#include "map.h"
#include "config.h"
#include "searchresult.h"
#include "environmentoptions.h"
#include "scenaries.h"

class Tasker
{
    public:
        Tasker();
        Tasker(const char* fileName);
        ~Tasker();

        bool prepareMap();
        Map getMap() const;
        bool getConfig();
        bool createLog();
        void createEnvironmentOptions();
        void startSearch();
        void printSearchResultsToConsole();
        void saveSearchResultsToLog();
        
    private:
        Map                     map;
        Config                  config;
        EnvironmentOptions      options;
        const char*             fileName;
        SearchResult            sr;
};