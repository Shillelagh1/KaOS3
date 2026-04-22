#include "Services/VGAText/VGAText.h"

extern char _ROMBEGIN;
extern char _ROMEND;
extern char _CODEBEGIN;
extern char _CODEEND;
extern char _stack_top;

extern void boot_c_setup(){
    VGAT_color(VGA_BLACK, VGA_BLUE);
    VGAT_fill(' ');
    VGAT_write(1, 1,  "> Boostrapper done!");
}