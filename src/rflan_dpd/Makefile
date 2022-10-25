PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))

.PHONY: all
all: clean sw

sw:
	@read -p "Select Device [ xczu3cg, xczu3eg, xczu2cg, xczu2eg ]: " DEVICE; \
	wget -nc "https://github.com/NextGenRF-Design-Inc/bytepipe_sdk/releases/download/v10.25.22/rflan_$$DEVICE.xsa" -O $(PROJECT)/rflan_$$DEVICE.xsa || true; \
	$(MAKE) -f $(PROJECT_DIR)/../adrv9001/Makefile sdk; \
	source $(PROJECT_DIR)/rflan_dpd.sh; build_sw $$DEVICE
  
clean:
	rm -rf $(PROJECT)
    
profile:	
	@read -p "Enter path of directory containing exported C99 code from TES: " C99_DIR; \
  source $(PROJECT_DIR)/../adrv9001/adrv9001.sh; profile_parse $$C99_DIR $(PROJECT_DIR)/sw/adrv9001/profile
  
sd_card:
	rm -rf $(PROJECT)/sd_card
	cp -rf $(PROJECT)/rpu_system/Debug/sd_card $(PROJECT)
	cp -rf $(PROJECT_DIR)/resources/* $(PROJECT)/sd_card
  



