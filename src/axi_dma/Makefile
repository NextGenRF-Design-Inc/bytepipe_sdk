PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))

.PHONY: all
all: 
	$(PROJECT_DIR)/axi_dma.sh

clean:
	rm -rf $(PROJECT)