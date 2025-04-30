#define IDT_NUM_VECTORS 22

#define ISR_DEFAULT_GATE_TYPE 0x0E
#define ISR_DEFAULT_SS 0x08
#define ISR_DEFAULT_PRIVELAGE 0

struct Intdesc{
    unsigned int offset0 : 16;
    unsigned int ss : 16;
    unsigned int res : 8;
    unsigned int gate_type : 4;
    unsigned int zero : 1;
    unsigned int privelage : 2;
    unsigned int present : 1;
    unsigned int offset1 : 16;
} __attribute__((packed));

struct IDT{
    unsigned int size : 16;
    struct Intdesc* offset;
} __attribute__((packed));

extern void SetupIDT();
extern void _setIDTR(struct IDT*);