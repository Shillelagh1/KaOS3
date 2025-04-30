#include "Devices/IO/Serial/Serial.h"
#include "GDT/GDT.h"
#include "IDT/IDT.h"

extern void boot_c_setup(){
    SetupGDT();
    SetupIDT();
    SetupSerialPort(0x3F8);
}