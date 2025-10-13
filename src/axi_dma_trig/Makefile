PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))

.PHONY: all
all: 
	source $(PROJECT_DIR)/axi_dma_trig.sh;

clean:
	rm -rf $(PROJECT)