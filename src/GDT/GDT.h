#define CODE_ACCESS 0b10011111
#define DATA_ACCESS 0b10010011
#define TSS_ACCESS  0b10001001

#define GLOB_FLAGS 0b1100
#define GLOB_LIMIT0 65535
#define GLOB_LIMIT1 15
#define GLOB_BASE0 0
#define GLOB_BASE1 0

#define GDT_LEN 4
#define GDT_SIZE (GDT_LEN * 8) - 1

struct Segdesc{
    unsigned int limit0 : 16;
    unsigned int base0  : 24;
    unsigned int access : 8;
    unsigned int limit1 : 4;
    unsigned int flags  : 4;
    unsigned int base1  : 8;
} __attribute__((packed));

struct TSS{
    unsigned int link;
    unsigned int esp0;
    unsigned int ss0;
    unsigned int esp1;
    unsigned int ss1;
    unsigned int esp2;
    unsigned int ss2;
    unsigned int cr3;
    unsigned int eip;
    unsigned int eflgs;
    unsigned int eax;
    unsigned int ecx;
    unsigned int edx;
    unsigned int ebx;
    unsigned int esp;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
    unsigned int es;
    unsigned int cs;
    unsigned int ss;
    unsigned int ds;
    unsigned int fs;
    unsigned int gs;
    unsigned int ldtr;
    unsigned int iopb;
    unsigned int ssp;
} __attribute__((packed));

struct GDT{
    unsigned int size : 16;
    struct Segdesc* offset;
} __attribute__((packed));

extern void SetupGDT();
extern void _setGDTR(struct GDT*);