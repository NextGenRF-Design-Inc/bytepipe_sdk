ESC:=$(shell printf '\033')
GREEN:=$(ESC)[1;32m
RED:=$(ESC)[1;31m
HL:=$(ESC)[0;33m
NC:=$(ESC)[0m

msg:
	@echo 'Building [$(HL)$(CURDIR)$(NC)]'
  
build:  
	vivado -mode batch -nojournal -quiet -source make.tcl -notrace

.PHONY:	msg build
