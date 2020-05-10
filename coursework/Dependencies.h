#pragma once

#include<map>
#include<set>
#include <string>
#include <vector>
#include<unordered_map>
#include "Dependencies.h"

class Dependencies {
public:
    std::map <std::pair<std::string, std::string>, int> allDependencies;
    std::vector<std::pair<std::pair<std::string, std::string>, bool> > causalDependencies;

    void getCausalDependencies();
    void getDependentTraces(std::map<int, std::vector<std::string>>& traces);
};