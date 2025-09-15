#include "GDT.h"
static const struct TSS tss0;

static const struct Segdesc sd_Null = {
    .limit0 = 0,
    .limit1 = 0,
    .base0 = 0,
    .base1 = 0,
    .access = 0,
    .flags = 0,
};

static const struct Segdesc sd_Code0 = {
    .limit0 = GLOB_LIMIT0,
    .limit1 = GLOB_LIMIT1,
    .base0 = GLOB_BASE0,
    .base1 = GLOB_BASE1,
    .access = CODE_ACCESS,
    .flags = GLOB_FLAGS,
};

static const struct Segdesc sd_Data0 = {
    .limit0 = GLOB_LIMIT0,
    .limit1 = GLOB_LIMIT1,
    .base0 = GLOB_BASE0,
    .base1 = GLOB_BASE1,
    .access = DATA_ACCESS,
    .flags = GLOB_FLAGS,
};

// if you touch this you will die
static struct Segdesc sd_TSS0 = {
    .limit0 = 0x89,
    .limit1 = 0,
    .access = TSS_ACCESS,
};

static struct Segdesc segdesc_Array[4];

static struct GDT gdt;

extern void SetupGDT(
) {
    sd_TSS0.base0 = ((int)&tss0) & 0xFFFFFF;
    sd_TSS0.base1 = ((int)&tss0 << 24) & 0xFF;

    segdesc_Array[0] = sd_Null;
    segdesc_Array[1] = sd_Code0;
    segdesc_Array[2] = sd_Data0;
    segdesc_Array[3] = sd_TSS0;

    gdt.size = GDT_SIZE;
    gdt.offset = segdesc_Array;

    _setGDTR(&gdt);
}
