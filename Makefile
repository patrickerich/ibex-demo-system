THIS_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

ARTY35 := xc7a35ticsg324-1L
ARTY100 := xc7a100tcsg324-1
FPGA ?= $(ARTY100)
DEVICE ?= /dev/ttyUSB1
BAUDRATE ?= 115200

SWCDIR   := $(THIS_DIR)/sw/c
SWRDIR   := $(THIS_DIR)/sw/rust 
SWCBUILD := $(SWCDIR)/build
HWPROG   ?= $(SWCBUILD)/demo/hello_world/demo

HWBUILD  := $(THIS_DIR)/build/lowrisc_ibex_demo_system_0/synth-vivado

.PHONY: clean-sw-c
clean-sw-c:
	-rm -rf $(SWCBUILD)

.PHONY: build-sw-c
build-sw-c: clean-sw-c
	mkdir -p $(SWCBUILD)
	(cd $(SWCBUILD) && cmake .. && make)

.PHONY: clean-sw-rust
clean-sw-rust:
	(cd $(SWRDIR) && cargo clean)

.PHONY: build-sw-rust
build-sw-rust: clean-sw-rust
	(cd $(SWRDIR) && cargo build --bin led)

.PHONY: build-hw
build-hw:
	fusesoc --cores-root=. run --target=synth --setup --build \
		lowrisc:ibex:demo_system --part $(FPGA) \
		--SRAMInitFile=$(HWPROG)

.PHONY: clean-hw
clean-hw:
	-rm -rf $(HWBUILD)

.PHONY: program-hw
program-hw:
	$(MAKE) -C $(THIS_DIR)/build/lowrisc_ibex_demo_system_0/synth-vivado/ pgm

.PHONY: start-vivado
start-vivado:
	make -C $(THIS_DIR)/build/lowrisc_ibex_demo_system_0/synth-vivado/ build-gui &

.PHONY: load-demo-run
load-demo-run:
	$(THIS_DIR)/util/load_demo_system.sh run $(THIS_DIR)/sw/c/build/demo/hello_world/demo

.PHONY: clean-sim
clean-sim:
	-rm -f *.fst *.vcd

.PHONY: clean-all
clean-all:
	$(MAKE) clean-sw-c
	$(MAKE) clean-sw-rust
	$(MAKE) clean-hw
	$(MAKE) clean-sim
	-rm -rf $(THIS_DIR)/build


