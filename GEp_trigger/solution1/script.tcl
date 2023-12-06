############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
############################################################
open_project GEp_trigger
set_top hcal_cluster_hls
add_files hcal_cluster_hls.cpp
add_files -tb hcal_cluster_hls_tb.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution1" -flow_target vivado
set_part {xc7vx550t-ffg1927-1}
create_clock -period 32 -name default
#source "./GEp_trigger/solution1/directives.tcl"
csim_design -profile
csynth_design
cosim_design
export_design -format ip_catalog
