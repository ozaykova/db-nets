#pragma once

#include <map>
#include <vector>
#include <unordered_map>
#include "PetriNet.h"
#include "LogParser.h"

class DBNet {
public:
    std::unordered_map<std::string, std::vector<std::string>> persistentLayer;
    std::set<std::string> dataLogicLayer;
    PetriNet* controlLayer;

    void getScheme(std::string& schemePath);
    void getTraces(std::vector<Log>& journal);
    void showTraces();
    void generateDataLogicLayer(std::string& action, std::string& table,
            std::vector<std::string>& attrs, std::vector<std::string>& scheme);

    void saveDataLogicLayer();

    std::map<int, std::vector<std::string>> traces;

    std::map<std::string, std::set<std::string> > placesAttributes;

    void getPlacesAttributes();

private:
    std::string getFinalDiff(Log& event);
    std::unordered_map<std::string, std::vector<std::string>> smartDiff;
    void tracesProcessor(Log& event);
    std::string isTable(std::string& s);
};
