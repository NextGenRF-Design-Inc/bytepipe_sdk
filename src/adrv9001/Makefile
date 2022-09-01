#!/bin/bash
PROJECT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT := $(shell basename $(PROJECT_DIR))

.PHONY: all
all: clean adrv9001_sdk hdl sw

hdl: | $(PROJECT)/vivado
$(PROJECT)/vivado:
	@echo '$(shell printf '\033')[0;33mBuilding $(PROJECT) Programmable Logic $(shell printf '\033')[0m'
	vivado -mode batch -nojournal -quiet -source $(shell cygpath -w $(PROJECT_DIR))\hdl\make_hdl.tcl -notrace 

sw: adrv9001_sdk copy_c99 parse_c99

copy_c99:
	rm -rf $(PROJECT)/profile
	@mkdir -p $(PROJECT)/profile;
	@echo '$(shell printf '\033')[0;33mBuilding $(PROJECT) Software $(shell printf '\033')[0m'
	@read -p "Enter path of directory containing exported C99 code from TES: " C99_DIR; \
	cp -r $$C99_DIR/initialize*.c $(PROJECT)/profile; \
	cp -r $$C99_DIR/initialize*.h $(PROJECT)/profile; \
	cp -r $$C99_DIR/calibrate*.c $(PROJECT)/profile; \
	cp -r $$C99_DIR/calibrate*.h $(PROJECT)/profile; \
	cp -r $$C99_DIR/configure*.c $(PROJECT)/profile; \
	cp -r $$C99_DIR/configure*.h $(PROJECT)/profile; \
	cp -r $$C99_DIR/prime*.c $(PROJECT)/profile; \
	cp -r $$C99_DIR/prime*.h $(PROJECT)/profile
  
adrv9001_sdk: | adrv9001-sdk 
adrv9001-sdk:  
	wget -nc "https://www.analog.com/media/en/evaluation-boards-kits/evaluation-software/adrv9001-sdk-eval-software.zip" || true
	unzip adrv9001-sdk-eval-software.zip  
	chmod -R 777 adrv9001-sdk
  
parse_c99: files Tx1ExternalPathDelay Tx2ExternalPathDelay Rx1AgcCfg Rx2AgcCfg Tx1DpdInit Tx2DpdInit Tx1DpdCfg Tx2DpdCfg Rx1InterfaceGain Rx2InterfaceGain ParamsClose InitializeClean ConfigureClean

files:
	sed -i '/printf/d' $(PROJECT)/profile/*.c
	sed -i '/getchar/d' $(PROJECT)/profile/*.c  
	sed -i 's/int initialize(adi_adrv9001_Device_t * adrv9001Device_0.*/int initialize(adi_adrv9001_Device_t * adrv9001Device_0)/g' $(PROJECT)/profile/*  
	sed -i '/#include "adi_fpga9001/d' $(PROJECT)/profile/* 
	sed -i '/error_code = adi_fpga9001/d' $(PROJECT)/profile/*   
	sed -i '/linux_uio_init/d' $(PROJECT)/profile/*   
	sed -i 's/adi_adrv9001_Init_t initialize_init_7/adrv9001_params_t Adrv9001Params/g' $(PROJECT)/profile/*  
	#sed -i 's/initialize_init_7/Adrv9001Params/g' $(PROJECT)/profile/*
	sed -i '1i #include "adrv9001.h"' $(PROJECT)/profile/initialize*
	sed -i '1i #include "adrv9001.h"' $(PROJECT)/profile/configure*
	sed -i 's/, adi_fpga9001_Device_t \* fpga9001Device_0//g' $(PROJECT)/profile/*    
	sed -i 's/\}\;/,/g' $(PROJECT)/profile/initializeinit7.c  
	echo '  .Lo1Carrier = {' >> $(PROJECT)/profile/initializeinit7.c  
	grep -m 1 'adi_adrv9001_Carrier_t' $(PROJECT)/profile/initialize.c -A 4 | tail -n4 >> $(PROJECT)/profile/initializeinit7.c  
	echo '  .Lo2Carrier = {' >> $(PROJECT)/profile/initializeinit7.c  
	grep -m 2 'adi_adrv9001_Carrier_t' $(PROJECT)/profile/initialize.c -A 4 | tail -n4 >> $(PROJECT)/profile/initializeinit7.c   
	echo '  .Tx1Attn = 10000,' >> $(PROJECT)/profile/initializeinit7.c  
	echo '  .Tx2Attn = 10000,' >> $(PROJECT)/profile/initializeinit7.c  
	echo '  .Tx1Boost = 0,' >> $(PROJECT)/profile/initializeinit7.c  
	echo '  .Tx2Boost = 0,' >> $(PROJECT)/profile/initializeinit7.c  
	echo '  .Tx1TestModeData = 0xA234ABC1,' >> $(PROJECT)/profile/initializeinit7.c
	echo '  .Tx2TestModeData = 0xA234ABC1,' >> $(PROJECT)/profile/initializeinit7.c
	echo '  .Rx1TestModeData = 0xA234ABC1,' >> $(PROJECT)/profile/initializeinit7.c
	echo '  .Rx2TestModeData = 0xA234ABC1,' >> $(PROJECT)/profile/initializeinit7.c
	echo '  .Tx1SsiEnableDly = 500,' >> $(PROJECT)/profile/initializeinit7.c
	echo '  .Tx2SsiEnableDly = 500,' >> $(PROJECT)/profile/initializeinit7.c
	echo '  .Rx1SsiEnableDly = 500,' >> $(PROJECT)/profile/initializeinit7.c
	echo '  .Rx2SsiEnableDly = 500,' >> $(PROJECT)/profile/initializeinit7.c
	echo '  .Tx1DisableDly = 200,' >> $(PROJECT)/profile/initializeinit7.c
	echo '  .Tx2DisableDly = 200,' >> $(PROJECT)/profile/initializeinit7.c  
	echo '  .Rx1SsiDisableDly = 200,' >> $(PROJECT)/profile/initializeinit7.c
	echo '  .Rx2SsiDisableDly = 200,' >> $(PROJECT)/profile/initializeinit7.c
  
Tx1ExternalPathDelay:  
	rm -rf tmp.txt
	grep 'adi_adrv9001_cals_ExternalPathDelay_Set(adrv9001Device_0, ADI_CHANNEL_1,' $(PROJECT)/profile/initialize.c >> tmp.txt || true
	sed -i 's/error_code = adi_adrv9001_cals_ExternalPathDelay_Set(adrv9001Device_0, ADI_CHANNEL_1,/.Tx1ExternalPathDelay = /g' tmp.txt || true
	sed -i 's/)\;/,/g' tmp.txt || true
	grep '.Tx1ExternalPathDelay' tmp.txt >> $(PROJECT)/profile/initializeinit7.c || true
  
Tx2ExternalPathDelay:   
	rm -rf tmp.txt
	grep 'adi_adrv9001_cals_ExternalPathDelay_Set(adrv9001Device_0, ADI_CHANNEL_2,' $(PROJECT)/profile/initialize.c >> tmp.txt || true
	sed -i 's/error_code = adi_adrv9001_cals_ExternalPathDelay_Set(adrv9001Device_0, ADI_CHANNEL_2,/.Tx2ExternalPathDelay = /g' tmp.txt || true 
	sed -i 's/)\;/,/g' tmp.txt || true    
	grep '.Tx2ExternalPathDelay' tmp.txt >> $(PROJECT)/profile/initializeinit7.c || true     

Rx1AgcCfg:   
	rm -rf tmp.txt     
	echo '.Rx1Agc = {' >> tmp.txt
	grep -m 1 'adi_adrv9001_GainControlCfg_t' $(PROJECT)/profile/configureagcCfg* -A 66 | tail -n66 >> tmp.txt || true
	sed -i 's/}\;/ /g' tmp.txt
	echo '},' >> tmp.txt  
	cat tmp.txt >> $(PROJECT)/profile/initializeinit7.c 
  
Rx2AgcCfg:   
	rm -rf tmp.txt    
	echo '.Rx2Agc = {' >> tmp.txt
	grep -m 2 'adi_adrv9001_GainControlCfg_t' $(PROJECT)/profile/configureagcCfg* -A 66 | tail -n66 >> tmp.txt || true
	sed -i 's/}\;/ /g' tmp.txt   
	echo '},' >> tmp.txt  
	cat tmp.txt >> $(PROJECT)/profile/initializeinit7.c    
  
Tx1DpdInit:
	rm -rf tmp.txt
	echo '  .Tx1DpdInitCfg = {' >> tmp.txt 
	grep -m 1 'adi_adrv9001_DpdInitCfg_t' $(PROJECT)/profile/initialize.c -A 8 | tail -n8 >> tmp.txt 
	sed -i 's/}\;/ /g' tmp.txt     
	echo '},' >> tmp.txt  
	cat tmp.txt >> $(PROJECT)/profile/initializeinit7.c  

Tx2DpdInit:  
	rm -rf tmp.txt
	echo '  .Tx2DpdInitCfg = {' >> tmp.txt   
	grep -m 2 'adi_adrv9001_DpdInitCfg_t' $(PROJECT)/profile/initialize.c -A 8 | tail -n8 >> tmp.txt   
	sed -i 's/}\;/ /g' tmp.txt    
	echo '},' >> tmp.txt  
	cat tmp.txt >> $(PROJECT)/profile/initializeinit7.c  
  
Tx1DpdCfg:  
	rm -rf tmp.txt  
	echo '  .Tx1DpdCfg = {' >> tmp.txt
	grep -m 1 'adi_adrv9001_DpdCfg_t' $(PROJECT)/profile/configure.c -A 19 | tail -n19 >> tmp.txt   
	sed -i 's/}\;/ /g' tmp.txt    
	echo '},' >> tmp.txt  
	cat tmp.txt >> $(PROJECT)/profile/initializeinit7.c  
  
Tx2DpdCfg:   
	rm -rf tmp.txt  
	echo '  .Tx2DpdCfg = {' >> tmp.txt 
	grep -m 2 'adi_adrv9001_DpdCfg_t' $(PROJECT)/profile/configure.c -A 19 | tail -n19 >> tmp.txt  
	sed -i 's/}\;/ /g' tmp.txt   
	echo '},' >> tmp.txt  
	cat tmp.txt >> $(PROJECT)/profile/initializeinit7.c   
  
Rx1InterfaceGain:   
	rm -rf tmp.txt  
	echo '  .Rx1InterfaceGain = {' >> tmp.txt  
	grep -m 1 'adi_adrv9001_RxInterfaceGainCtrl_t' $(PROJECT)/profile/configure.c -A 8 | tail -n8 >> tmp.txt  
	sed -i 's/}\;/ /g' tmp.txt    
	echo '},' >> tmp.txt  
	cat tmp.txt >> $(PROJECT)/profile/initializeinit7.c    

Rx2InterfaceGain:  
	rm -rf tmp.txt  
	echo '  .Rx2InterfaceGain = {' >> tmp.txt 
	grep -m 2 'adi_adrv9001_RxInterfaceGainCtrl_t' $(PROJECT)/profile/configure.c -A 8 | tail -n8 >> tmp.txt  
	sed -i 's/}\;/ /g' tmp.txt    
	echo '},' >> tmp.txt  
	cat tmp.txt >> $(PROJECT)/profile/initializeinit7.c    
  
ParamsClose:
	sed -i 's/\;/,/g' $(PROJECT)/profile/initializeinit7.c
	echo '};' >> $(PROJECT)/profile/initializeinit7.c    

InitializeClean:  
	sed -i -e '/adi_fpga9001_SsiCalibrationCfg_t/,+8d' $(PROJECT)/profile/initialize.c      
	sed -i -e '/adi_fpga9001_ClockStatus_t/,+5d' $(PROJECT)/profile/initialize.c        
	sed -i -e '/adi_fpga9001_Version_t/,+3d' $(PROJECT)/profile/initialize.c  
	sed -i -e '/adi_adrv9001_Carrier_t/,+4d' $(PROJECT)/profile/initialize.c
	sed -i -e '/adi_adrv9001_DpdInitCfg_t/,+8d' $(PROJECT)/profile/initialize.c  
	sed -i 's/\&initialize_init_7/(adi_adrv9001_Init_t*)\&Adrv9001Params/g' $(PROJECT)/profile/initialize.c    
	sed -i 's/adi_adrv9001_Tx_OutputPowerBoost_Set(adrv9001Device_0, ADI_CHANNEL_1, .*/adi_adrv9001_Tx_OutputPowerBoost_Set(adrv9001Device_0, ADI_CHANNEL_1, Adrv9001Params.Tx1Boost);/g' $(PROJECT)/profile/initialize.c  
	sed -i 's/adi_adrv9001_Tx_OutputPowerBoost_Set(adrv9001Device_0, ADI_CHANNEL_2, .*/adi_adrv9001_Tx_OutputPowerBoost_Set(adrv9001Device_0, ADI_CHANNEL_2, Adrv9001Params.Tx2Boost);/g' $(PROJECT)/profile/initialize.c  
	sed -i 's/adi_adrv9001_Tx_Attenuation_Set(adrv9001Device_0, ADI_CHANNEL_1, .*/adi_adrv9001_Tx_Attenuation_Set(adrv9001Device_0, ADI_CHANNEL_1, \&Adrv9001Params.Tx1Attn);/g' $(PROJECT)/profile/initialize.c 
	sed -i 's/adi_adrv9001_Tx_Attenuation_Set(adrv9001Device_0, ADI_CHANNEL_2, .*/adi_adrv9001_Tx_Attenuation_Set(adrv9001Device_0, ADI_CHANNEL_2, \&Adrv9001Params.Tx2Attn);/g' $(PROJECT)/profile/initialize.c       
	sed -i 's/adi_adrv9001_Radio_Carrier_Configure(adrv9001Device_0, ADI_TX, ADI_CHANNEL_1, .*/adi_adrv9001_Radio_Carrier_Configure(adrv9001Device_0, ADI_TX, ADI_CHANNEL_1, \&Adrv9001Params.Lo1Carrier);/g' $(PROJECT)/profile/initialize.c  
	sed -i 's/adi_adrv9001_Radio_Carrier_Configure(adrv9001Device_0, ADI_TX, ADI_CHANNEL_2, .*/adi_adrv9001_Radio_Carrier_Configure(adrv9001Device_0, ADI_TX, ADI_CHANNEL_2, \&Adrv9001Params.Lo2Carrier);/g' $(PROJECT)/profile/initialize.c 
	sed -i 's/adi_adrv9001_Radio_Carrier_Configure(adrv9001Device_0, ADI_RX, ADI_CHANNEL_1, .*/adi_adrv9001_Radio_Carrier_Configure(adrv9001Device_0, ADI_RX, ADI_CHANNEL_1, \&Adrv9001Params.Lo1Carrier);/g' $(PROJECT)/profile/initialize.c 
	sed -i 's/adi_adrv9001_Radio_Carrier_Configure(adrv9001Device_0, ADI_RX, ADI_CHANNEL_2, .*/adi_adrv9001_Radio_Carrier_Configure(adrv9001Device_0, ADI_RX, ADI_CHANNEL_2, \&Adrv9001Params.Lo2Carrier);/g' $(PROJECT)/profile/initialize.c 
	sed -i 's/adi_adrv9001_dpd_Initial_Configure(adrv9001Device_0, ADI_CHANNEL_1, .*/adi_adrv9001_dpd_Initial_Configure(adrv9001Device_0, ADI_CHANNEL_1, \&Adrv9001Params.Tx1DpdInitCfg);/g' $(PROJECT)/profile/initialize.c 
	sed -i 's/adi_adrv9001_dpd_Initial_Configure(adrv9001Device_0, ADI_CHANNEL_2, .*/adi_adrv9001_dpd_Initial_Configure(adrv9001Device_0, ADI_CHANNEL_2, \&Adrv9001Params.Tx2DpdInitCfg);/g' $(PROJECT)/profile/initialize.c 	  
	sed -i 's/adi_adrv9001_cals_ExternalPathDelay_Set(adrv9001Device_0, ADI_CHANNEL_1, .*/adi_adrv9001_cals_ExternalPathDelay_Set(adrv9001Device_0, ADI_CHANNEL_1, \Adrv9001Params.Tx1ExternalPathDelay);/g' $(PROJECT)/profile/initialize.c      
	sed -i 's/adi_adrv9001_cals_ExternalPathDelay_Set(adrv9001Device_0, ADI_CHANNEL_2, .*/adi_adrv9001_cals_ExternalPathDelay_Set(adrv9001Device_0, ADI_CHANNEL_2, \Adrv9001Params.Tx2ExternalPathDelay);/g' $(PROJECT)/profile/initialize.c
	rm -rf $(PROJECT)/profile/configureagcCfg* || true	
  
ConfigureClean:
	sed -i -e '/adi_adrv9001_DpdCfg_t/,+19d' $(PROJECT)/profile/configure.c  
	sed -i -e '/adi_adrv9001_RxInterfaceGainCtrl_t/,+8d' $(PROJECT)/profile/configure.c    
	sed -i 's/adi_adrv9001_Rx_GainControl_Configure(adrv9001Device_0, ADI_CHANNEL_1, .*/adi_adrv9001_Rx_GainControl_Configure(adrv9001Device_0, ADI_CHANNEL_1, \&Adrv9001Params.Rx1Agc);/g' $(PROJECT)/profile/configure.c 
	sed -i 's/adi_adrv9001_Rx_GainControl_Configure(adrv9001Device_0, ADI_CHANNEL_2, .*/adi_adrv9001_Rx_GainControl_Configure(adrv9001Device_0, ADI_CHANNEL_2, \&Adrv9001Params.Rx2Agc);/g' $(PROJECT)/profile/configure.c 
	sed -i 's/adi_adrv9001_Tx_Attenuation_Set(adrv9001Device_0, ADI_CHANNEL_1, .*/adi_adrv9001_Tx_Attenuation_Set(adrv9001Device_0, ADI_CHANNEL_1, Adrv9001Params.Tx1Attn);/g' $(PROJECT)/profile/configure.c 
	sed -i 's/adi_adrv9001_Tx_Attenuation_Set(adrv9001Device_0, ADI_CHANNEL_2, .*/adi_adrv9001_Tx_Attenuation_Set(adrv9001Device_0, ADI_CHANNEL_2, Adrv9001Params.Tx2Attn);/g' $(PROJECT)/profile/configure.c    
	sed -i 's/adi_adrv9001_dpd_Configure(adrv9001Device_0, ADI_CHANNEL_1, .*/adi_adrv9001_dpd_Configure(adrv9001Device_0, ADI_CHANNEL_1, \&Adrv9001Params.Tx1DpdCfg);/g' $(PROJECT)/profile/configure.c 
	sed -i 's/adi_adrv9001_dpd_Configure(adrv9001Device_0, ADI_CHANNEL_2, .*/adi_adrv9001_dpd_Configure(adrv9001Device_0, ADI_CHANNEL_2, \&Adrv9001Params.Tx2DpdCfg);/g' $(PROJECT)/profile/configure.c      
	sed -i 's/adi_adrv9001_Rx_InterfaceGain_Configure(adrv9001Device_0, ADI_CHANNEL_1, .*/adi_adrv9001_Rx_InterfaceGain_Configure(adrv9001Device_0, ADI_CHANNEL_1, \&Adrv9001Params.Rx1InterfaceGain);/g' $(PROJECT)/profile/configure.c 
	sed -i 's/adi_adrv9001_Rx_InterfaceGain_Configure(adrv9001Device_0, ADI_CHANNEL_2, .*/adi_adrv9001_Rx_InterfaceGain_Configure(adrv9001Device_0, ADI_CHANNEL_2, \&Adrv9001Params.Rx2InterfaceGain);/g' $(PROJECT)/profile/configure.c      
        
clean:
	rm -rf $(PROJECT)  
