#include <stdint.h>

#define SER_REG_RX 0 // R
#define SER_REG_TX 0 // W
#define SER_REG_INTERRUPT_EN 1 // RW
#define SER_REG_DLAB_DIVISOR_LSB 0 // RW
#define SER_REG_DLAB_DIVISOR_MSB 1 // RW
#define SER_REG_INTERRUPT_ID 2 // R
#define SER_REG_FIFO 2 // W
#define SER_REG_LINECTL 3 // RW
#define SER_REG_MODEMCTL 4 // RW
#define SER_REG_LINESTAT 5 // R
#define SER_REG_MODEMSTAT 6 // R
#define SER_REG_SCRATCPAD 7 // RW

#define SER_TESTNUM 0x55
#define SER_DIVISOR 128
#define SER_DLAB_SET 0x80
#define SER_DLAB_CLR 0x7F
#define SER_PARITY_MD 1
#define SER_STOP_BITS 0 // 1 Stop bit
#define SER_FIFO 0xC7

#define SER_MODEM_DATAREADY 1
#define SER_MODEM_REQUESTTOSEND 2
#define SER_MODEM_OUT 7 
#define SER_MODEM_LOOP 16

#define SER_DATA_BITS_5 0
#define SER_DATA_BITS_6 1
#define SER_DATA_BITS_7 2
#define SER_DATA_BITS_8 3

void Serial_SendString(uint16_t port, char* str);
int SetupSerialPort(uint16_t port);