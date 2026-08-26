#include <stdint.h>
#include <stddef.h>

#include "PS2Kbd.h"

#include "../../glob.h"
#include "../../Devices/PIC/IRQ.h"
#include "../../Services/VGAText/VGAText.h"
#include "../../Devices/PIC/PIC.h"
#include "../../Devices/IO/Serial/Serial.h"
#include "../../IDT/IDT.h"
#include "../../Devices/IO/IOPort/IOPort.h"
#include "../../Services/Timers/Timers.h"

#define KBD_Refresh_S (float)2

extern void _isr_ps2kbd();

struct CTimer *ps2kbd_refresh_timer;

int irq_count = 0;
int irq_count_checked = 0;

// Interrupt service routine child called by IRQ1 (ps2 keyboard)
void c_isr_ps2kbd(){
    irq_count++;
    VGAT_write(0,0, itoa_dec(irq_count));

    unsigned int code = inb(PS2_Data);

    VGAT_write(0,2, "Scancode: 0x");
    VGAT_print(itoa(code, 16));
    Serial_SendString(COM0, itoa(code, 16));
    PIC_sendEOI(IRQ_KB);
}

// Timer callback to read keyboard changes
void t_ps2KbdRefresh(){
    irq_count_checked = irq_count;
    VGAT_write(4,0, "[");
    VGAT_print(itoa_dec(irq_count_checked));
    VGAT_print("]");
}

// PS2 Controller Related
// Wait for the PS2 controller to be ready to recieve a command
void PS2WaitNotBusy(){
    unsigned int status;
    while(1){
        status = inb(PS2_Status);
        if(!(status & 0b10)){
            return;
        }
    }
}

// Wait for the PS2 controller to have an output ready.
void PS2WaitResponse(){
    unsigned int status;
    while(1){
        status = inb(PS2_Status);
        if(status & 0b1){
            return;
        }
    } 
}

// Initialize the PS2 Keyboard subsystem.
void SetupPS2Kbd(){
    // IRQ1 assignment
    IDT_AssignISR(PIC0_OFFSET + IRQ_KB, _isr_ps2kbd);

    // Disable scancode translation
    // TODO: remove magic
    PS2WaitNotBusy(); outb(PS2_Cmd, 0x20); PS2WaitResponse(); // Read config byte
    unsigned int config = inb(PS2_Data);
    config &= 0b10111111;
    PS2WaitNotBusy(); outb(PS2_Cmd, 0x60); PS2WaitNotBusy();
    outb(PS2_Data, config);

    // Set keyboard to use scancode set 2
    unsigned volatile int resp; // TODO: Remove after done debugging
    outb(PS2_Data, PS2Kbd_Cmd_Scanset);
    resp = inb(PS2_Data);
    outb(PS2_Data, PS2Kbd_Default_Scanset);
    resp = inb(PS2_Data);

    // Keyboard refresh timer...    
    struct CTimerSpec spec = {
        .timerType = CTimerSpec_Type_Repeat,
        .reset = SecondsToCycles(KBD_Refresh_S),
        .dividerSel = 0,
        .timerFunc = t_ps2KbdRefresh,
        .destroyFunc = NULL,
        .loggingName = "PS2Kbd Rf"
    };
    CreateTimer(spec, 1);

    PIC_unmask(IRQ_KB);
}