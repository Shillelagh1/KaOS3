#include "glob.h"

#include "Devices/IO/Serial/Serial.h"
#include "Devices/PIC/PIC.h"
#include "Devices/PIT/PIT.h"
#include "Devices/PIC/IRQ.h"

#include "GDT/GDT.h"
#include "IDT/IDT.h"
#include "Services/VGAText/VGAText.h"
#include "Services/Multiboot/multiboot.h"
#include "Services/Paging/Paging.h"

#define KAOS_SPLASH "+================================+\n|                                |\n| ##\\  ##\\#####\\ ######\\#######\\ |\n| ##| ##/##/==####/===####/====/ |\n| #####/ #########|   #########\\ |\n| ##/=##\\##/==####|   ##\\====##| |\n| ##|  ####|  ##\\######/#######| |\n| \\=/  \\=\\=/  \\=/\\=====/\\======/ |\n|                                |\n+================================+\n"
#define KAOS_VERSION "Version X.X.X"

#define PIC0_OFFSET 0x20
#define PIC1_OFFSET 0x28

// Linker symbols
extern char _ROMBEGIN;
extern char _ROMEND;
extern char _CODEBEGIN;
extern char _CODEEND;

extern void _isr_heartbeat();

int toggle = 0;

void c_heartbeat(){
    toggle++;
    if(toggle > 10){
        VGAT_put(1, 1, ' ');
        VGAT_write(3, 1, "       ");
    } else {
        VGAT_put(1, 1, 0x02);
        VGAT_write(3, 1, "PIT OK!");
    }
    if(toggle > 20){
        toggle = 0;
    }
    PIC_sendEOI(0); // Tell the PIC to reset the IRQ line ... OR ELSE!
}

extern void boot_c_setup(multiboot_info_t* mbd){
    SetupGDT();
    SetupIDT();
    PIC_remap(PIC0_OFFSET, PIC1_OFFSET);

    // Here is where we should probably granularize the page directory.
    // TODO: We do need to setup non 4 MiB entries.

    SetupSerialPort(COM_ADDR[0]);
    Serial_SendString(COM_ADDR[0], "===> SERCOM0 <===");

    // Setup a PIT heartbeat clock
    SetupPIT(0, PIT_MD_RATE);
    PIT_SetReload(0, 65535);
    IDT_AssignISR(PIC0_OFFSET + IRQ_PIT, &_isr_heartbeat);

    // Enable interrupts
    __asm__ volatile ("sti;");

    PIC_unmask(IRQ_PIT); // Enable IRQ0 (PIT)

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
}