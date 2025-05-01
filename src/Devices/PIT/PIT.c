#include "PIT.h"
#include "../IO/IOPort/IOPort.h"

void SetupPIT(int PIT, uint8_t mode){
    outb(PIT_CMD, mode | PIT_ACC_LOHI | ((PIT & 3) << 6));
}

void PIT_SetReload(int PIT, uint16_t reload){
    uint16_t port = 0;
    switch(PIT){
        default:
        case 0:
            port = PIT0_DATA;
            break;
        case 1:
            port = PIT1_DATA;
            break;
        case 2:
            port = PIT2_DATA;
            break;
    }

    outb(port, reload & 0xFF);
    outb(port, reload >> 8);
}