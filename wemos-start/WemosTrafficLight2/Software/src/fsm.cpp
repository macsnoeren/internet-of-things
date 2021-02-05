#include "fsm.h"
#include <Arduino.h>

FSM::FSM(int totalStates, int totalEvents, int debug) : totalStates(totalStates), totalEvents(totalEvents), debug(debug) {
    this->states.resize(totalStates);
}

void FSM::addTransition(int state, int event, int newState) {
    if ( state < this->totalStates && event < this->totalEvents ) { 
        this->transitions[state][event] = newState;
    }
}

void FSM::addState(int s, std::function<void(void)> pre, std::function<void(void)> loop, std::function<void(void)> post) {
    if ( s < this->totalStates ) {
        Serial.println("Added state: " + String(s));
        this->states[s].pre = pre;
        this->states[s].loop = loop;
        this->states[s].post = post;
    }
}

void FSM::raiseEvent(int e) {
    if ( this->currentState != -1 &&  e < this->totalEvents ) {
        Serial.println("finding event: " + String(currentState) + " => " + String(e));
        if ( this->transitions[currentState].find(e) != this->transitions[currentState].end() ) { // Check if event exists for the current state!
            int newState = this->transitions[this->currentState][e]; // get the new state from the transition map
            Serial.println("found new state: " + String(newState));

            this->states[this->currentState].post(); // Leaving current state
            this->states[newState].pre();            // Entering new state

            this->currentState = newState;
        }
    }
}

void FSM::setup(int state, int eventStateExecuted) {
    if ( this->currentState == -1 || state < this->totalStates ) { // Only set start state if not set yet!
        this->currentState = state;
        this->eventStateExecuted = eventStateExecuted;
        this->states[state].pre(); // Call the pre function!
    }
}

void FSM::loop () {
    Serial.println("loop: " + String(this->currentState));
    if ( this->currentState != -1 &&  this->currentState < this->totalEvents ) {
        this->states[this->currentState].loop();
        this->raiseEvent(this->eventStateExecuted);

    } else {
        Serial.println("Could not execute loop!");
    }
}
