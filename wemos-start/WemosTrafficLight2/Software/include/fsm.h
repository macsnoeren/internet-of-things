#pragma once

#include <map>
#include <vector>
#include <functional>

/* Define FSM transition structure (advanced using pre and post methods) */
struct FSMStateMethods {
   std::function<void(void)> pre;  // When the state is active, this method is called once at start
   std::function<void(void)> loop; // When the state is active, this method is continously called
   std::function<void(void)> post; // When the state is deactivated, this method is called once
};

class FSM {
  private:
    std::map<int, std::map<int, int>> transitions; // map[state][event]
    std::vector<FSMStateMethods> states;           // vector of state methods

    int totalStates;
    int totalEvents;
    int currentState;

  public:
    FSM(int totalStates, int totalEvents);
    void addTransition(int state, int event, int nextState);
    void addState(int s, void (*pre)(void), void (*loop)(void), void (*post)(void));
    void raiseEvent(int e);
    void setup(int state);
    void loop();
};

