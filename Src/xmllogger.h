#pragma once

#include "tinyxml2.h"
#include "ilogger.h"
#include "nodes.h"

class XmlLogger : public ILogger {

public:
    XmlLogger(std::string loglevel):ILogger(loglevel){}

    virtual ~XmlLogger() {};

    bool getLog(const char *FileName, const std::string *LogParams);

    void saveLog();

    void writeToLogMap(const XMLMap &Map, const std::vector<Node<GridCell>> &path);

    //void writeToLogOpenClose(const typename &open, const typename &close);

    void writeToLogPath(const std::vector<Node<GridCell>> &path);

    void writeToLogHPpath(const std::vector<Node<GridCell>> &hppath);

    void writeToLogNotFound();

    void writeToLogSummary(unsigned int numberofsteps, unsigned int nodescreated, float length, double time, double cellSize);

private:
    std::string LogFileName;
    tinyxml2::XMLDocument doc;
};
