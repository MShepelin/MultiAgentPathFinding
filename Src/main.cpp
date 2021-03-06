#include "nodes.h"
#include "mission.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "A map must be chosen!" << std::endl;
        return 0;
    }

    Mission mission(argv[1]);

    std::cout<<argv[1]<<std::endl;
    std::cout<<"Parsing the map from XML:"<<std::endl;

    if(!mission.prepareMap()) {
        std::cout<<"Incorrect map! Program halted!"<<std::endl;
    }
    else {
        std::cout<<"Map OK!"<<std::endl<<"Parsing configurations (algorithm, log) from XML:"<<std::endl;
        if(!mission.getConfig())
            std::cout<<"Incorrect configurations! Program halted!"<<std::endl;
        else {
            std::cout<<"Configurations OK!"<<std::endl<<"Creating log channel:"<<std::endl;

            if(!mission.createLog())
                std::cout<<"Log chanel has not been created! Program halted!"<<std::endl;
            else {
                std::cout<<"Log OK!"<<std::endl<<"Start searching the path:"<<std::endl;

                mission.startSearch();

                std::cout<<"Search is finished!"<<std::endl;

                mission.printSearchResultsToConsole();
                mission.saveSearchResultsToLog();

                std::cout<<"Results are saved (if chosen) via created log channel."<<std::endl;
            }
        }
    }
    
    return 0;
}
