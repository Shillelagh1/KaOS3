#include "Serial.h"
#include "../IOPort/IOPort.h"

void Serial_SendString(uint16_t port, char* str){
    for(int i = 0; 1; i++){
        if(str[i] == 0){
            return;
        }
        outb(port + SER_REG_TX, str[i]);
    }
}

int SetupSerialPort(uint16_t port){

    outb(port + SER_REG_SCRATCPAD, SER_TESTNUM);
    if(inb(port + SER_REG_SCRATCPAD) != SER_TESTNUM){
        return -1;  // Scratchpad fail.
    }

    // Set divisor
    int divisor = SER_DIVISOR;
    outb(port + SER_REG_LINECTL, inb(port + SER_REG_LINECTL) | SER_DLAB_SET);   // Set DLAB
    outb(port + SER_REG_DLAB_DIVISOR_LSB, divisor & 0xFF);
    outb(port + SER_REG_DLAB_DIVISOR_MSB, divisor >> 8);
    outb(port + SER_REG_LINECTL, inb(port + SER_REG_LINECTL) & SER_DLAB_CLR);   // Clear DLAB

    // Setup line control
    uint8_t linectl = 0;
    linectl |= SER_DATA_BITS_8 & 0x3;
    linectl |= (SER_STOP_BITS & 1) << 2;
    linectl |= (SER_PARITY_MD & 7) << 3;
    outb(port + SER_REG_LINECTL, linectl);

    // Unset all interrupts
    outb(port + SER_REG_INTERRUPT_EN, 0);

    // Loopback test
    outb(port + SER_REG_MODEMCTL, SER_MODEM_REQUESTTOSEND | SER_MODEM_OUT | SER_MODEM_LOOP);
    outb(port + SER_REG_TX, SER_TESTNUM);
    if(inb(port + SER_REG_RX) != SER_TESTNUM){
        return -2; // Loopback fail.
    }

    // Set port to normal operatoin
    outb(port + SER_REG_MODEMCTL, SER_MODEM_DATAREADY | SER_MODEM_REQUESTTOSEND | SER_MODEM_OUT);

    return 0;
}