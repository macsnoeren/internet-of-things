#include "fsmtrafficllight2.h"
#include <Arduino.h>


FSMTrafficLight2::FSMTrafficLight2() : FSM(STATES_TOTAL, EVENTS_TOTAL) {
    this->addState(STATE_START, FSMTrafficLight2::preStart, FSMTrafficLight2::preStart, FSMTrafficLight2::preStart);
 /*   this->addState(STATE_LIGHT_1_GREEN,  new StateLight1Green());
    this->addState(STATE_LIGTH_1_ORANGE, new StateLight1Orange());
    this->addState(STATE_LIGTH_1_RED,    new StateLight1Red());
    this->addState(STATE_LIGHT_2_GREEN,  new StateLight2Green());
    this->addState(STATE_LIGTH_2_ORANGE, new StateLight2Orange());
    this->addState(STATE_LIGTH_2_RED,    new StateLight2Red());
    this->addState(STATE_OUT_OF_ORDER,   new StateLight2Red());*/
}

FSMTrafficLight2::~FSMTrafficLight2 () {

}

void FSMTrafficLight2::preStart() {
    Serial.println("HOI");
    
}


void FSMTrafficLight2::loopStartloop() {

}

void FSMTrafficLight2::postStart() {

}

void FSMTrafficLight2::preLight1Green() {

}

void FSMTrafficLight2::loopLight1Green() {

}

void FSMTrafficLight2::postLight1Green() {

}

void FSMTrafficLight2::preLight1Orange() {

}

void FSMTrafficLight2::loopLight1Orange() {

}

void FSMTrafficLight2::postLight1Orange() {

}

void FSMTrafficLight2::preLight1Red() {
    
}

void FSMTrafficLight2::loopLight1Red() {

}

void FSMTrafficLight2::postLight1Red() {

}

void FSMTrafficLight2::preLight2Green() {

}

void FSMTrafficLight2::loopLight2Green() {

}

void FSMTrafficLight2::postLight2Green() {

}

void FSMTrafficLight2::preLight2Orange() {

}

void FSMTrafficLight2::loopLight2Orange() {

}

void FSMTrafficLight2::postLight2Orange() {

}

void FSMTrafficLight2::preLight2Red() {

}

void FSMTrafficLight2::loopLight2Red() {

}

void FSMTrafficLight2::postLight2Red() {

}

void FSMTrafficLight2::preOutOfOrder() {

}

void FSMTrafficLight2::loopOutOfOrder() {

}

void FSMTrafficLight2::postOutOfOrder() {

}
