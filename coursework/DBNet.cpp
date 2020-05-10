#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
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

void DBNet::generateDataLogicLayer(std::string& action, std::string& table,
        std::vector<std::string>& attrs, std::vector<std::string>& scheme = *(new std::vector<std::string>())) {
    std::stringstream ss;
    if (action == "insert") {
        ss << "INSERT INTO " << table << "(";
        for (size_t i = 0; i < attrs.size(); ++i) {
            if (i == attrs.size() - 1) {
                ss << attrs[i] << ");";
            } else {
                ss << attrs[i] << ", ";
            }
        }
    }

    if (action == "delete") {
        ss << "DELETE FROM " << table << " WHERE ";
        for (size_t i = 0; i < attrs.size(); ++i) {
            if (i == attrs.size() - 1) {
                ss << attrs[i] << " == val;";
            } else {
                ss << attrs[i] << " == val AND ";
            }
        }
    }

    if (action == "update") {
        ss << "UPDATE " << table << " ";
        for (size_t i = 0; i < attrs.size(); ++i) {
            if (i == attrs.size() - 1) {
                ss << attrs[i] << " == val ";
            } else {
                ss << attrs[i] << " == val, ";
            }
        }
        ss << "WHERE ";
        for (size_t i = 0; i < scheme.size(); ++i) {
            if (i == scheme.size() - 1) {
                ss << scheme[i] << " == val; ";
            } else {
                ss << scheme[i] << " == val AND ";
            }
        }
    }

    dataLogicLayer.insert(ss.str());
}

std::string DBNet::getFinalDiff(Log& event){
    if (event.actionType == "insert") {
        std::string result = "insert: ";
        if (persistentLayer.find(event.tableName) != persistentLayer.end()) {
            for (auto& columnName: persistentLayer[event.tableName]) {
                result += columnName + ", ";
            }
            generateDataLogicLayer(event.actionType, event.tableName, persistentLayer[event.tableName]);
            smartDiff[result.substr(0, result.size() - 2)] = persistentLayer[event.tableName];
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

                std::vector<std::string> tmp;
                for (size_t i = 0; i < diff.oldVal.size(); ++i) {
                    if (diff.oldVal[i] != diff.newVal[i]) {
                        result += persistentLayer[event.tableName][i] + ", ";
                        tmp.push_back(persistentLayer[event.tableName][i]);
                    }
                }
                generateDataLogicLayer(event.actionType, event.tableName, tmp, persistentLayer[event.tableName]);
                smartDiff[result.substr(0, result.size() - 2)] = tmp;
                return result;
            }
        } else {
            std::cout << "Table " << event.tableName << " is not exist in scheme";
            exit(0);
        }
    }

    if (event.actionType == "delete") {
        generateDataLogicLayer(event.actionType, event.tableName, persistentLayer[event.tableName]);
        smartDiff["delete"] = persistentLayer[event.tableName];
    }
    return event.actionType + "  ";
}

void DBNet::saveDataLogicLayer() {
    std::ofstream fout;
    fout.open("DataLogicLayer.txt");

    for (auto& a: dataLogicLayer) {
        fout << a << std::endl;
    }

    fout.close();
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

void DBNet::getPlacesAttributes() {
    std::queue<Place*> q;
    std::set<std::string> visited;

    q.push(controlLayer->places[0]);

    while (!q.empty()) {
        auto& cur = q.front();
        if (visited.find(cur->name) != visited.end()) {
            q.pop();
            continue;
        }

        for (auto& transition: cur->kids) {
            for (auto& place: transition->kids) {
                for (auto& attr: placesAttributes[cur->name]) {
                    placesAttributes[place->name].insert(attr);
                }

                if (transition->name.find("insert") != std::string::npos
                    || transition->name.find("update") != std::string::npos) {
                    for (auto& attr: smartDiff[transition->name]) {
                        placesAttributes[place->name].insert(attr);
                    }
                }

                if (transition->name.find("delete") != std::string::npos) {
                    for (auto& attr: smartDiff[transition->name]) {
                        placesAttributes[place->name].erase(attr);
                    }
                }

                q.push(place);
            }
        }

        visited.insert(cur->name);
        q.pop();
    }

    for (auto& attrs: placesAttributes) {
        std::cout << attrs.first << ": ";
        for (auto& attr: attrs.second) {
            std::cout << attr << "; ";
        }
        std::cout << std::endl;
    }
}