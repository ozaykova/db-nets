#include <fstream>
#include <iostream>
#include "PetriNet.h"

void PetriNet::prepareInitialPlaceTransitions(Place* A, std::pair<std::pair<std::string, std::string>, bool>& i) {
    for (auto a : transitions) {
        if (a->name == i.first.first) {
            A->parents.push_back(a);
            a->kids.push_back(A);
        }

        if (a->name == i.first.second) {
            A->kids.push_back(a);
            a->parents.push_back(A);
        }
    }
}

int PetriNet::greedyILPSolver() {
    int chk = 1;

    for (auto i : depend.causalDependencies) {
        if (i.second != 1) {
            int countILP = 0;
            countILP = StateWithoutLast[i.first.first] - FullState[i.first.second];
            Place* A = new Place(std::to_string(chk));
            chk++;

            prepareInitialPlaceTransitions(A, i);
            int countILP_last = 0;

            while (countILP < 0 && countILP != countILP_last) {
                std::string addTransition;
                int cur_value = INT_MAX;
                std::pair<std::pair<std::string, std::string>, bool> copyy;
                for (auto& j : depend.causalDependencies) {
                    if (j.first.first != i.first.first && j.first.second == i.first.second) {
                        if (StateWithoutLast[j.first.first] < cur_value) {
                            cur_value = StateWithoutLast[j.first.first];
                            addTransition = j.first.first;
                            copyy = { j.first,  j.second };
                        }
                    }
                }


                if (addTransition != "") {
                    countILP_last = countILP;
                    countILP += cur_value;
                    for (auto& a : transitions) {
                        if (a->name == addTransition) {
                            A->parents.push_back(a);
                            a->kids.push_back(A);
                            auto rem = find(depend.causalDependencies.begin(), depend.causalDependencies.end(), copyy);
                            (*rem).second = 1;
                        }
                    }
                }
                else {
                    break;
                }
            }

            std::vector<std::string> visit_event;
            while (countILP >= 0) {
                std::string addTransition;
                int cur_value = INT_MAX;
                std::pair<std::pair<std::string, std::string>, bool> copyy;
                for (auto j : depend.causalDependencies) {
                    if (j.first.first == i.first.first && j.first.second != i.first.second && find(visit_event.begin(), visit_event.end(), j.first.second) == visit_event.end()) {
                        if (FullState[j.first.second] < cur_value) {
                            cur_value = FullState[j.first.second];
                            addTransition = j.first.second;
                            copyy = { j.first, j.second };
                            visit_event.push_back(j.first.second);
                        }
                    }
                }

                if (addTransition != "") {
                    if (countILP - cur_value >= 0) {
                        countILP -= cur_value;
                        for (auto a : transitions) {
                            if (a->name == addTransition) {
                                A->kids.push_back(a);
                                a->parents.push_back(A);
                                auto rem = find(depend.causalDependencies.begin(), depend.causalDependencies.end(), copyy);
                                (*rem).second = 1;
                            }
                        }
                    }
                }
                else {
                    break;
                }
            }
            places.push_back(A);
        }
    }
    return chk;
}

void PetriNet::minePetriNet() {
    for (auto i : FullState) {
        transitions_name.push_back(i.first);
        Transition* cur = new Transition(i.first);
        transitions.push_back(cur);
    }

    Place *A = new Place("0");
    for (auto i : Begining) {
        for (auto j : transitions) {
            if (j->name == i) {
                j->parents.push_back(A);
                A->kids.push_back(j);
            }
        }
    }
    places.push_back(A);

    int chk = greedyILPSolver();

    A = new Place(std::to_string(chk));
    for (auto i : Ending) {
        for (auto j : transitions) {
            if (i == j->name) {
                A->parents.push_back(j);
                j->kids.push_back(A);
            }
        }
    }
    dotSerializer();
}

void PetriNet::prepareInitialData(std::map<int, std::vector<std::string>>& traces) {
    for (auto& trace: traces) {
        for (size_t i = 0; i < trace.second.size(); ++i) {
            FullState[trace.second[i]]++;
            if (i == 0) {
                Begining.push_back(trace.second[i]);
            }
            if (i == trace.second.size() - 1) {
                Ending.push_back(trace.second[i]);
                StateWithoutLast[trace.second[i]]++;
            }
            allTransitions.insert(trace.second[i]);
        }
    }

    depend.getDependentTraces(traces);
    depend.getCausalDependencies();
    
    minePetriNet();
}

void PetriNet::dotSerializer() {
    std::ofstream fout;
    fout.open("D:output.txt");

    std::set<std::string> s;
    fout << "Digraph {" << std::endl;
    fout << "node [shape = box];";
    for (auto k : transitions) {
        fout << "\"" << k->name << "\"" << ";";
    }
    fout << std::endl;
    fout << "node [shape = circle];" << std::endl;
    for (auto& a : places) {
        for (auto b : a->kids) {
            s.insert("\"" + a->name + "\"" + "->" + "\"" + b->name + "\"");
            for (auto c : b->kids) {
                s.insert("\"" + b->name + "\"" + "->" + "\"" + c->name + "\"");
            }
        }
    }
    for (auto& a : s) {
        fout << a << std::endl;
    }
    fout << "}" << std::endl;
    fout.flush();
    fout.close();
}