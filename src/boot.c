#include "Devices/IO/Serial/Serial.h"
#include "Devices/PIC/PIC.h"
#include "Devices/PIT/PIT.h"
#include "GDT/GDT.h"
#include "IDT/IDT.h"

extern void _isr_heartbeat();

void heartbeat(){
    Serial_SendString(0x3F8, "heartbeat\n");
    PIC_sendEOI(0);
}

extern void boot_c_setup(){
    SetupGDT();
    SetupIDT();
    PIC_remap(0x20, 0x28);
    
    SetupSerialPort(0x3F8);
    Serial_SendString(0x3F8, "== SERIAL 0 ==\n");

    // Setup interval timer and heartbeat function
    SetupPIT(0, PIT_MD_RATE);
    PIT_SetReload(0, 65535);
    IDT_AssignISR(0x20, &_isr_heartbeat);

    __asm__ volatile ("sti;");

    PIC_unmask(0);
}