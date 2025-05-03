#include "../Devices/PIT/PIT.h"
#include "../Devices/PIC/PIC.h"
#include "../IDT/IDT.h"
#include "Multitask.h"
#include "Task.h"
#include "../sysdefine.h"
#include "../Devices/IO/Serial/Serial.h"

extern void _isr_MIT_heartbeat();

void MIT_Init(){
    Serial_SendString(SERCOM0, "=> Initializing multitasking\n");

    IDT_AssignISR(PIC_OFFSET + IRQ_PIT, &_isr_MIT_heartbeat);

    SetupPIT(0, PIT_MD_RATE);
    PIT_SetReload(0, CLK_DIVIDE);

    PIC_unmask(IRQ_PIT);
}

void MIT_Multitasker(){

}

void MIT_heartbeat(){
    PIC_sendEOI(IRQ_PIT);
}