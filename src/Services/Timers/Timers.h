#pragma once

#define CTimers_MaxTimers 1024

static volatile long int CTimer_globCount; // Mainly for debugging purposes, avoid using this for timing.

void SetupTimers();             // Setup the timers SUBSYTEM
void EnableTimers();            // Enable the timers SUBSYSTEM
void DisableTimers();           // Disable the timers SUBSYSTEM
void SetTimersDbgSmiley(int);   // Set the reset for the timers debug strober. 0 to disable
int SecondsToCycles(float);
float CyclesToSeconds(int);

// Enum describing a CTimer
enum CTimerSpec_Type{
    CTimerSpec_Type_Oneshot,    // Runs once and then is destroyed
    CTimerSpec_Type_Repeat      // Runs until disabled
};

struct CTimerSpec{
    enum CTimerSpec_Type timerType; // What type of timer is this (see above)
    int reset;                      // The timer is initialized to this value and counts down.
    int dividerSel;                    // Max 32
    //  Dividersel controls the clock divider this timer belongs to. A sel of 0 decrements the count
    // every tick, a sel of 1 runs every other, 2 every 4 ticks, and so on.
    void* timerFunc;                // What function is called when the timer completes?
    void* destroyFunc;              // Function called when the timer is destroyed.
    char* loggingName;
};

struct CTimer{
    struct CTimerSpec timerSpec;
    unsigned int exists : 1;
    unsigned int enabled : 1;
    unsigned int counter;
} __attribute__((packed));

void SetTimerActive(struct CTimer*, int);
void DestroyTimer(struct CTimer*);
struct CTimer* CreateTimer(struct CTimerSpec spec, int enableInstantly);