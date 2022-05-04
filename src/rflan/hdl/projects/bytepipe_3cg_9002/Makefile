wrkDir:=${PWD}
srcDir:=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
hdlDir:=$(wrkDir)/Adi/hdl
whichVivado:= $(shell which vivado) |rev | cut -d'/' -f3- |rev
vivadoDir=$(realpath $(shell echo $(whichVivado)))
os:=$(uname -s)
ifeq (,$(CYGWIN,$(os)))
winSrcDir:=$(shell cygpath -w $(srcDir))
endif
ESC:=$(shell printf '\033')
GREEN:=$(ESC)[1;32m
RED:=$(ESC)[1;31m
HL:=$(ESC)[0;33m
NC:=$(ESC)[0m

.PHONY:	all

all: directories boards adi_hdl ip msg build

directories:
ifeq (,$(CYGWIN,$(os)))
	@echo 'Source Directory: $(winSrcDir)'
else
	@echo 'Source Directory: $(srcDir)'
endif
	@echo 'Build Directory: $(wrkDir)'
	@echo 'Vivado Directory: ' $(vivadoDir)

boards:
	cp -r $(realpath $(srcDir)/../../boards/bytepipe_3cg_som) $(vivadoDir)/data/boards/board_files/

ip:
	$(MAKE) -C $(wrkDir)/Adi/hdl/library/util_pack/util_upack2 xilinx || exit $$?;
	$(MAKE) -C $(wrkDir)/Adi/hdl/library/util_pack/util_cpack2 xilinx || exit $$?;
	$(MAKE) -C $(wrkDir)/Adi/hdl/library/axi_dmac xilinx || exit $$?;
	$(MAKE) -C $(srcDir)/../../library/adrv9002 || exit $$?;
    
msg:
	@echo 'Building [$(HL)$(srcDir)$(NC)]'
  
build:
ifeq (,$(CYGWIN,$(os)))
	vivado -mode batch -nojournal -quiet -source $(winSrcDir)/make.tcl -notrace
else
	vivado -mode batch -nojournal -quiet -source $(srcDir)/make.tcl -notrace
endif


adi_hdl: | $(hdlDir)

$(hdlDir):
	@mkdir -p $(hdlDir)
	git clone --branch hdl_2019_r2 https://github.com/analogdevicesinc/hdl.git $(wrkDir)/Adi/hdl
	@sed -i 's/2019.1/2019.1.3/g' $(wrkDir)/Adi/hdl/library/scripts/adi_ip_xilinx.tcl





	

