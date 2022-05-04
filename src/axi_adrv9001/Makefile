PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))

.PHONY: all
all: hdl sw

hdl: | $(PROJECT)/vivado
$(PROJECT)/vivado:
	@echo 'Building $(shell printf '\033')[0;33m$(PROJECT)$(shell printf '\033')[0m hdl'
	vivado -mode batch -nojournal -quiet -source $(shell cygpath -w $(PROJECT_DIR))\hdl\make_hdl.tcl -notrace 
  
sw:
  
clean:
	rm -rf $(PROJECT)