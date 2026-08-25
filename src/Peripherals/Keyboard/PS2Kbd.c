#include <stdint.h>
#include "../../glob.h"
#include "../../Devices/PIC/IRQ.h"
#include "../../Services/VGAText/VGAText.h"
#include "../../Devices/PIC/PIC.h"
#include "../../IDT/IDT.h"
#include "../../Devices/IO/IOPort/IOPort.h"

#define PS2_Data 0x60
#define PS2_Cmd 0x64
#define PS2_Status 0x64

extern void _isr_ps2kbd();

static int irq_count = 0;

void c_isr_ps2kbd(){
    irq_count++;
    VGAT_write(0,0, itoa_dec(irq_count));

    inb(0x60);
    PIC_sendEOI(IRQ_KB);
}

void SetupPS2Kbd(){
    IDT_AssignISR(PIC0_OFFSET + IRQ_KB, _isr_ps2kbd);
    PIC_unmask(IRQ_KB);
}