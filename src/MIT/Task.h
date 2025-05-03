struct TaskState{
    void* IP;
    unsigned int EFlags;
    unsigned int EAX;
    unsigned int EBX;
    unsigned int ECX;
    unsigned int EDX;
    unsigned int ESI;
    unsigned int EDI;
    unsigned int ESP;
    unsigned int EBP;
} __attribute__((packed));

struct TaskDescriptor{
    unsigned int interval   : 16;
    unsigned int execTime   : 8;
    unsigned int counter    : 32;
    struct TaskState* taskState;
} __attribute__((packed));