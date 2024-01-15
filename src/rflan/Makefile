PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))
SRCDIR := $(realpath $(dir $(PROJECT_DIR)/../))
SDK_SRCDIR := $(SRCDIR)
DEV := $(shell bash -c 'read -p "Select Device [ xczu3cg, xczu3eg, xczu2cg, xczu2eg ]: " dev; echo $$dev-sbva484-1-e')

.PHONY: all
all: clean hdl sw fsbl

hdl:
	$(MAKE) -f $(SDK_SRCDIR)/adrv9001/Makefile hdl
	$(MAKE) -f $(SDK_SRCDIR)/axi_dma/Makefile
	source $(SDK_SRCDIR)/include.sh; build_hdl $(SRCDIR) $(PROJECT) $(DEV)
  
sw: 
	$(MAKE) -f $(SDK_SRCDIR)/adrv9001/Makefile sdk
	source $(SDK_SRCDIR)/include.sh; build_sw $(SRCDIR) $(PROJECT) $(DEV)
   
fsbl:
	cp $(SDK_SRCDIR)/fsbl/sw/xfsbl_main.c $(PROJECT)/hwp/zynqmp_fsbl   
  
clean:
	rm -rf $(PROJECT)
  
profile:	
	@read -p "Enter path of directory containing exported C99 code from TES: " C99_DIR; \
  source $(SDK_SRCDIR)/adrv9001/adrv9001.sh; profile_parse $$C99_DIR $(PROJECT_DIR)/sw/adrv9001/profile
	
  



