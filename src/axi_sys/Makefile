PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))

.PHONY: all
all: 
	source $(PROJECT_DIR)/axi_sys.sh;

clean:
	rm -rf $(PROJECT)