#!/bin/bash
PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))

.PHONY: all
all: clean adrv9001_sdk hdl sw

hdl: | $(PROJECT)/vivado
$(PROJECT)/vivado:
	@echo '$(shell printf '\033')[0;33mBuilding $(PROJECT) Programmable Logic $(shell printf '\033')[0m'
	vivado -mode batch -nojournal -quiet -source $(shell cygpath -w $(PROJECT_DIR))\hdl\make_hdl.tcl -notrace 

sw: adrv9001_sdk parse_profile

parse_profile:
	rm -rf $(PROJECT)/profile
	@mkdir -p $(PROJECT)/profile;
	@echo '$(shell printf '\033')[0;33mBuilding $(PROJECT) Software $(shell printf '\033')[0m'
	@read -p "Enter path of directory containing exported C99 code from TES: " C99_DIR; \
	$(PROJECT_DIR)/sw/profile_parse.sh $$C99_DIR $(PROJECT)/profile
  
adrv9001_sdk: | adrv9001-sdk 
adrv9001-sdk:  
	wget -nc "https://www.analog.com/media/en/evaluation-boards-kits/evaluation-software/adrv9001-sdk-eval-software.zip" || true
	unzip adrv9001-sdk-eval-software.zip  
	chmod -R 777 adrv9001-sdk

clean:
	rm -rf $(PROJECT)  
