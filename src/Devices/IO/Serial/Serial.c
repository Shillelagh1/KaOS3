#include <stdint.h>

#include "Serial.h"
#include "../IOPort/IOPort.h"

static void sp_good(){
    __asm__ volatile ("cli; hlt");
}

static void sp_bad(){
    __asm__ volatile ("cli; hlt");
}

extern int SetupSerialPort(uint16_t port){
    outb(port + SER_REG_SCRATCPAD, SER_SCRATCHPAD_TESTNUM);
    if(inb(port + SER_REG_SCRATCPAD) == SER_SCRATCHPAD_TESTNUM){
        sp_good();
    }
    sp_bad();
    return 0;
}