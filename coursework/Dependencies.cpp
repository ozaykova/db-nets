#include "Dependencies.h"

    void Dependencies::getCausalDependencies() {
        for (auto a : allDependencies) {
            if (a.second != 0 || allDependencies[{a.first.second, a.first.first}] != 0) {
                if (a.second - allDependencies[{a.first.second, a.first.first}] >
                    (a.second + allDependencies[{a.first.second, a.first.first}]) / 4) {
                    causalDependencies.push_back({ a.first, 0 });
                }
            }
        }
    }

    void Dependencies::getDependentTraces(std::map<int, std::vector<std::string>>& traces) {
        for (auto& trace: traces) {
            for (size_t j = 1; j < trace.second.size(); ++j) {
                allDependencies[{trace.second[j - 1], trace.second[j]}]++;
            }
        }
    }
