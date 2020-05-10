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

void LogParser::getLog() {
    std::ifstream fin;
    fin.open("log.txt", std::ios::in);
    std::string s;
    getline(fin, s);
    int count = stoi(s);
    for (size_t i = 0; i < count; ++i) {
        Log log;
        getline(fin, s);
        std::cout << s << std::endl;
        log.sessionId = stoi(s);
        getline(fin, s);
        log.actionType = s;
        getline(fin, s);
        log.tableName = s;
        getline(fin, s);
        if (s != "null") {
            std::istringstream is(s);
            is >> s;
            int row = stoi(s.substr(0, s.size() - 1));
            std::vector<std::string> oldVal, newVal;
            getData(oldVal, is);
            getData(newVal, is);
            log.diff = Diff(row, oldVal, newVal);
        }
        journal.push_back(log);
    }
}
