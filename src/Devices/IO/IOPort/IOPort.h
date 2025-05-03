#include <stdint.h>

inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

inline void outd(uint16_t port, uint32_t val)
{
    __asm__ volatile ( "out %d0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

inline uint32_t ind(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "in %w1, %d0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}