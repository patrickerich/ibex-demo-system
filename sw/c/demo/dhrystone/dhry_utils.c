#include <sys/times.h>
#include "dhry.h"
#include "demo_system.h"
#include "timer.h"

/* Safe memory allocator */
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

/* Timer function for Dhrystone */
clock_t times(struct tms *buffer) {
    uint64_t cycles = get_elapsed_time();
    if (buffer) {
        buffer->tms_utime = cycles;
        buffer->tms_stime = 0;
        buffer->tms_cutime = 0;
        buffer->tms_cstime = 0;
    }
    return cycles;
}

/* Fixed number of iterations */
int scanf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int *n = va_arg(args, int*);
    *n = 5000; // Set appropriate value for Ibex
    va_end(args);
    return 1;
}

// uint64_t get_cycle_count(void) {
//     uint64_t cycles;
//     asm volatile ("rdcycle %0" : "=r" (cycles));
//     return cycles;
// }

uint64_t get_cycle_count(void) {
    // Using standard timer instead of cycle counter
    return (uint64_t)get_elapsed_time();
}
