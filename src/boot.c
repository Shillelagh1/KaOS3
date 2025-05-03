#include "Devices/IO/Serial/Serial.h"
#include "Devices/PIC/PIC.h"
#include "sysdefine.h"
#include "GDT/GDT.h"
#include "IDT/IDT.h"
#include "MIT/Multitask.h"

extern void boot_c_setup(){
    SetupGDT();
    SetupIDT();
    PIC_remap(PIC_OFFSET, PIC_OFFSET + 8);
    
    SetupSerialPort(SERCOM0);
    Serial_SendString(SERCOM0, "==== KaOS SERCOM0 ====\n");

    __asm__ volatile ("sti;");

    MIT_Init();
}