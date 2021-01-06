#ifndef MISSION_H
#define	MISSION_H

#include "map.h"
#include "config.h"
#include "search.h"
#include "ilogger.h"
#include "searchresult.h"
#include "environmentoptions.h"
#include "search.h"
#include "xmllogger.h"

//That's the wrap up class that first creates all the needed objects (Map, Search etc.)
//and then runs the search and then cleans everything up.

//Hint: Create Mission object in the main() function and then use it 1) to retreive all the data from input XML
//2) run the search 3) flush the results to output XML

#ifndef SEARCHALGORITHM_API
#define SEARCHALGORITHM_API
#endif

class SEARCHALGORITHM_API Mission
{
    public:
        Mission();
        Mission (const char* fileName);
        ~Mission();

        bool getMap();
        bool getConfig();
        bool createLog();
        void createSearch();
        void createEnvironmentOptions();
        void startSearch();
        void printSearchResultsToConsole();
        void saveSearchResultsToLog();
        SearchResult getSearchResult();
        
    private:
        Map                     map;
        Config                  config;
        EnvironmentOptions      options;
        Search                  search;
        ILogger*                logger;
        const char*             fileName;
        SearchResult            sr;
};

#endif

