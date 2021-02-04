#pragma once

#include <map>
#include <vector>
#include <functional>

/* Define FSM transition structure (advanced using pre and post methods) */
struct MauriceMethods {
   std::function<void(void)> pre;  // When the state is active, this method is called once at start
   std::function<void(void)> loop; // When the state is active, this method is continously called
   std::function<void(void)> post; // When the state is deactivated, this method is called once
};

class Maurice {
    private:
        std::map<int, std::map<int, int>> transitions; // map[state][event]
        std::vector<MauriceMethods> states;            // vector of state methods

        int totalStates;
        int totalEvents;
        int currentState;
        int eventStateExecuted;

    public:
        Maurice(int totalStates, int totalEvents);
        void test();
        void addTransition(int state, int event, int nextState);
        void addState(int s, std::function<void(void)> pre, std::function<void(void)> loop, std::function<void(void)> post);
        void raiseEvent(int e);
        void setup(int state, int eventStateExecuted);
        void loop();
};