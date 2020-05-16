#include <iostream>
#include "LogParser.h"
#include "DBNet.h"

struct Files {
    std::string scheme;
    std::string log;
};

void helpPrint() {
    std::cout << "--input-scheme - pass scheme file, for example, --input-scheme scheme.txt. Default is scheme.txt\n";
    std::cout << "--input-log - pass log file, for example, --input-log log.txt. Default is log.txt\n";
}

Files initFlags(int argc, char **argv) {
    if (argc > 5) {
        std::cout << "Wrong arguments number. Check --help:\n\n";
        helpPrint();
        exit(0);
    }

    std::string help = "--help";
    std::string log = "log.txt", scheme = "scheme.txt";
    std::string schemeFlag = "--input-scheme";
    std::string logFlag = "--input-log";

    if (argc > 1 && argv[1] == help) {
        helpPrint();
        exit(0);
    }


    for (int i = 1; i < argc; i += 2) {
        if (argv[i] == schemeFlag) {
            if (i + 1 >= argc) {
                std::cout << "Flag without argument. Check --help:\n\n";
                helpPrint();
                exit(0);
            }

            scheme = argv[i + 1];
        } else if (argv[i] == logFlag) {
            if (i + 1 >= argc) {
                std::cout << "Flag without argument. Check --help:\n\n";
                helpPrint();
                exit(0);
            }

            log = argv[i + 1];
        } else {
            std::cout << "Wrong arguments number. Check --help:\n\n";
            helpPrint();
            exit(0);
        }
    }

    return Files{scheme, log};
}

int main(int argc, char **argv) {
    Files init = initFlags(argc, argv);

    DBNet net;
    std::cout << "Start reading scheme" << std::endl;
    net.getScheme(init.scheme);
    std::cout << "Scheme reading done" << std::endl;
    LogParser parser;
    std::cout << "Start getting log" << std::endl;
    parser.getLog(init.log);
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