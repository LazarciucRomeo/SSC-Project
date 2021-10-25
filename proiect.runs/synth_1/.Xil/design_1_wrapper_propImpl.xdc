set_property SRC_FILE_INFO {cfile:D:/UTC/AN3/SEM1/SSC/Proiect/proiect/proiect.srcs/constrs_1/new/basys3.xdc rfile:../../../proiect.srcs/constrs_1/new/basys3.xdc id:1} [current_design]
set_property SRC_FILE_INFO {cfile:D:/UTC/AN3/SEM1/SSC/Proiect/proiect/proiect.srcs/constrs_1/new/nexys4.xdc rfile:../../../proiect.srcs/constrs_1/new/nexys4.xdc id:2} [current_design]
set_property src_info {type:XDC file:1 line:12 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN V17 [get_ports {reset}]
set_property src_info {type:XDC file:1 line:267 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN B18 [get_ports rx_0]
set_property src_info {type:XDC file:1 line:269 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN A18 [get_ports tx_0]
set_property src_info {type:XDC file:2 line:13 export:INPUT save:INPUT read:READ} [current_design]
set_property -dict { PACKAGE_PIN J15   IOSTANDARD LVCMOS33 } [get_ports { reset }]; #IO_L24N_T3_RS0_15 Sch=sw[0]
set_property src_info {type:XDC file:2 line:219 export:INPUT save:INPUT read:READ} [current_design]
set_property -dict { PACKAGE_PIN C4    IOSTANDARD LVCMOS33 } [get_ports { tx_0 }]; #IO_L7P_T1_AD6P_35 Sch=uart_txd_in
set_property src_info {type:XDC file:2 line:220 export:INPUT save:INPUT read:READ} [current_design]
set_property -dict { PACKAGE_PIN D4    IOSTANDARD LVCMOS33 } [get_ports { rx_0 }]; #IO_L11N_T1_SRCC_35 Sch=uart_rxd_out
