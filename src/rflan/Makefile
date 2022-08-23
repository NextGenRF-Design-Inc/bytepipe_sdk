PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))

.PHONY: all
all:
	rm -rf $(PROJECT)
	@mkdir -p $(PROJECT)/
	@read -p "Select Device [ xczu3cg, xczu3eg, xczu2cg, xczu2eg ]: " DEVICE; \
	$(MAKE) -f $(PROJECT_DIR)/../adrv9001/Makefile hdl; \
	$(MAKE) -f $(PROJECT_DIR)/../adrv9001/Makefile adrv9001_sdk; \   
	$(MAKE) -f $(PROJECT_DIR)/../axi_dma/Makefile hdl; \
	echo "$(shell printf '\033')[0;33mBuilding $$DEVICE $(PROJECT) Programmable Logic $(shell printf '\033')[0m"; \
	vivado -mode batch -nojournal -quiet -source "$(shell cygpath -w $(PROJECT_DIR))\hdl\make_hdl.tcl" -notrace -tclargs $$DEVICE; \
	echo "$(shell printf '\033')[0;33mBuilding $$DEVICE $(PROJECT) Software $(shell printf '\033')[0m"; \
	xsct.bat "$(shell cygpath -w $(PROJECT_DIR))\sw\make_sw.tcl" $$DEVICE
	cp $(PROJECT)/rpu_system/Debug/sd_card/BOOT.bin $(PROJECT)
  
hdl:
	@mkdir -p $(PROJECT)/
	rm -rf $(PROJECT)/vivado
	@read -p "Select Device [ xczu3cg, xczu3eg, xczu2cg, xczu2eg ]: " DEVICE; \
	rm -rf $(PROJECT)/rflan_$$DEVICE.xsa; \
	$(MAKE) -f $(PROJECT_DIR)/../adrv9001/Makefile hdl; \
	$(MAKE) -f $(PROJECT_DIR)/../axi_dma/Makefile hdl; \
	echo "$(shell printf '\033')[0;33mBuilding $$DEVICE $(PROJECT) Programmable Logic $(shell printf '\033')[0m"; \
	vivado -mode batch -nojournal -quiet -source "$(shell cygpath -w $(PROJECT_DIR))\hdl\make_hdl.tcl" -notrace -tclargs $$DEVICE
 
sw:	
	@mkdir -p $(PROJECT)/
	rm -rf $(PROJECT)/rpu
	rm -rf $(PROJECT)/rpu_system
	rm -rf $(PROJECT)/.metadata
	rm -rf $(PROJECT)/hwp
	rm -rf $(PROJECT)/RemoteSystemsTempFiles
	rm -rf $(PROJECT)/.analytics
	rm -rf $(PROJECT)/*.log
	rm -rf $(PROJECT)/*.bin
	@read -p "Select Device [ xczu3cg, xczu3eg, xczu2cg, xczu2eg ]: " DEVICE; \
	wget -nc "https://github.com/NextGenRF-Design-Inc/bytepipe_sdk/releases/download/v.11.04.21/rflan_$$DEVICE.xsa" -O $(PROJECT)/rflan_$$DEVICE.xsa || true; \
	$(MAKE) -f $(PROJECT_DIR)/../adrv9001/Makefile adrv9001_sdk; \
	echo "$(shell printf '\033')[0;33mBuilding $$DEVICE $(PROJECT) Software $(shell printf '\033')[0m"; \
	xsct.bat "$(shell cygpath -w $(PROJECT_DIR))\sw\make_sw.tcl" $$DEVICE
	cp $(PROJECT)/rpu_system/Debug/sd_card/BOOT.bin $(PROJECT)
  
profile:	
	$(MAKE) -f $(PROJECT_DIR)/../adrv9001/Makefile sw
	rm -rf $(PROJECT_DIR)/sw/adrv9001/profile 
	cp -r adrv9001/profile $(PROJECT_DIR)/sw/adrv9001  
  
sd_card:
	rm -rf $(PROJECT)/sd_card
	cp -rf $(PROJECT)/rpu_system/Debug/sd_card $(PROJECT)
	cp -rf $(PROJECT_DIR)/resources/* $(PROJECT)/sd_card  
  



