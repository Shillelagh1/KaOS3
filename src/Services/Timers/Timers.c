#include <stddef.h>

#include "../VGAText/VGAText.h"
#include "../../Devices/PIC/PIC.h"
#include "../../Devices/PIC/IRQ.h"
#include "../../Devices/PIT/PIT.h"
#include "../../IDT/IDT.h"
#include "../../glob.h"

#include "Timers.h"

#define PIT_Reload 2048
#define PIT_freq_Hz 1193180 // TODO: Find a real value here
#define HB_Freq ((float)PIT_freq_Hz / (float)PIT_Reload)

int divider_L = 0;
int divider_C = 0;

static struct CTimer ctimer_Array[CTimers_MaxTimers];

int TestDivider(int divider){
    if (divider == 0){
        return 1;
    }

    int mask = 1 << (divider - 1);
    int edge = divider_C & ~divider_L;
    return edge & mask;
}

void StepOneTimer(struct CTimer *ctimer){
    if(ctimer->exists && ctimer->enabled){
        if (TestDivider(ctimer->timerSpec.dividerSel)){
            ctimer->counter -= 1;
            if (ctimer->counter < 1){
                // NULL check and call ctimer->timerSpec.timerFunct
                if(ctimer->timerSpec.timerFunc != NULL) ((void(*)())(ctimer->timerSpec.timerFunc))();

                switch(ctimer->timerSpec.timerType){
                    case CTimerSpec_Type_Oneshot:
                        DestroyTimer(ctimer);
                        break;
                    case CTimerSpec_Type_Repeat:
                        ctimer->counter = ctimer->timerSpec.reset;
                        break;
                }
            }
        }
    }
}

void CTimersStep(){
    // Step dividers
    divider_L = divider_C;
    divider_C = divider_C + 1;

    // Process clocks
    for(int i = 0; i < CTimers_MaxTimers; i++){
        StepOneTimer(&ctimer_Array[i]);
    }
}
extern void _isr_heartbeat();

int strober_max = 0;
int strober_count = 0;
void c_heartbeat(){
    CTimer_globCount++;

    // Manage the strober
    #pragma region 
    strober_count++;
    if(strober_count < (strober_max / 2) || !strober_max){
        VGAT_put(1, 1, ' ');
    } else {
        VGAT_put(1, 1, 0x02);
    }
    if(strober_count > strober_max){
        strober_count = 0;
    }
    #pragma endregion
    
    CTimersStep();

    PIC_sendEOI(0); // Tell the PIC to reset the IRQ line ... OR ELSE!
}

struct CTimer* GetEmptyTimer(){
    // Return first empty timer slot algorithm
    struct CTimer *itimer;
    for(int i = 0; i < CTimers_MaxTimers; i++){
        itimer = &ctimer_Array[i];
        if(!itimer->exists){
            return itimer;
        }
    }

    return NULL;
}

// TODO: Test for a full timers array case
struct CTimer* CreateTimer(struct CTimerSpec spec, int enableInstantly){
    struct CTimer* newTimer = GetEmptyTimer();
    if (newTimer == NULL){
        return NULL;
    }

    newTimer->timerSpec = spec;
    newTimer->exists = 1;
    newTimer->counter = spec.reset;
    newTimer->enabled = enableInstantly;

    return newTimer;
}

void DestroyTimer(struct CTimer* timer){
    timer->enabled = 0;
    timer->exists = 0;

    if(timer->timerSpec.destroyFunc != NULL) ((void(*)())(timer->timerSpec.destroyFunc))();
}

void SetTimerActive(struct CTimer* timer, int enabled){
    timer->enabled = enabled;
}

void SetupTimers(){
    // Setup heartbeat clock (PIT) & interrupt
    SetupPIT(0, PIT_MD_RATE);
    PIT_SetReload(0, PIT_Reload);
    IDT_AssignISR(PIC0_OFFSET + IRQ_PIT, &_isr_heartbeat);  // High level callback: c_heartbeat()

    // Initialize ctimer_Array to all unpopulated timers
    for(int i = 0; i < CTimers_MaxTimers; i++){
        struct CTimer *timer = &ctimer_Array[i];
        timer->exists = 0;
        timer->enabled = 0;
    }

    CTimer_globCount = 0;
}

void EnableTimers(){
    PIC_unmask(IRQ_PIT);
}

void DisableTimers(){
    PIC_mask(IRQ_PIT);
}

void SetTimersDbgSmiley(int enabled){
    strober_max = enabled;
}

int SecondsToCycles(float secs){
   return secs * (float)HB_Freq;
}

float CyclesToSeconds(int cycles){
    return (float)cycles / (float)HB_Freq;
}