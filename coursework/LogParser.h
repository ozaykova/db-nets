#pragma once

#include <optional>
#include <string>
#include <vector>

class Diff {
public:
    std::vector<std::string> oldVal;
    std::vector<std::string> newVal;

    Diff(std::vector<std::string> oldVal, std::vector<std::string> newVal)
        : oldVal(std::move(oldVal))
        , newVal(std::move(newVal))
    {}
};

struct Log {
    int sessionId;
    std::string actionType;
    std::string tableName;
    std::optional<Diff> diff;
};


class LogParser {
public:
    void getLog(std::string& logPath);
    void getData(std::vector<std::string>& vec, std::istringstream& is);
    std::vector<Log> journal;
};