# CMakeLists.txt for Dhrystone benchmark

cmake_minimum_required(VERSION 3.10)
project(dhrystone C)

# Set C standard
set(CMAKE_C_STANDARD 11)

# Add include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/sw/c/common
)

# Source files for the benchmark
set(SOURCES
    dhry_1.c
    dhry_2.c
    dhry_utils.c
)

# Link to the common library path
link_directories(${CMAKE_BINARY_DIR}/sw/c/common)

# Create the executable
add_executable(dhrystone ${SOURCES})

# Link with the common library which contains demo_system, timer, etc.
target_link_libraries(dhrystone common)

# Compiler flags
# Optimization flags - you might want to adjust these
target_compile_options(dhrystone PRIVATE
    -O2                  # Optimization level
    -march=rv32imc       # RISC-V architecture spec
    -mabi=ilp32          # ABI type
    -fno-builtin         # Don't use built-in functions
    -fno-common          # Don't place uninitialized globals in common block
    -Wall                # Enable warnings
    -g                   # Include debug info
)

# Copy the binary to the output directory if needed
# (Uncomment and modify if you have a specific output directory)
# add_custom_command(TARGET dhrystone POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:dhrystone> ${OUTPUT_DIR}
# )
