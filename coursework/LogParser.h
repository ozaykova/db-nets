#include <optional>
#include <string>
#include <vector>

class Diff {
public:
    int row;
    std::vector<std::string> oldVal;
    std::vector<std::string> newVal;

    Diff(int id, std::vector<std::string> oldVal, std::vector<std::string> newVal)
        : row(id)
        , oldVal(std::move(oldVal))
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
    void getLog();
    void getData(std::vector<std::string>& vec, std::istringstream& is);
    std::vector<Log> journal;
};