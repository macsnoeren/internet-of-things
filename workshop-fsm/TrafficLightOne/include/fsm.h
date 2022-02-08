#pragma once

typedef enum {
    STATE_START,
    STATE_RED,
    STATE_ORANGE,
    STATE_GREEN,
    STATE_OUTOFORDER,
    STATE_INVALID_TRANSITION,
    STATE_NUM
} ENUM_STATE;

typedef enum {
    EVENT_BUTTON,
    EVENT_GREEN,
    EVENT_TIMER,
    EVENT_OUTOFORDER,
    EVENT_RED,
    EVENT_STATE_EXECUTED,
    EVENT_NUM
} ENUM_EVENT;

typedef struct {
    void (*pre)(void);
    void (*loop)(void);
    void (*post)(void);
} STATE_METHODS_STRUCT;

typedef struct {
    ENUM_STATE nextState;
} STATE_TRANSITION_STRUCT;

void FSMInit ();
void FSMLoop ();
void FSMRaiseEvent (ENUM_EVENT new_event);

// Forward declaration of the transition methods.
void start_pre();
void start_loop();
void start_post();

void red_pre();
void red_loop();
void red_post();

void orange_pre();
void orange_loop();
void orange_post();

void green_pre();
void green_loop();
void green_post();

void outoforder_pre();
void outoforder_loop();
void outoforder_post();


