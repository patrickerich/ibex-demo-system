ARTY35 := xc7a35ticsg324-1L
ARTY100 := xc7a100tcsg324-1
FPGA ?= $(ARTY100)
DEVICE ?= /dev/ttyUSB1
BAUDRATE ?= 115200

# HWPROG := cmdint
# HWPROG := blank
# HWPROGFILE = $(PWD)/sw/c/build/$(HWPROG)/$(HWPROG)
HWPROGFILE = $(PWD)/sw/c/build/demo/hello_world/demo

# SIMPROG := cmdint
# # SIMPROG := demo
# # SIMPROG := demo_uart
# SIMPROGFILE := $(PWD)/sw/build/$(SIMPROG)/$(SIMPROG).vmem

.PHONY: build-hw
build-hw:
	fusesoc --cores-root=. run --target=synth --setup --build \
		lowrisc:ibex:demo_system --part $(FPGA) \
		--SRAMInitFile=$(HWPROGFILE)

.PHONY: build-sw-c
build-sw-c:
	rm -rf sw/c/build 
	mkdir sw/c/build
	pushd sw/c/build
	cmake ..
	$(MAKE)
	popd

.PHONY: build-sw-rust
build-sw-rust:
	cargo clean
	pushd sw/rust
	cargo build --bin led
	popd

.PHONY: program-hw
program-hw:
	# fusesoc --cores-root=. run --target=synth --run \
	# 	lowrisc:ibex:demo_system 
	$(MAKE) -C $(PWD)/build/lowrisc_ibex_demo_system_0/synth-vivado/ pgm

.PHONY: start-vivado
start-vivado:
	make -C $(PWD)/build/lowrisc_ibex_demo_system_0/synth-vivado/ build-gui &

# .PHONY: load-demo-run
# load-demo-run:
# 	./util/load_demo_system.sh run $(HWPROGFILE)

# .PHONY: run-cmdint
# run-cmdint:
# 	python sw/cmdint/cmdint.py

# .PHONY: setup-sims
# setup-sims:
# 	fusesoc --cores-root=. run --target=sim --setup \
# 		lowrisc:ibex:demo_system   \
# 		--SRAMInitFile=$(SIMPROGFILE)
# # SRAMInitFile can be modified/overridden in the simulation setup

# .PHONY: run-sims
# run-sims: build-sw setup-sims
# 	(cd sim && mkdir -p sim_reports && \
# 	SIM=verilator pytest \
# 	   -n 1 \
# 	   -o cache_dir=.pytest_cache \
# 	   -o python_files="pytest_*.py" \
# 	   --html=sim_reports/sim_report_$$(date +%Y%m%d%H%M%S).html \
# 	)
# # Running multiple parallel simulations with different SRAM files seems to cause some problems?

# .PHONY: view-wave
# view-wave:
# 	gtkwave -f build/lowrisc_ibex_demo_system_0/sim-cocotb/dump.fst \
# 	        -a sim/sim.wav &

# .PHONY: clean-sw
# clean-sw:
# 	-rm -rf sw/build

# .PHONY: clean-hw
# clean-hw:
# 	-rm -rf build/lowrisc_ibex_demo_system_0/synth-vivado

# .PHONY: clean-sim
# clean-sim:
# 	-rm -rf build/lowrisc_ibex_demo_system_0/sim-*
# 	-rm -rf sim/__pycache__ sim/.pytest_cache

# .PHONY: clean-sim-results
# clean-sim-results:
# 	-rm -rf sim/sim_reports

# .PHONY: clean-all
# clean-all: clean-sw clean-hw clean-sim clean-sim-results
# 	-rm -rf build
