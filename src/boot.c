#include "Devices/IO/Serial/Serial.h"
#include "Devices/PIC/PIC.h"
#include "GDT/GDT.h"
#include "IDT/IDT.h"

extern void boot_c_setup(){
    SetupGDT();
    SetupIDT();
    PIC_remap(31,40);
    
    SetupSerialPort(0x3F8);
    Serial_SendString(0x3F8, "== SERIAL 0 ==\n");
}