#pragma once

#include <map>
#include <vector>

class FSMState {
    public:
        FSMState();
        void pre();
        void loop();
        void post();
};

class FSM {

  private:
    std::map<int, std::map<int, int>> transitions; // map[state][event]
    std::vector<FSMState*> states;                  // vector of states

    int totalStates;
    int totalEvents;
    int currentState;

  public:
    FSM(int totalStates, int totalEvents);
    void addTransition(int state, int event, int nextState);
    void addState(int s, FSMState *pState);
    void raiseEvent(int e);
    void setup(int state);
    void loop();
};

