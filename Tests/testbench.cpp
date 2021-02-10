#include "mission.h"
#include "tinyxml2.h"
#include <cassert>
#include <gtest/gtest.h>

#define EPS 1e-5

bool getRealResults(const char* fileName, float* realLen);


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cerr<<"Error! Pathfinding task file (XML) is not specified!"<<std::endl;
        return 0;
    }
    float len;
    if(!getRealResults(argv[1], &len))
    {
        return -1;
    }

    Mission mission(argv[1]);

    if(!mission.getMap())
    {
        std::cerr<<"Incorrect map! Program halted!"<<std::endl;
        return -1;
    }

    if(!mission.getConfig())
    {
        std::cerr<<"Incorrect configurations! Program halted!"<<std::endl;
        return -1;
    }

    if(!mission.createLog())
    {
        std::cerr<<"Log chanel has not been created! Program halted!"<<std::endl;
        return -1;
    }

    mission.createEnvironmentOptions();
    mission.createSearch();
    mission.startSearch();
    SearchResult result = mission.getSearchResult();

    
    if(abs(len - result.pathlength) > EPS)
    {
        std::cerr<<"Incorrect path legth"<<std::endl;
        return 1;
    }

    return 0;
}

bool getRealResults(const char* fileName, float* realLen)
{
    tinyxml2::XMLDocument doc;

    float len = 0.0f;

    // Load XML File
    if (doc.LoadFile(fileName) != tinyxml2::XMLError::XML_SUCCESS) 
    {
        std::cerr << "Error opening XML file!" << std::endl;
        return false;
    }

    tinyxml2::XMLElement *root = doc.FirstChildElement(CNS_TAG_ROOT);
    if (!root) 
    {
        std::cerr << "Error! No '" << CNS_TAG_ROOT << "' tag found in XML file!" << std::endl;
        return false;
    }

    tinyxml2::XMLElement *log = root->FirstChildElement(CNS_TAG_LOG);
    if (!log) 
    {
        std::cerr << "Error! No '" << CNS_TAG_LOG << "' tag found in XML file!" << std::endl;
        return false;
    }

    tinyxml2::XMLElement *sum = log->FirstChildElement(CNS_TAG_SUM);
    if (!sum) 
    {
        std::cerr << "Error! No '" << CNS_TAG_SUM << "' tag found in XML file!" << std::endl;
        return false;
    }

    if(sum->QueryFloatAttribute(CNS_TAG_ATTR_LENGTH, &len) != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Error! No '" << CNS_TAG_ATTR_LENGTH<< "' tag found in XML file!" << std::endl;
        return false;
    }
    
    *realLen = len;

    return true;
}