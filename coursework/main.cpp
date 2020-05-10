#include <iostream>
#include "LogParser.h"
#include "DBNet.h"

int main() {
    DBNet net;
    std::cout << "Start reading scheme" << std::endl;
    net.getScheme();
    std::cout << "Scheme reading done" << std::endl;
    LogParser parser;
    std::cout << "Start getting log" << std::endl;
    parser.getLog();
    std::cout << "Getting log done" << std::endl;
    net.getTraces(parser.journal);
    std::cout << "Traces has been got" << std::endl;
    net.showTraces();
    net.controlLayer = new PetriNet;
    std::cout << "Start mine control layer" << std::endl;
    net.controlLayer->prepareInitialData(net.traces);
    net.saveDataLogicLayer();
    net.getPlacesAttributes();
    return 0;
}