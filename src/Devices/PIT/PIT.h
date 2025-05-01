#include <stdint.h>

#define PIT0_DATA 0x40
#define PIT1_DATA 0x41
#define PIT2_DATA 0x42
#define PIT_CMD 0x43

#define PIT_MD_INTERRUPT 0x0
#define PIT_MD_ONESHOT 0x2
#define PIT_MD_RATE 0x4
#define PIT_MD_SQUARE 0x6
#define PIT_MD_SWSTROBE 0x8
#define PIT_MD_HWSTROBE 0xA

#define PIT_ACC_LO 0x10
#define PIT_ACC_HI 0x20
#define PIT_ACC_LOHI 0x30

void SetupPIT(int whichPIT, uint8_t mode);
void PIT_SetReload(int PIT, uint16_t reload);