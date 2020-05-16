#include <fstream>
#include <optional>
#include <sstream>
#include <iostream>
#include "LogParser.h"

void LogParser::getData(std::vector<std::string>& vec, std::istringstream& is) {
    char c;
    while(is.get(c) && c != '{');

    std::string curr = "";
    while (is.get(c) && c != '}') {
        if (c != ',') {
            curr += c;
        } else {
            vec.push_back(curr);
            curr.clear();
        }
    }
    vec.push_back(curr);
    curr.clear();
}

void LogParser::getLog(std::string& logPath) {
    std::ifstream fin;
    fin.open(logPath, std::ios::in);
    if (!fin.is_open()) {
        std::cout << "Wrong log path. Check --help.";
        exit(0);
    }

    std::string s;
    getline(fin, s);
    int count = stoi(s);
    for (size_t i = 0; i < count; ++i) {
        Log log;
        getline(fin, s);
        log.sessionId = stoi(s);
        getline(fin, s);
        log.actionType = s;
        getline(fin, s);
        log.tableName = s;
        getline(fin, s);
        if (s != "null") {
            std::istringstream is(s);
            std::vector<std::string> oldVal, newVal;
            getData(oldVal, is);
            getData(newVal, is);
            log.diff = Diff(oldVal, newVal);
        }
        journal.push_back(log);
    }
}
