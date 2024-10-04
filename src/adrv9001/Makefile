PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))
SRCDIR := $(realpath $(dir $(PROJECT_DIR)/../))
SDK_SRCDIR := $(SRCDIR)

.PHONY: all
all: clean profile sdk hdl

hdl:
	source $(PROJECT_DIR)/adrv9001.sh; package_hdl
  
profile:
	@read -p "Enter path of directory containing exported C99 code from TES: " C99_DIR; \
  source $(PROJECT_DIR)/adrv9001.sh; profile_parse $$C99_DIR $(PROJECT)/profile
  
sdk: | adrv9001-sdk 
adrv9001-sdk:  
	source $(PROJECT_DIR)/adrv9001.sh; download_sdk

clean:
	rm -rf $(PROJECT)  