#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>

#include "Dependencies.h"

class Transition;

class Place {
public:
    std::vector<Transition*> kids;
    std::vector<Transition*> parents;
    std::string name;

    Place(std::string s): name(s) {}
};

class Transition {
public:
    std::vector<Place*> kids;
    std::vector<Place*> parents;
    std::string name;

    Transition(std::string s): name(s) {}
};


class PetriNet {
public:
    std::vector<Place*> places;
    std::vector<Transition*> transitions;

    void prepareInitialData(std::map<int, std::vector<std::string>>& traces);
    void minePetriNet();
    int greedyILPSolver();
    void prepareInitialPlaceTransitions(Place* A, std::pair<std::pair<std::string, std::string>, bool>& i);

    std::map<std::string, int> FullState;
    std::map<std::string, int> StateWithoutLast;
    std::vector<std::string> Begining;
    std::vector<std::string> Ending;

    Dependencies depend;

    std::vector<std::string> transitions_name;

    std::set<std::string> allTransitions;

    void dotSerializer();
};