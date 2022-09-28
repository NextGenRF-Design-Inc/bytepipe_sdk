PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))

.PHONY: all
all: hdl

$(PROJECT):
	@mkdir -p $(PROJECT)
	git clone --branch hdl_2019_r2  https://github.com/analogdevicesinc/hdl.git $(PROJECT)
	@sed -i 's/2019.1/2021.1/g' $(PROJECT)/library/scripts/adi_ip_xilinx.tcl  
  
hdl: | $(PROJECT)
	@echo '$(shell printf '\033')[0;33mBuilding $(PROJECT) Programmable Logic $(shell printf '\033')[0m'
	$(MAKE) -C $(PROJECT)/library/util_pack/util_upack2 xilinx || exit $$?;
	$(MAKE) -C $(PROJECT)/library/util_pack/util_cpack2 xilinx || exit $$?;
	$(MAKE) -C $(PROJECT)/library/axi_dmac xilinx || exit $$?;   

clean:
	rm -rf $(PROJECT)