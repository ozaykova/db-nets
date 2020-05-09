#include <fstream>
#include <sstream>
#include <iostream>
#include "DBNet.h"

void DBNet::getScheme() {
    std::ifstream fin;
    fin.open("scheme.txt", std::ios::binary);

    std::string name;

    while (getline(fin, name)) {
        std::cout << "TABLE NAME: " << name << std::endl;
        std::vector<std::string> attrs;

        std::string line;
        getline(fin,line);
        std::istringstream is(line);
        std::string s;
        while(is >> s) {
            attrs.push_back(s);
            std::cout << s << " ";
        }
        std::cout << std::endl;

        persistentLayer.push_back(new Scheme(std::move(name), std::move(attrs)));
    }
    fin.close();
}