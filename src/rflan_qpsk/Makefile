PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))

.PHONY: all
all: ip hdl sw

hdl: | $(PROJECT)/vivado
$(PROJECT)/vivado:
	@echo 'Building $(shell printf '\033')[0;33m$(PROJECT)$(shell printf '\033')[0m'
	vivado -mode batch -nojournal -quiet -source $(shell cygpath -w $(PROJECT_DIR))\hdl\make_hdl.tcl -notrace 
  
sw:
	xsct.bat $(shell cygpath -w $(PROJECT_DIR))\sw\make_sw.tcl
  
clean:
	rm -rf $(PROJECT)
  
ip: 
	$(MAKE) -f $(PROJECT_DIR)/../axi_adrv9001/Makefile  