// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include <stdbool.h>

#include "demo_system.h"
#include "gpio.h"
#include "pwm.h"
#include "timer.h"

#define USE_GPIO_SHIFT_REG 0

int main(void) {
  uart_enable_rx_int();

  // Initialize the timer hardware
  timer_init();
  timer_enable(500000);

  uint64_t last_elapsed_time = get_elapsed_time();
  
  // Add variables to store the baseline timer value and measurements
  uint64_t baseline_time = timer_read();
  uint64_t measurement_start = 0;
  uint64_t measurement_end = 0;
  uint64_t elapsed_hw_ticks = 0;

  while (1) {
    uint64_t cur_time = get_elapsed_time();

    if (cur_time != last_elapsed_time) {
      last_elapsed_time = cur_time;

      // Disable interrupts during output
      set_global_interrupt_enable(0);

      // Take a measurement using timer_read() instead of read_cycles()
      measurement_start = timer_read();
      
      // Something to measure (just a small delay loop as an example)
      for (volatile int i = 0; i < 1000; i++) { }
      
      measurement_end = timer_read();
      elapsed_hw_ticks = measurement_end - measurement_start;

      // Print results
      puts("Hello world!\n");
      puts("Timer ticks elapsed: 0x");
      puthex((uint32_t)(elapsed_hw_ticks >> 32)); // Upper 32 bits
      puthex((uint32_t)elapsed_hw_ticks);         // Lower 32 bits
      puts("\n");
      
      puts("Total time since start: 0x");
      uint64_t total_time = timer_read() - baseline_time;
      puthex((uint32_t)(total_time >> 32));
      puthex((uint32_t)total_time);
      puts("\n");

      // Re-enable interrupts
      set_global_interrupt_enable(1);
    }

    asm volatile("wfi");
  }
}