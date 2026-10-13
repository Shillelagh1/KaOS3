#include "glob.h"

#include <stddef.h>

#include "Devices/IO/Serial/Serial.h"
#include "Devices/PIC/PIC.h"
#include "Devices/PIT/PIT.h"
#include "Devices/PIC/IRQ.h"

#include "GDT/GDT.h"
#include "IDT/IDT.h"
#include "Services/VGAText/VGAText.h"
#include "Services/Multiboot/multiboot.h"
#include "Services/Paging/Paging.h"
#include "Peripherals/Keyboard/PS2Kbd.h"
#include "Services/Timers/Timers.h"

#define KAOS_SPLASH "+================================+\n|                                |\n| ##\\  ##\\#####\\ ######\\#######\\ |\n| ##| ##/##/==####/===####/====/ |\n| #####/ #########|   #########\\ |\n| ##/=##\\##/==####|   ##\\====##| |\n| ##|  ####|  ##\\######/#######| |\n| \\=/  \\=\\=/  \\=/\\=====/\\======/ |\n|                                |\n+================================+\n"
#define KAOS_VERSION "Version X.X.X"

// Linker symbols
extern char _ROMBEGIN;
extern char _ROMEND;
extern char _CODEBEGIN;
extern char _CODEEND;

extern void boot_c_setup(multiboot_info_t* mbd){
    SetupGDT();
    SetupIDT();
    PIC_remap(PIC0_OFFSET, PIC1_OFFSET);

    // Here is where we should probably granularize the page directory.
    // TODO: We do need to setup non 4 MiB entries.

    SetupSerialPort(COM_ADDR[0]);
    Serial_SendString(COM_ADDR[0], "===> SERCOM0 <===");

    // Enable interrupts
    __asm__ volatile ("sti;");

    // Perform memory enumeration.
    // The memory structures are all providing addresses in their physical addresses,
    // so we must add HH_OFFSET to them to convert them to virtual
    multiboot_info_t* hh_mbd = (multiboot_info_t*)((int)mbd + HH_OFFSET);
    unsigned long int mem_count = 0;
    if (hh_mbd->flags & 1 << 6){
        for(unsigned int i = 0; i < hh_mbd->mmap_length; 
            i += sizeof(multiboot_memory_map_t)) 
        {
            multiboot_memory_map_t* mmmt = 
                (multiboot_memory_map_t*) (hh_mbd->mmap_addr + i + HH_OFFSET);

            if(mmmt->type == 1) {
                mem_count += (mmmt->len);
            }
        }
    }
    else{
        mem_count = hh_mbd->mem_upper - hh_mbd->mem_lower;
    }

    // Create the splash screen
    VGAT_color(VGA_RED, VGA_BLACK);
    VGAT_fill(' ');
    VGAT_glyph(22, 1,  KAOS_SPLASH);
    VGAT_write(30, 11, KAOS_VERSION);

    // Memory info
    VGAT_write(1,13,"|-Available: ");
    VGAT_print(itoa_dec(mem_count / (1024*1024)));
    VGAT_print(" MiB");

    VGAT_write(1,14,"|-Memory Used: ");
    VGAT_print(itoa_dec(&_ROMEND - &_ROMBEGIN));
    VGAT_print(" B");

    VGAT_write(1,15,"|-Kernel Code Size: ");
    VGAT_print(itoa_dec(&_CODEEND - &_CODEBEGIN));
    VGAT_print(" B");

    // Serial info
    /*
    for(int i = 0; i < 8; i++){
        VGAT_write(57, 1+i, "COM");
        VGAT_print(itoa_dec(i)); VGAT_print(": ");
        switch (CheckSerialPort(COM_ADDR[i])){
            case 0:
                VGAT_print("OK");
                break;
            case 1:
                VGAT_print("NO (1)");
                break;
            case 2:
                VGAT_print("BAD (2)");
                break;
        }
    }*/

    // Setup timers (heartbeat etc.)
    SetupTimers();
    EnableTimers();
    SetTimersDbgSmiley(320);

    SetupPS2Kbd();
    VGAT_write(0,0, " ");
}