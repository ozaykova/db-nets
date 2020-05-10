#pragma once

#include <map>
#include <vector>
#include <unordered_map>
#include "PetriNet.h"
#include "LogParser.h"

class DBNet {
public:
    std::unordered_map<std::string, std::vector<std::string>> persistentLayer;
    std::vector<std::string> dataLogicLayer;
    PetriNet* controlLayer;

    void getScheme();
    void getTraces(std::vector<Log>& journal);
    void showTraces();

    std::map<int, std::vector<std::string>> traces;

private:
    std::string getFinalDiff(Log& event);
};
