#ifndef ILOGGER_H
#define	ILOGGER_H
#include "xml_map.h"
#include "nodes.h"
#include <unordered_map>
#include <list>

class ILogger
{
    public:
        ILogger(std::string loglevel) {this->loglevel = loglevel;}
        virtual bool getLog(const char* FileName, const std::string* LogParams) = 0;
        virtual void saveLog() = 0;
        virtual void writeToLogMap(const XMLMap& map, const std::vector<Node<GridCell>>& path) = 0;
        virtual void writeToLogPath(const std::vector<Node<GridCell>>& path) = 0;
        virtual void writeToLogHPpath(const std::vector<Node<GridCell>>& path) = 0;
        virtual void writeToLogNotFound() = 0;
        virtual void writeToLogSummary(unsigned int numberofsteps, unsigned int nodescreated, float length, double time, double cellSize) = 0;
        virtual ~ILogger() {};
    protected:
        std::string loglevel;
};

#endif

