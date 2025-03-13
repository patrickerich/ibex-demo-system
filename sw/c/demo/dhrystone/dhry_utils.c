#include "dhry.h"
#include "demo_system.h"
#include "timer.h"

/* Simple memory allocator */
void *malloc(size_t size) {
    static unsigned char memory[16384] __attribute__((aligned(8)));
    static size_t next = 0;
    
    // Ensure alignment
    size_t aligned_next = (next + 7) & ~7;
    void *result = &memory[aligned_next];
    next = aligned_next + size;
    
    // Zero the memory
    for (size_t i = 0; i < size; i++) {
        ((unsigned char *)result)[i] = 0;
    }
    
    return result;
}

clock_t times(struct tms *buffer) {
    unsigned long cycles;
    unsigned long insns;
    asm volatile ("rdcycle %0" : "=r" (cycles));
    asm volatile ("rdinstret %0" : "=r" (insns));
    if (buffer) {
        buffer->tms_utime = cycles;
        buffer->tms_stime = insns;
        buffer->tms_cutime = 0;
        buffer->tms_cstime = 0;
    }
    return cycles;
}

void putdec(unsigned int val) {
    char buffer[12];
    int i = 0;
    
    if (val == 0) {
        putchar('0');
        return;
    }
    
    while (val > 0) {
        buffer[i++] = '0' + (val % 10);
        val /= 10;
    }
    
    while (i > 0) {
        putchar(buffer[--i]);
    }
}

// Print float with 3 decimal places
void print_float3(float val) {
    int whole = (int)val;
    int frac = (int)((val - whole) * 1000 + 0.5);
    if (frac < 0) frac = -frac;
    
    putdec(whole);
    putchar('.');
    if (frac < 100) putchar('0');
    if (frac < 10) putchar('0');
    putdec(frac);
}