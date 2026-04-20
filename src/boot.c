#include "Services/VGAText/VGAText.h"

extern char _ROMBEGIN;
extern char _ROMEND;
extern char _CODEBEGIN;
extern char _CODEEND;
extern char _stack_top;

extern void boot_c_setup(){
    VGAT_write(0, 0, "|| KAOS PANIC ||");
}