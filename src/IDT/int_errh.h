#include <stdint.h>

#define ISR_STUB_NO_ERR(__n) void isr_stub_##__n(){ \
    __asm__ volatile ("cli; hlt"); \
}

#define ISR_STUB_ERR(__n) void isr_stub_##__n(uint32_t n){ \
    __asm__ volatile ("cli; hlt"); \
}

void isr_stub_0();
void isr_stub_1();
void isr_stub_2();
void isr_stub_3();
void isr_stub_4();
void isr_stub_5();
void isr_stub_6();
void isr_stub_7();
void isr_stub_8(uint32_t n);
void isr_stub_9();
void isr_stub_10(uint32_t n);
void isr_stub_11(uint32_t n);
void isr_stub_12(uint32_t n);
void isr_stub_13(uint32_t n);
void isr_stub_14(uint32_t n);
void isr_stub_15();
void isr_stub_16();
void isr_stub_17(uint32_t n);
void isr_stub_18();
void isr_stub_19();
void isr_stub_20();
void isr_stub_21(uint32_t n);