#pragma once

#include <vector>
#include <string>

class PetriNet {
public:
    class Transition;

    class Place {
    public:
        std::vector<Transition*> kids;
        std::vector<Transition*> parents;
        std::string name;

        Place(std::string s) {
            name = s;
        }
    };

    class Transition {
    public:
        std::vector<Place*> kids;
        std::vector<Place*> parents;
        std::string name;

        Transition(std::string s) {
            name = s;
        }
    };

    std::vector<Place*> places;
    std::vector<Transition*> transitions;
};
