#pragma once

#include "xml_map.h"
#include "config.h"
#include "astar.h"
#include "ilogger.h"
#include "searchresult.h"
#include "environmentoptions.h"
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
        void startSearch();
        void printSearchResultsToConsole();
        void saveSearchResultsToLog();
        SearchResult<GridCell> getSearchResult();
        
    private:
        XMLMap                  map;
        Config                  config;
        EnvironmentOptions      options;
        GridSingleSearch        search;
        ILogger*                logger;
        const char*             fileName;
        SearchResult<GridCell>  sr;
};