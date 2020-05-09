#include <vector>
#include <string>

class Scheme {
public:
    std::string tableName;
    std::vector<std::string> attributes;

    Scheme(std::string name, std::vector<std::string> attrs)
    : tableName(name)
    , attributes(attrs)
    {}
};
