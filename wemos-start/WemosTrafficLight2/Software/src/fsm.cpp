#include <fsm.h>

FSM::FSM(int totalStates, int totalEvents) {
    this->totalStates  = totalStates;
    this->totalEvents  = totalEvents;
    this->currentState = -1;
}

void FSM::addTransition(int state, int event, int newState) {
    if ( state < this->totalStates && event < this->totalEvents ) { 
        this->transitions[state][event] = newState;
    }
}

void FSM::addState(int s, FSMState *pState) {
    if ( s < this->totalStates ) {
        this->states[s] = pState;
    }
}

void FSM::raiseEvent(int e) {
    if ( this->currentState != -1 &&  e < this->totalEvents ) {
        if ( this->transitions[currentState].find( e ) != this->transitions[currentState].end() ) { // Check if event exists for the current state!
            int newState = this->transitions[this->currentState][e]; // get the new state from the transition map

            this->states[this->currentState]->post(); // Leaving current state
            this->states[newState]->pre();            // Entering new state

            this->currentState = newState;
        }
    }
}

void FSM::setup(int state) {
    if ( this->currentState == -1 && state < this->totalStates ) { // Only set start state if not set yet!
        this->currentState = state;
    }
}

void FSM::loop () {
    if ( this->currentState != -1 &&  this->currentState < this->totalEvents ) {
        this->states[this->currentState]->loop();
    }
}

