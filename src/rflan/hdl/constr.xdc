#set_property  -dict {PACKAGE_PIN D5    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dev_clk_in]     ; # IO_L7P_HDGC_AD5P_26
set_property  -dict {PACKAGE_PIN D8    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[0]]       ; # IO_L6N_HDGC_AD6N_26
set_property  -dict {PACKAGE_PIN E5    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[1]]       ; # IO_L3N_AD9N_26
set_property  -dict {PACKAGE_PIN D6    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[2]]       ; # IO_L5N_HDGC_AD7N_26
set_property  -dict {PACKAGE_PIN B7    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[3]]       ; # IO_L12P_AD0P_26
set_property  -dict {PACKAGE_PIN B5    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[4]]       ; # IO_L11N_AD1N_26
set_property  -dict {PACKAGE_PIN A6    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[5]]       ; # IO_L12N_AD0N_26
set_property  -dict {PACKAGE_PIN G5    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[6]]       ; # IO_L1N_AD11N_26
set_property  -dict {PACKAGE_PIN E6    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[7]]       ; # IO_L3P_AD9P_26
set_property  -dict {PACKAGE_PIN D7    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[8]]       ; # IO_L5P_HDGC_AD7P_26
set_property  -dict {PACKAGE_PIN C7    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[9]]       ; # IO_L8N_HDGC_AD4N_26
set_property  -dict {PACKAGE_PIN C8    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[10]]      ; # IO_L8P_HDGC_AD4P_26
set_property  -dict {PACKAGE_PIN B6    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[11]]      ; # IO_L11P_AD1P_26

# The following pins can be used instead of tx_ref_clk
#set_property  -dict {PACKAGE_PIN A3    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[12]]      ;# IO_L12N_T1U_N11_GC_66	
#set_property  -dict {PACKAGE_PIN A4    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[13]]      ;# IO_L12P_T1U_N10_GC_66	
#set_property  -dict {PACKAGE_PIN B1    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[14]]      ;# IO_L11N_T1U_N9_GC_66
#set_property  -dict {PACKAGE_PIN B2    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_dgpio[15]]      ;# IO_L11P_T1U_N8_GC_66

set_property  -dict {PACKAGE_PIN A9    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_irq]            ; # IO_L9N_AD3N_26
set_property  -dict {PACKAGE_PIN A8    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_rstn]           ; # IO_L10P_AD2P_26
set_property  -dict {PACKAGE_PIN A7    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_rx1_en]         ; # IO_L10N_AD2N_26
set_property  -dict {PACKAGE_PIN F6    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_rx2_en]         ; # IO_L2N_AD10N_26
set_property  -dict {PACKAGE_PIN B9    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_tx1_en]         ; # IO_L9P_AD3P_26
set_property  -dict {PACKAGE_PIN F7    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_tx2_en]         ; # IO_L4N_AD8N_26
set_property  -dict {PACKAGE_PIN E8    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_sclk]           ; # IO_L6P_HDGC_AD6P_26
set_property  -dict {PACKAGE_PIN G6    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_mosi]           ; # IO_L1P_AD11P_26
set_property  -dict {PACKAGE_PIN G7    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_miso]           ; # IO_L2P_AD10P_26
set_property  -dict {PACKAGE_PIN F8    IOSTANDARD LVCMOS18}                         [get_ports adrv9001_csn]            ; # IO_L4P_AD8P_26

set_property  -dict {PACKAGE_PIN K3     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx1_dclk_n]     ;# IO_L14N_T2L_N3_GC_65
set_property  -dict {PACKAGE_PIN K4     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx1_dclk_p]     ;# IO_L14P_T2L_N2_GC_65
set_property  -dict {PACKAGE_PIN H5     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx1_idata_n]    ;# IO_L16N_T2U_N7_QBC_AD3N_65
set_property  -dict {PACKAGE_PIN J5     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx1_idata_p]    ;# IO_L16P_T2U_N6_QBC_AD3P_65
set_property  -dict {PACKAGE_PIN J1     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx1_qdata_n]    ;# IO_L15N_T2L_N5_AD11N_65
set_property  -dict {PACKAGE_PIN K1     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx1_qdata_p]    ;# IO_L15P_T2L_N4_AD11P_65
set_property  -dict {PACKAGE_PIN G2     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx1_strobe_n]   ;# IO_L17N_T2U_N9_AD10N_65
set_property  -dict {PACKAGE_PIN H2     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx1_strobe_p]   ;# IO_L17P_T2U_N8_AD10P_65

set_property  -dict {PACKAGE_PIN J2     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx2_dclk_n]     ;# IO_L13N_T2L_N1_GC_QBC_65		
set_property  -dict {PACKAGE_PIN J3     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx2_dclk_p]     ;# IO_L13P_T2L_N0_GC_QBC_65
set_property  -dict {PACKAGE_PIN D1     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx2_idata_p]    ;# IO_L21N_T3L_N5_AD8N_65    
set_property  -dict {PACKAGE_PIN E1     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx2_idata_n]    ;# IO_L21P_T3L_N4_AD8P_65     
set_property  -dict {PACKAGE_PIN F1     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx2_qdata_p]    ;# IO_L19N_T3L_N1_DBC_AD9N_65  
set_property  -dict {PACKAGE_PIN G1     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx2_qdata_n]    ;# IO_L19P_T3L_N0_DBC_AD9P_65
set_property  -dict {PACKAGE_PIN E3     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx2_strobe_p]   ;# IO_L20N_T3L_N3_AD1N_65  
set_property  -dict {PACKAGE_PIN E4     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_rx2_strobe_n]   ;# IO_L20P_T3L_N2_AD1P_65     

set_property  -dict {PACKAGE_PIN A3     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_tx1_ref_clk_p]  ;# IO_L12N_T1U_N11_GC_66	
set_property  -dict {PACKAGE_PIN A4     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_tx1_ref_clk_n]  ;# IO_L12P_T1U_N10_GC_66	
set_property  -dict {PACKAGE_PIN G4     IOSTANDARD LVDS}                            [get_ports adrv9001_tx1_dclk_p]     ;# IO_L18N_T2U_N11_AD2N_65	
set_property  -dict {PACKAGE_PIN H4     IOSTANDARD LVDS}                            [get_ports adrv9001_tx1_dclk_n]     ;# IO_L18P_T2U_N10_AD2P_65	
set_property  -dict {PACKAGE_PIN F2     IOSTANDARD LVDS}                            [get_ports adrv9001_tx1_idata_p]    ;# IO_L23N_T3U_N9_65	      
set_property  -dict {PACKAGE_PIN F3     IOSTANDARD LVDS}                            [get_ports adrv9001_tx1_idata_n]    ;# IO_L23P_T3U_N8_I2C_SCLK_65	
set_property  -dict {PACKAGE_PIN C3     IOSTANDARD LVDS}                            [get_ports adrv9001_tx1_qdata_n]    ;# IO_L22N_T3U_N7_DBC_AD0N_65
set_property  -dict {PACKAGE_PIN D3     IOSTANDARD LVDS}                            [get_ports adrv9001_tx1_qdata_p]    ;# IO_L22P_T3U_N6_DBC_AD0P_65
set_property  -dict {PACKAGE_PIN C2     IOSTANDARD LVDS}                            [get_ports adrv9001_tx1_strobe_p]   ;# IO_L24N_T3U_N11_PERSTN0_65  
set_property  -dict {PACKAGE_PIN D2     IOSTANDARD LVDS}                            [get_ports adrv9001_tx1_strobe_n]   ;# IO_L24P_T3U_N10_PERSTN1_I2C 

set_property  -dict {PACKAGE_PIN B1     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_tx2_ref_clk_n]  ;# IO_L11N_T1U_N9_GC_66
set_property  -dict {PACKAGE_PIN B2     IOSTANDARD LVDS  DIFF_TERM_ADV TERM_100}    [get_ports adrv9001_tx2_ref_clk_p]  ;# IO_L11P_T1U_N8_GC_66
set_property  -dict {PACKAGE_PIN M1     IOSTANDARD LVDS}                            [get_ports adrv9001_tx2_dclk_n]     ;# IO_L9N_T1L_N5_AD12N_65
set_property  -dict {PACKAGE_PIN M2     IOSTANDARD LVDS}                            [get_ports adrv9001_tx2_dclk_p]     ;# IO_L9P_T1L_N4_AD12P_65
set_property  -dict {PACKAGE_PIN P1     IOSTANDARD LVDS}                            [get_ports adrv9001_tx2_idata_n]    ;# IO_L7N_T1L_N1_QBC_AD13N_65
set_property  -dict {PACKAGE_PIN N2     IOSTANDARD LVDS}                            [get_ports adrv9001_tx2_idata_p]    ;# IO_L7P_T1L_N0_QBC_AD13P_65
set_property  -dict {PACKAGE_PIN R5     IOSTANDARD LVDS}                            [get_ports adrv9001_tx2_qdata_n]    ;# IO_L6N_T0U_N11_AD6N_65
set_property  -dict {PACKAGE_PIN P5     IOSTANDARD LVDS}                            [get_ports adrv9001_tx2_qdata_p]    ;# IO_L6P_T0U_N10_AD6P_65
set_property  -dict {PACKAGE_PIN N4     IOSTANDARD LVDS}                            [get_ports adrv9001_tx2_strobe_n]   ;# IO_L8N_T1L_N3_AD5N_65
set_property  -dict {PACKAGE_PIN N5     IOSTANDARD LVDS}                            [get_ports adrv9001_tx2_strobe_p]   ;# IO_L8P_T1L_N2_AD5P_65

#set_property  -dict {PACKAGE_PIN T3     IOSTANDARD LVCMOS18}                        [get_ports pmoda[0]]                ;# IO_L1P_T0L_N0_DBC_65
#set_property  -dict {PACKAGE_PIN T2     IOSTANDARD LVCMOS18}                        [get_ports pmoda[1]]                ;# IO_L1N_T0L_N1_DBC_65
#set_property  -dict {PACKAGE_PIN P3     IOSTANDARD LVCMOS18}                        [get_ports pmoda[2]]                ;# IO_L2P_T0L_N2_65
#set_property  -dict {PACKAGE_PIN R3     IOSTANDARD LVCMOS18}                        [get_ports pmoda[3]]                ;# IO_L2N_T0L_N3_65
#set_property  -dict {PACKAGE_PIN U2     IOSTANDARD LVCMOS18}                        [get_ports pmoda[4]]                ;# IO_L3P_T0L_N4_AD15P_65
#set_property  -dict {PACKAGE_PIN U1     IOSTANDARD LVCMOS18}                        [get_ports pmoda[5]]                ;# IO_L3N_T0L_N5_AD15N_65
#set_property  -dict {PACKAGE_PIN R4     IOSTANDARD LVCMOS18}                        [get_ports pmoda[6]]                ;# IO_L4P_T0U_N6_DBC_AD7P_SMBALERT_65
#set_property  -dict {PACKAGE_PIN T4     IOSTANDARD LVCMOS18}                        [get_ports pmoda[7]]                ;# IO_L4N_T0U_N7_DBC_AD7N_65


#set_property  -dict {PACKAGE_PIN R1     IOSTANDARD LVCMOS18}                        [get_ports pmodb[0]]                ;## IO_L5P_T0U_N8_AD14P_65
#set_property  -dict {PACKAGE_PIN T1     IOSTANDARD LVCMOS18}                        [get_ports pmodb[1]]                ;## IO_L5N_T0U_N9_AD14N_65
#set_property  -dict {PACKAGE_PIN M5     IOSTANDARD LVCMOS18}                        [get_ports pmodb[2]]                ;## IO_L10P_T1U_N6_QBC_AD4P_65
#set_property  -dict {PACKAGE_PIN M4     IOSTANDARD LVCMOS18}                        [get_ports pmodb[3]]                ;## IO_L10N_T1U_N7_QBC_AD4N_65
#set_property  -dict {PACKAGE_PIN L2     IOSTANDARD LVCMOS18}                        [get_ports pmodb[4]]                ;## IO_L11P_T1U_N8_GC_65
#set_property  -dict {PACKAGE_PIN L1     IOSTANDARD LVCMOS18}                        [get_ports pmodb[5]]                ;## IO_L11N_T1U_N9_GC_65
#set_property  -dict {PACKAGE_PIN L4     IOSTANDARD LVCMOS18}                        [get_ports pmodb[6]]                ;## IO_L12P_T1U_N10_GC_65
#set_property  -dict {PACKAGE_PIN L3     IOSTANDARD LVCMOS18}                        [get_ports pmodb[7]]                ;## IO_L12N_T1U_N11_GC_65




# clocks

create_clock -name adrv9001_rx1_dclk_out   -period  2.034 [get_ports adrv9001_rx1_dclk_p]
create_clock -name adrv9001_rx2_dclk_out   -period  2.034 [get_ports adrv9001_rx2_dclk_p]
create_clock -name adrv9001_tx1_dclk_out   -period  2.034 [get_ports adrv9001_tx1_ref_clk_p]
create_clock -name adrv9001_tx2_dclk_out   -period  2.034 [get_ports adrv9001_tx2_ref_clk_p]

# Allow max skew of 0.5 ns between input clocks
set_clock_latency -source -early -0.25 [get_clocks adrv9001_rx1_dclk_out]
set_clock_latency -source -early -0.25 [get_clocks adrv9001_rx2_dclk_out]
set_clock_latency -source -late 0.25 [get_clocks adrv9001_rx1_dclk_out]
set_clock_latency -source -late 0.25 [get_clocks adrv9001_rx2_dclk_out]



