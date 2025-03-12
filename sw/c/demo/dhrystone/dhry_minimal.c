// Minimal Dhrystone-like benchmark for Ibex
#include <stdbool.h>
#include <string.h>
#include "demo_system.h"
#include "gpio.h"
#include "timer.h"

// Number of iterations for benchmark
#define NUM_RUNS 100000

// Simple record structure (similar to Dhrystone but simpler)
typedef struct {
    int value;
    char string[32];
} Record;

// Simplified procedure 1 (similar to Proc_1 in Dhrystone)
void Proc_1(Record *ptr) {
    ptr->value = 5;
    strcpy(ptr->string, "MODIFIED STRING");
}

// Simplified procedure 2 (similar to Proc_2 in Dhrystone) 
void Proc_2(int *ptr) {
    int loc = *ptr + 10;
    *ptr = loc - 5;
}

// Simplified procedure with array operations
void Proc_3(int array[], int idx) {
    array[idx] = array[idx] + 1;
    array[idx+1] = array[idx];
}

// Simplified string comparison function
bool Func_1(const char *s1, const char *s2) {
    return strcmp(s1, s2) == 0;
}

// UART interrupt handler - press 'r' to run the benchmark
void uart_irq_handler(void) __attribute__((interrupt));
volatile bool run_benchmark = false;

void uart_irq_handler(void) {
    int uart_in_char;
    while ((uart_in_char = uart_in(DEFAULT_UART)) != -1) {
        uart_out(DEFAULT_UART, uart_in_char);
        if (uart_in_char == 'r' || uart_in_char == 'R') {
            run_benchmark = true;
        }
    }
}

// Add this function to print decimal numbers
void putdec(int val) {
    char buffer[12]; // Enough for 32-bit integers
    int i = 0;
    
    // Handle negative numbers
    if (val < 0) {
        putchar('-');
        val = -val;
    }
    
    // Special case for zero
    if (val == 0) {
        putchar('0');
        return;
    }
    
    // Convert to decimal digits in reverse order
    while (val > 0) {
        buffer[i++] = '0' + (val % 10);
        val /= 10;
    }
    
    // Print in correct order
    while (i > 0) {
        putchar(buffer[--i]);
    }
}

// Function to print floating point with 3 decimal places
void print_float3(float val) {
    int whole = (int)val;
    int frac = (int)((val - whole) * 1000 + 0.5); // Add 0.5 for rounding
    if (frac < 0) frac = -frac;
    
    // Print whole part in decimal
    putdec(whole);
    putchar('.');
    
    // Print fractional part with leading zeros
    if (frac < 100) putchar('0');
    if (frac < 10) putchar('0');
    putdec(frac);
}

int main(void) {
    // System initialization
    install_exception_handler(UART_IRQ_NUM, &uart_irq_handler);
    uart_enable_rx_int();
    
    timer_init();
    timer_enable(5000);
    
    set_outputs(GPIO_OUT, 0x10);
    
    uint64_t last_elapsed_time = get_elapsed_time();
    bool benchmark_complete = false;
    
    // Variables for the benchmark
    Record record1;
    Record record2;
    int array[50];
    int int_value = 0;
    char string1[32] = "STRING ONE";
    char string2[32] = "STRING TWO";
    
    puts("\nMinimal Dhrystone-like Benchmark\n");
    puts("Press 'r' to run the benchmark\n");
    
    while (1) {
        uint64_t cur_time = get_elapsed_time();
        
        if (cur_time != last_elapsed_time) {
            last_elapsed_time = cur_time;
            
            if (run_benchmark && !benchmark_complete) {
                puts("Starting benchmark with ");
                putdec(NUM_RUNS);
                puts(" iterations...\n");
                
                // Initialize test data
                record1.value = 10;
                strcpy(record1.string, "DHRYSTONE STRING ONE");
                record2.value = 20;
                strcpy(record2.string, "DHRYSTONE STRING TWO");
                
                for (int i = 0; i < 50; i++) {
                    array[i] = i;
                }
                
                // Start timing
                uint64_t start_time = get_elapsed_time();
                
                // Main benchmark loop
                for (int i = 0; i < NUM_RUNS; i++) {
                    Proc_1(&record1);
                    Proc_2(&int_value);
                    Proc_3(array, i % 40);
                    
                    if (Func_1(string1, string2)) {
                        strcpy(string1, string2);
                    } else {
                        strcpy(string2, string1);
                    }
                    
                    record2.value = record1.value + 5;
                    int_value = array[i % 40] + record2.value;
                }
                
                // End timing
                uint64_t end_time = get_elapsed_time();
                uint64_t elapsed = end_time - start_time;
                
                puts("Benchmark complete!\n");
                puts("Elapsed ticks: ");
                putdec((int)elapsed);
                puts("\n");
                
                // Calculate DMIPS/MHz
                if (elapsed > 0) {
                    long long dmips_x1000 = ((long long)NUM_RUNS * 1000000 * 1000) / 
                    ((long long)elapsed * 5000 * 1757);
                    puts("Estimated DMIPS/MHz: ");
                    print_float3((float)dmips_x1000 / 1000.0);
                    puts("\n");
                }
                
                benchmark_complete = true;
                run_benchmark = false;
            }
            
            // Visual feedback
            uint32_t out_val = read_gpio(GPIO_OUT);
            if (benchmark_complete) {
                out_val = ((out_val << 1) | ((out_val >> 3) & 0x1)) & 0xF0;
                if (out_val == 0) out_val = 0x10;
            } else {
                out_val ^= 0x10;
            }
            set_outputs(GPIO_OUT, out_val);
        }
        
        asm volatile("wfi");
    }
}
