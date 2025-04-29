#include "GDT.h"

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

static struct Segdesc segdesc_Array[3];

static struct GDT gdt;

extern void SetupGDT(
) {
    segdesc_Array[0] = sd_Null;
    segdesc_Array[1] = sd_Code0;
    segdesc_Array[2] = sd_Data0;

    gdt.size = GDT_SIZE;
    gdt.offset = segdesc_Array;

    _setGDTR(&gdt);
}
