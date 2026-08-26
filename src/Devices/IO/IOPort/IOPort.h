// Provides high level access to IO ports (inb outb ind outd)

#include <stdint.h>

// Writes the byte 'val' to the IO port specified by 'port'
inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

// Returns the byte from the IO port specified by 'port'
inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

// Writes the dword 'val' to the IO port specified by 'port'
inline void outd(uint16_t port, uint32_t val)
{
    __asm__ volatile ( "out %d0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

// Returns the dword from the IO port specified by 'port'
inline uint32_t ind(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "in %w1, %d0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}