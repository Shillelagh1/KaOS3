#include "Devices/IO/Serial/Serial.h"
#include "Devices/PIC/PIC.h"
#include "Devices/PIT/PIT.h"
#include "Services/VGAText/VGAText.h"
#include "GDT/GDT.h"
#include "IDT/IDT.h"
#include "Services/Multiboot/multiboot.h"

#define KAOS_SPLASH "+================================+\n|                                |\n| ##\\  ##\\#####\\ ######\\#######\\ |\n| ##| ##/##/==####/===####/====/ |\n| #####/ #########|   #########\\ |\n| ##/=##\\##/==####|   ##\\====##| |\n| ##|  ####|  ##\\######/#######| |\n| \\=/  \\=\\=/  \\=/\\=====/\\======/ |\n|                                |\n+================================+\n"
#define KAOS_VERSION "Version 0.1.0"

extern void _isr_heartbeat();

void heartbeat(){
    Serial_SendString(0x3F8, "heartbeat\n");
    PIC_sendEOI(0);
}

extern void boot_c_setup(multiboot_info_t* mbd){
    // Do tons of setup
    SetupGDT();
    SetupIDT();
    PIC_remap(0x20, 0x28);
    
    SetupSerialPort(0x3F8);

    // Setup interval timer and heartbeat function
    SetupPIT(0, PIT_MD_RATE);
    PIT_SetReload(0, 65535);
    IDT_AssignISR(0x20, &_isr_heartbeat);

    __asm__ volatile ("sti;");

    PIC_unmask(0);

    // Perform memory enumeration.
    unsigned long int mem_count = 0;
    if (mbd->flags & 1 << 6){
        for(unsigned int i = 0; i < mbd->mmap_length; 
            i += sizeof(multiboot_memory_map_t)) 
        {
            multiboot_memory_map_t* mmmt = 
                (multiboot_memory_map_t*) (mbd->mmap_addr + i);

            if(mmmt->type == 1) {
                mem_count += (mmmt->len);
            }
        }
    }
    else{
        mem_count = mbd->mem_upper - mbd->mem_lower;
    }

    // Create the splash screen
    // Splash
    VGAT_color(VGA_BLACK, VGA_RED);
    VGAT_fill(' ');
    VGAT_glyph(22,1,KAOS_SPLASH);

    VGAT_write(30, 11, KAOS_VERSION);

    // Memory info
    VGAT_write(1,13,"Memory: ");
    VGAT_print(itoa_dec(mem_count / (1024*1024)));
    VGAT_print(" MiB");
}