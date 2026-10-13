#pragma once

#define PS2Kbd_ERR0 0x00
#define PS2Kbd_Selftest_OK 0xAA
#define PS2Kbd_Echo 0xEE
#define PS2Kbd_Ack 0xFA
#define PS2Kbd_Selftest_BAD0 0xFC
#define PS2Kbd_Selftest_BAD1 0xFD
#define PS2Kbd_Resend 0xFE
#define PS2Kbd_ERR1 0xFF

#define PS2Kbd_Cmd_Scanset 0xF0

#define PS2_Data 0x60
#define PS2_Cmd 0x64
#define PS2_Status 0x64

#define PS2Kbd_Default_Scanset 2

#define KEYEVENT_PRESS 0
#define KEYEVENT_RELEASE 1

// ScanCode Parser state machine
enum SCP_State{
    s_SCP_DEF,
    s_SCP_ALT
};

void SetupPS2Kbd();
