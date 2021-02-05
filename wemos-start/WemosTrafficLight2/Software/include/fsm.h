// Author: Maurice Snoeren

#pragma once

#include <map>
#include <vector>
#include <functional>

#include <Arduino.h>

/* Define FSM transition structure (advanced using pre and post methods) */
struct FSMMethods {
   std::function<void(void)> pre;  // When the state is active, this method is called once at start
   std::function<void(void)> loop; // When the state is active, this method is continously called
   std::function<void(void)> post; // When the state is deactivated, this method is called once
};

class FSM {
    private:
        std::map<int, std::map<int, int>> transitions; // map[state][event]
        std::vector<FSMMethods> states;                // vector of state methods

        int totalStates;
        int totalEvents;
        int currentState;
        int eventStateExecuted;
        bool debugEnabled;
        unsigned long loopTiming;
        unsigned long currentLoopTime;

    public:
        FSM(int totalStates, int totalEvents, bool debugEnabled = false);
        void addTransition(int state, int event, int nextState);
        void addState(int s, std::function<void(void)> pre, std::function<void(void)> loop, std::function<void(void)> post);
        void raiseEvent(int e);
        void setup(int state, int eventStateExecuted);
        void loop();
        void debug(String text);
        unsigned long getLoopTime();
};