#include "Devices/IO/Serial/Serial.h"
#include "Devices/PIC/PIC.h"
#include "Devices/PIC/IRQ.h"
#include "Devices/PIT/PIT.h"

#include "GDT/GDT.h"
#include "IDT/IDT.h"
#include "Services/VGAText/VGAText.h"
#include "Services/Multiboot/multiboot.h"
#include "Services/Paging/Paging.h"

#define KAOS_SPLASH "+================================+\n|                                |\n| ##\\  ##\\#####\\ ######\\#######\\ |\n| ##| ##/##/==####/===####/====/ |\n| #####/ #########|   #########\\ |\n| ##/=##\\##/==####|   ##\\====##| |\n| ##|  ####|  ##\\######/#######| |\n| \\=/  \\=\\=/  \\=/\\=====/\\======/ |\n|                                |\n+================================+\n"
#define KAOS_VERSION "Version 0.1.0"

#define PIC0_OFFSET 0x20
#define KERNEL_PAGE_COUNT 2
#define FIRST_KERNEL_PAGE (uint32_t)(PAGE_DIR_LEN - KERNEL_PAGE_COUNT)

#define VIRTUAL_HIGHERHALF_OFFSET FIRST_KERNEL_PAGE * PAGE_WIDTH

extern void _isr_heartbeat();
extern void _enter_higher_half(uint32_t offset);

extern char _ROMBEGIN;
extern char _ROMEND;
extern char _CODEBEGIN;
extern char _CODEEND;
extern char _stack_top;

uint8_t COM_STATUS[8];

CREATE_PAGEDIR(kernel);

void heartbeat(){
    // Serial_SendString(COM0, "heartbeat\n");
    PIC_sendEOI(0);
}

extern void boot_c_setup(multiboot_info_t* mbd){
    // Setup tables and interrupts
    SetupGDT();
    SetupIDT();
    PIC_remap(PIC0_OFFSET, PIC0_OFFSET + 8);

    // Setup kernel paging
    PG_ZeroizePagedir(page_dir_kernel);
    for(int i = 0; i < KERNEL_PAGE_COUNT; i++){
        PG_ConfigurePagedirEntry(&page_dir_kernel[i], PDE_PRESENT | PDE_RW, 0, i);  // Identity
        PG_ConfigurePagedirEntry(&page_dir_kernel[(PAGE_DIR_LEN - KERNEL_PAGE_COUNT) + i], PDE_PRESENT | PDE_US, 0, i);  // Higher Half
    }
    PG_EnablePaging(page_dir_kernel);
    _enter_higher_half(VIRTUAL_HIGHERHALF_OFFSET);

    // Destroy the identity mapping (Move fully to higher half)
    for(int i = 0; i < KERNEL_PAGE_COUNT; i++){
        // PG_ConfigurePagedirEntry(&page_dir_kernel[i], 0, 0, 0);  // Identity
    }

    // Do hardware configuraiton
    #pragma region 
    SetupSerialPort(COM0);
    Serial_SendString(COM0, "==== SERCOM0 ====\n");

    // Setup interval timer and heartbeat function
    SetupPIT(0, PIT_MD_RATE);
    PIT_SetReload(0, 65535);
    IDT_AssignISR(PIC0_OFFSET + IRQ_PIT, &_isr_heartbeat);

    #pragma endregion

    __asm__ volatile ("sti;");
    PIC_unmask(IRQ_PIT);

    // Perform hardware evaluation
    #pragma region 
    // Perform memory enumeration.
    Serial_SendString(COM0, "Enumerating Memory...\n");
    unsigned long int mem_count = 0;
    if (mbd->flags & 1 << 6){
        for(unsigned int i = 0; i < mbd->mmap_length; 
            i += sizeof(multiboot_memory_map_t)) 
        {
            multiboot_memory_map_t* mmmt = 
                (multiboot_memory_map_t*) (mbd->mmap_addr + i);

            if(mmmt->type == 1) {
                mem_count += (mmmt->len);
                Serial_SendString(COM0, "0x");
                Serial_SendString(COM0, itoa(mmmt->addr_low, 16));
                Serial_SendString(COM0, ": ");
                Serial_SendString(COM0, itoa_dec(mmmt->len / 1024));
                Serial_SendString(COM0, " kB \n");
            }
        }
    }
    else{
        mem_count = mbd->mem_upper - mbd->mem_lower;
    }

    // Perform COM port checks.
    for (int i = 0; i < 8; i++){
        COM_STATUS[i] = CheckSerialPort(COM_ADDR[i]);
        SetupSerialPort(COM_ADDR[i]);
    }

    #pragma endregion

    // Create the splash screen
    #pragma region 
    VGAT_color(VGA_BLACK, VGA_RED);
    VGAT_fill(' ');
    VGAT_glyph(22,1,KAOS_SPLASH);

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

    int stack_ptr;
    __asm__ volatile ("mov %%esp, %0" : "=g"(stack_ptr) : : );
    VGAT_write(1,16,"|-Stack Used in Init: ");
    VGAT_print(itoa_dec((int)&_stack_top - stack_ptr));
    VGAT_print(" B");

    // Show COM port status
    for(int i = 0; i < 8; i++){
        VGAT_color(VGA_BLACK, VGA_RED);
        VGAT_write(1, 2 + i,"|-COM");
        VGAT_print(itoa_dec(i));
        switch(COM_STATUS[i]){
            case 0:
                VGAT_print(": OK"); break;
            case 1:
                VGAT_print(": "); break;
            case 2:
                VGAT_print(": FAIL"); break;
        }
    }
    
    #pragma endregion
}