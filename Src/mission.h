#pragma once

#include "xml_map.h"
#include "config.h"
#include "search.h"
#include "ilogger.h"
#include "searchresult.h"
#include "environmentoptions.h"
#include "search.h"
#include "xmllogger.h"

class Mission
{
    public:
        Mission();
        Mission (const char* fileName);
        ~Mission();

        bool prepareMap();
        XMLMap getMap() const;
        bool getConfig();
        bool createLog();
        void createSearch();
        void createEnvironmentOptions();
        void startSearch();
        void printSearchResultsToConsole();
        void saveSearchResultsToLog();
        SearchResult getSearchResult();
        
    private:
        XMLMap                     map;
        Config                  config;
        EnvironmentOptions      options;
        Search                  search;
        ILogger*                logger;
        const char*             fileName;
        SearchResult            sr;
};