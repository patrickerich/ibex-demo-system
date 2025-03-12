// Main entry point for Dhrystone benchmark on Ibex
#include <stdbool.h>
#include "demo_system.h"
#include "gpio.h"
#include "timer.h"
#include "dhry.h"



// Declare the Dhrystone benchmark function
int dhry_main(void);

// Globals shared with dhry_1.c
extern struct tms time_info;
extern long Begin_Time, End_Time;
extern int Number_Of_Runs;

// UART interrupt handler
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

// Custom integer to decimal conversion
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

// Decimal with sign
void putdec_signed(int val) {
    if (val < 0) {
        putchar('-');
        putdec((unsigned int)-val);
    } else {
        putdec((unsigned int)val);
    }
}

// Print float with 3 decimal places
void print_float3(float val) {
    int whole = (int)val;
    int frac = (int)((val - whole) * 1000 + 0.5);
    if (frac < 0) frac = -frac;
    
    putdec_signed(whole);
    putchar('.');
    if (frac < 100) putchar('0');
    if (frac < 10) putchar('0');
    putdec(frac);
}

int main(void) {
    // System initialization
    install_exception_handler(UART_IRQ_NUM, &uart_irq_handler);
    uart_enable_rx_int();
    
    timer_init();
    timer_enable(50000);  // More frequent timer updates
    
    set_outputs(GPIO_OUT, 0x10);
    
    uint64_t last_elapsed_time = get_elapsed_time();
    bool benchmark_complete = false;
    
    puts("\nDhrystone 2.1 Benchmark for Ibex\n");
    puts("Press 'r' to run the benchmark\n");
    
    while (1) {
        uint64_t cur_time = get_elapsed_time();
        
        if (cur_time != last_elapsed_time) {
            last_elapsed_time = cur_time;
            
            if (run_benchmark && !benchmark_complete) {
                // Initialize Dhrystone specific variables
                Number_Of_Runs = 100000;  // Start with a reasonable value
                
                puts("Running standard Dhrystone benchmark with ");
                putdec(Number_Of_Runs);
                puts(" iterations...\n");
                
                // Call the main Dhrystone function
                int result = dhry_main();
                
                if (result == 0) {
                    puts("Benchmark completed successfully!\n");
                    
                    // Display results in decimal format
                    puts("Elapsed ticks: ");
                    putdec((unsigned int)(End_Time - Begin_Time));
                    puts("\n");
                    
                    // Calculate DMIPS/MHz
                    long long estimated_cycles = (long long)(End_Time - Begin_Time) * 50000;
                    float dmips = ((float)Number_Of_Runs * 1000000.0f) / (float)(estimated_cycles * 1757);
                    // When using direct cycle counts
                    // float dmips = ((float)Number_Of_Runs * 1000000.0f) / (float)(cycles * 1757);

                    puts("DMIPS/MHz: ");
                    print_float3(dmips);
                    puts("\n");
                } else {
                    puts("Benchmark failed with error code: ");
                    putdec(result);
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
