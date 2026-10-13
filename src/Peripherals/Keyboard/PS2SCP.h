#include "Keyevent.h"

#define RESK 0x0
#define NULK 0x0

char LUT_LOWER_CHAR[256]={
    0x1B,
    [0x20]='`','1','2','3','4','5','6','7','8','9','0','-','=',0x08,
    [0x41]='q','w','e','r','t','y','u','i','o','p','[',']','\\',
    [0x61]='a','s','d','f','g','h','j','k','l',';','\'',0x0D,
    [0x81]='z','x','c','v','b','n','m',',','.','/',
    [0xA3]=' '
};

char LUT_UPPER_CHAR[256]={
    0x1B,
    [0x20]='~','!','@','#','$','%','^','&','*','(',')','_','+',0x08,
    [0x41]='Q','W','E','R','T','Y','U','I','O','P','{','}','|',
    [0x61]='A','S','D','F','G','H','J','K','L',':','\"',0x0D,
    [0x81]='Z','X','C','V','B','N','M','<','>','?',
    [0xA3]=' '
};

enum KEYEVENT_KEY LUT_KEKEY[256]={
    KEY_ESCAPE, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_PRINTSCREEN,[32]=
    KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_9, KEY_0, KEY_MINUS, KEY_EQUALS, KEY_BACKSPACE, KEY_INSERT, KEY_HOME, KEY_PAGEUP, [64]=
    KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_LEFTSQUAREBRACKET, KEY_RIGHTSQUAREBRACKET, KEY_BACKSLASH, KEY_DELETE, KEY_END, KEY_PAGEDOWN, [96]=
    KEY_CAPSLOCK, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_SINGLEQUOTE, KEY_ENTER, [128]=
    KEY_LEFTSHIFT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_FORWARDSLASH, KEY_RIGHTSHIFT, KEY_UP, [160]=
    KEY_LEFTCTRL, KEY_LEFTALT, KEY_SPACE, KEY_RIGHTALT, KEY_RIGHTCTRL
};

unsigned char LUT_DEF_KEYPOS[256]={
    0x76, 0x05, 0x06, 0x04, 0x0C, 0x03, 0x0B, 0x83, 0x0A, 0x01, 0x09, 0x78, 0x07, RESK, 0x7E, RESK, [32]=
    0x0E, 0x16, 0x1E, 0x26, 0x25, 0x2E, 0x36, 0x3D, 0x3E, 0x46, 0x45, 0x4E, 0x55, 0x66, RESK, RESK, RESK, 0x77, RESK, RESK, RESK, [64]=
    0x0D, 0x15, 0x1D, 0x24, 0x2D, 0x2C, 0x35, 0x3C, 0x43, 0x44, 0x4D, 0x54, 0x5B, 0x5D, RESK, RESK, RESK, RESK, RESK, RESK, RESK, [96]=
    0x58, 0x1C, 0x1B, 0x23, 0x2B, 0x34, 0x33, 0x3B, 0x42, 0x4B, 0x4C, 0x52, 0x5A, RESK, RESK, RESK, [128]=
    0x12, 0x1A, 0x22, 0x21, 0x2A, 0x32, 0x31, 0x3A, 0x41, 0x49, 0x4A, 0x59, RESK, RESK, RESK, RESK, RESK, [160]=
    0x14, RESK, 0x11, 0x29, RESK, RESK, RESK, RESK, NULK, NULK, NULK, RESK, RESK, RESK
};

unsigned char LUT_ALT_KEYPOS[256]={
    [0x2E]= 0x70, 0x6C,
    [0x4E]= 0x71, 0x69,
    [0x8C]= 0x75,
    [0xA4]= 0x11,
    [0xA7]= 0x14,
    [0xAB]= 0x6B, 0x72, 0x74
};