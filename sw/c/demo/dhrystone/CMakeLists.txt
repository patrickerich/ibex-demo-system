# # Minimal Dhrystone-like benchmark
# add_executable(dhrystone
#   dhry_minimal.c
# )

# target_link_libraries(dhrystone common)

# Full Dhrystone benchmark
add_executable(dhrystone
  dhry_main.c
  dhry_1.c
  dhry_2.c
  dhry_utils.c
)

target_link_libraries(dhrystone common)

# Define configuration flags
target_compile_definitions(dhrystone PRIVATE
  -DNOSTRUCTASSIGN      # Use safer structure assignment
  -DTIMER_ENABLED=50000 # Timer frequency
)

# Optimization flags
target_compile_options(dhrystone PRIVATE
  -O2                   # Optimize for performance
  -falign-functions=4   # Ensure proper alignment
  -falign-jumps=4
  -falign-loops=4
)