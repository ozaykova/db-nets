#pragma once

#include <vector>
#include "PetriNet.h"
#include "Scheme.h"

class DBNet {
public:
    std::vector<Scheme*> persistentLayer;
    std::vector<std::string> dataLogicLayer;
    PetriNet* controlLayer;

    void getScheme();
};
