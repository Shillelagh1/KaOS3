#include <stdint.h>
#include <stddef.h>

#include "PS2Kbd.h"
#include "PS2SCP.h"
#include "Keyevent.h"

#include "../../glob.h"
#include "../../Devices/PIC/IRQ.h"
#include "../../Services/VGAText/VGAText.h"
#include "../../Devices/PIC/PIC.h"
#include "../../Devices/IO/Serial/Serial.h"
#include "../../IDT/IDT.h"
#include "../../Devices/IO/IOPort/IOPort.h"
#include "../../Services/Timers/Timers.h"

#define KBD_FIFO_Size 64
#define KBD_Refresh_S (float)0.005

extern void _isr_ps2kbd();

struct CTimer *ps2kbd_refresh_timer;

unsigned char scancode_fifo[KBD_FIFO_Size];
int scancode_fifo_count = 0;
enum SCP_State SCP_cstate;  // State of the decoder statemachine
int SCP_isrelease;          // Release/Press state of the decoder

// bit 0 = pressed
unsigned char keymap[256];
int isShifted = 0;

// Interrupt service routine child called by IRQ1 (ps2 keyboard)
void c_isr_ps2kbd(){
    unsigned char scancode = inb(PS2_Data);

    // TODO: Handle FIFO overflow case

    // Add the scancode to a FIFO buffer
    scancode_fifo[scancode_fifo_count] = scancode;
    scancode_fifo_count++;
    PIC_sendEOI(IRQ_KB);
}

void DbgPrintPos(unsigned char pos){
    if (SCP_isrelease)
        Serial_SendString(COM0, "\n- ");
    else
        Serial_SendString(COM0, "\n+ ");

    Serial_SendString(COM0, "0x");
    Serial_SendString(COM0, itoa(pos, 16));
    Serial_SendString(COM0, ": ");
    Serial_SendString(COM0, itoa_dec(pos & 0b00011111));
    Serial_SendString(COM0, ", ");
    Serial_SendString(COM0, itoa_dec(pos >> 5));
}

unsigned char SearchLUT(unsigned char key, unsigned char* LUT, int max){
    for (int i = 0; i < max; i++){
        if (LUT[i] == key){
            return (unsigned char)i;
        }
    }

    return (unsigned char)0xFF;
}

struct Keyevent DecodeKeyEvent(unsigned char position, int release){
    struct Keyevent event;
    event.keyRelease = release & 1;
    event.keyPosition = position;
    event.key = LUT_KEKEY[position];

    if (isShifted) event.keyChar = LUT_UPPER_CHAR[position];
    else event.keyChar = LUT_LOWER_CHAR[position];

    return event;
}

void SendKeyEvent(struct Keyevent event){
    keymap[event.keyPosition] = !event.keyRelease;
    if (event.key == KEY_LEFTSHIFT) isShifted = !event.keyRelease;

    if (event.keyRelease) return;
    char stringtoprint[]={event.keyChar, 0};
    VGAT_print(stringtoprint);
}

void ProcessFIFOCodebuffer(){
   /* Read the scancodes out of the FIFO one at a time, and
    * process them. A state machine should be involved to set the states
    * for 0xf0 break codes or 0xe0 function codes for instance
    */

    unsigned char code;
    unsigned char pos;
    struct Keyevent event;

    for (int i = 0; i < scancode_fifo_count; i++){
        code = scancode_fifo[i];
        switch(SCP_cstate){
            case s_SCP_DEF:
                if (code == 0xE0){  // Alternate Keycodes
                    SCP_cstate = s_SCP_ALT;
                }
                else if (code == 0xF0){ // Release Keycodes
                    SCP_isrelease = 1;
                } 
                else {
                    // +Character
                    pos = SearchLUT(code, LUT_DEF_KEYPOS, 256);
                    event = DecodeKeyEvent(pos, SCP_isrelease);
                    SendKeyEvent(event);

                    SCP_isrelease = 0;
                }
            break; 
            case s_SCP_ALT:
                // +Character
                SCP_cstate = s_SCP_DEF;
                pos = SearchLUT(code, LUT_ALT_KEYPOS, 256);
                event = DecodeKeyEvent(pos, SCP_isrelease);
                SendKeyEvent(event);

                SCP_isrelease = 0;
            break;
        }
    }
    scancode_fifo_count = 0;
}

// Timer callback to read keyboard changes
void t_ps2KbdRefresh(){
    ProcessFIFOCodebuffer();
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

    SCP_cstate = s_SCP_DEF;

    for(int i = 0; i < 256; i++){
        keymap[i] = 0;
    }

    PIC_unmask(IRQ_KB);
}