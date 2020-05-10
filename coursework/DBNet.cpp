#include <fstream>
#include <sstream>
#include <iostream>
#include "DBNet.h"

void DBNet::getScheme() {
    std::ifstream fin;
    fin.open("scheme.txt", std::ios::binary);

    std::string name;

    while (getline(fin, name)) {
        std::vector<std::string> attrs;

        std::string line;
        getline(fin,line);
        std::istringstream is(line);
        std::string s;
        while(is >> s) {
            attrs.push_back(s);
        }

        persistentLayer[name] = attrs;
    }
    fin.close();
}

std::string DBNet::getFinalDiff(Log& event){
    if (event.actionType == "insert") {
        std::string result = "insert: ";
        if (persistentLayer.find(event.tableName) != persistentLayer.end()) {
            for (auto& columnName: persistentLayer[event.tableName]) {
                result += columnName + ", ";
            }
            return result;
        } else {
            std::cout << "Table " << event.tableName << " is not exist in scheme";
            exit(0);
        }
    }

    if (event.actionType == "update") {
        std::string result = "update: ";
        if (persistentLayer.find(event.tableName) != persistentLayer.end()) {
            if (!event.diff.has_value()) {
                std::cout << "Empty diff in update operation";
                exit(0);
            } else {
                auto diff = event.diff.value();
                if (diff.oldVal.size() != diff.newVal.size()
                    || diff.oldVal.size() != persistentLayer[event.tableName].size()) {
                    std::cout << "Update operation returned another set of attributes";
                    exit(0);
                }

                for (size_t i = 0; i < diff.oldVal.size(); ++i) {
                    if (diff.oldVal[i] != diff.newVal[i]) {
                        result += persistentLayer[event.tableName][i] + ", ";
                    }
                }
                return result;
            }
        } else {
            std::cout << "Table " << event.tableName << " is not exist in scheme";
            exit(0);
        }
    }
    return event.actionType + "  ";
}

void DBNet::getTraces(std::vector<Log>& journal) {
    for (auto& event: journal) {
        std::string diff = getFinalDiff(event);
        traces[event.sessionId].push_back(diff.substr(0, diff.size() - 2));
    }
}

void DBNet::showTraces() {
    for (auto& trace: traces) {
        std::cout << "Session id: " << trace.first << std::endl;
        for (auto& event: trace.second) {
            std::cout << event << ";";
        }
        std::cout << std::endl;
    }
}