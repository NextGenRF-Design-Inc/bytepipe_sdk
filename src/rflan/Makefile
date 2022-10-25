PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))

.PHONY: all
all: clean
	@read -p "Select Device [ xczu3cg, xczu3eg, xczu2cg, xczu2eg ]: " DEVICE; \
	$(MAKE) -f $(PROJECT_DIR)/../adrv9001/Makefile hdl; \
	$(MAKE) -f $(PROJECT_DIR)/../adrv9001/Makefile sdk; \
	$(MAKE) -f $(PROJECT_DIR)/../axi_dma/Makefile; \
	source $(PROJECT_DIR)/rflan.sh; build_hdl $$DEVICE; \
	source $(PROJECT_DIR)/rflan.sh; build_sw $$DEVICE;

hdl:
	@read -p "Select Device [ xczu3cg, xczu3eg, xczu2cg, xczu2eg ]: " DEVICE; \
	$(MAKE) -f $(PROJECT_DIR)/../adrv9001/Makefile hdl; \
	$(MAKE) -f $(PROJECT_DIR)/../axi_dma/Makefile; \
	source $(PROJECT_DIR)/rflan.sh; build_hdl $$DEVICE
  
sw:
	@read -p "Select Device [ xczu3cg, xczu3eg, xczu2cg, xczu2eg ]: " DEVICE; \
	wget -nc "https://github.com/NextGenRF-Design-Inc/bytepipe_sdk/releases/download/v10.25.22/$(PROJECT)_$$DEVICE.xsa" -O $(PROJECT)/$(PROJECT)_$$DEVICE.xsa || true; \
	$(MAKE) -f $(PROJECT_DIR)/../adrv9001/Makefile sdk; \
	source $(PROJECT_DIR)/rflan.sh; build_sw $$DEVICE
  
clean:
	rm -rf $(PROJECT)
  
profile:	
	@read -p "Enter path of directory containing exported C99 code from TES: " C99_DIR; \
  source $(PROJECT_DIR)/../adrv9001/adrv9001.sh; profile_parse $$C99_DIR $(PROJECT_DIR)/sw/adrv9001/profile
	
sd_card:
	rm -rf $(PROJECT)/sd_card
	cp -rf $(PROJECT)/rpu_system/Debug/sd_card $(PROJECT)
	cp -rf $(PROJECT_DIR)/resources/* $(PROJECT)/sd_card  
  



