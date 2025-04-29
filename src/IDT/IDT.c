#include "IDT.h"
#include "int_errh.h"

#define ASSIGN_ISR(__n) CreateISRStruct(__n, &isr_stub_##__n)

static struct IDT idt;

static struct Intdesc interrupt_table[IDT_NUM_VECTORS];

static void CreateISRStruct(int n, void* f){
    interrupt_table[n].offset0 = (int)f & 0xFFFF;
    interrupt_table[n].offset1 = (int)f >> 16;
    interrupt_table[n].ss = ISR_DEFAULT_SS;
    interrupt_table[n].gate_type = ISR_DEFAULT_GATE_TYPE;
    interrupt_table[n].privelage = ISR_DEFAULT_PRIVELAGE;
    interrupt_table[n].present = 1;
}

extern void SetupIDT(){
    ASSIGN_ISR(0);
    ASSIGN_ISR(1);
    ASSIGN_ISR(2);
    ASSIGN_ISR(3);
    ASSIGN_ISR(4);
    ASSIGN_ISR(5);
    ASSIGN_ISR(6);
    ASSIGN_ISR(7);
    ASSIGN_ISR(8);
    ASSIGN_ISR(9);
    ASSIGN_ISR(10);
    ASSIGN_ISR(11);
    ASSIGN_ISR(12);
    ASSIGN_ISR(13);
    ASSIGN_ISR(14);
    ASSIGN_ISR(15);
    ASSIGN_ISR(16);
    ASSIGN_ISR(17);
    ASSIGN_ISR(18);
    ASSIGN_ISR(19);
    ASSIGN_ISR(20);
    ASSIGN_ISR(21);

    idt.size = (IDT_NUM_VECTORS * 8) - 1;
    idt.offset = interrupt_table;

    _setIDTR(&idt);
}