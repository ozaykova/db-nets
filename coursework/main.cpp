#include <iostream>
#include "LogParser.h"
#include "DBNet.h"

int main() {
    DBNet net;
    net.getScheme();
    LogParser parser;
    parser.getLog();
    return 0;
}