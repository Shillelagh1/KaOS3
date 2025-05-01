/* TODO -- Implement some kind of ISR stub table to make this far more 
 modular. This current implementation is fine for errors but we will struggle
 with anything else.
*/

#include "IDT.h"
#include "int_errh.h"

#define ASSIGN_ERR_ISR(__n) IDT_AssignISR(__n, &isr_stub_##__n)

static struct IDT idt;

static struct Intdesc interrupt_table[IDT_NUM_VECTORS];

static void IDT_AssignISR(int n, void* f){
    interrupt_table[n].offset0 = (int)f & 0xFFFF;
    interrupt_table[n].offset1 = (int)f >> 16;
    interrupt_table[n].ss = ISR_DEFAULT_SS;
    interrupt_table[n].gate_type = ISR_DEFAULT_GATE_TYPE;
    interrupt_table[n].privelage = ISR_DEFAULT_PRIVELAGE;
    interrupt_table[n].present = 1;
}

extern void SetupIDT(){
    ASSIGN_ERR_ISR(0);
    ASSIGN_ERR_ISR(1);
    ASSIGN_ERR_ISR(2);
    ASSIGN_ERR_ISR(3);
    ASSIGN_ERR_ISR(4);
    ASSIGN_ERR_ISR(5);
    ASSIGN_ERR_ISR(6);
    ASSIGN_ERR_ISR(7);
    ASSIGN_ERR_ISR(8);
    ASSIGN_ERR_ISR(9);
    ASSIGN_ERR_ISR(10);
    ASSIGN_ERR_ISR(11);
    ASSIGN_ERR_ISR(12);
    ASSIGN_ERR_ISR(13);
    ASSIGN_ERR_ISR(14);
    ASSIGN_ERR_ISR(15);
    ASSIGN_ERR_ISR(16);
    ASSIGN_ERR_ISR(17);
    ASSIGN_ERR_ISR(18);
    ASSIGN_ERR_ISR(19);
    ASSIGN_ERR_ISR(20);
    ASSIGN_ERR_ISR(21);

    idt.size = (IDT_NUM_VECTORS * 8) - 1;
    idt.offset = interrupt_table;

    _setIDTR(&idt);
}