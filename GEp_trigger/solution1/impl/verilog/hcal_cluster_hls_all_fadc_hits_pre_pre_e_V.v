// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
`timescale 1 ns / 1 ps
module hcal_cluster_hls_all_fadc_hits_pre_pre_e_V_ram (addr0, ce0, d0, we0, q0,  clk);

parameter DWIDTH = 13;
parameter AWIDTH = 9;
parameter MEM_SIZE = 288;

input[AWIDTH-1:0] addr0;
input ce0;
input[DWIDTH-1:0] d0;
input we0;
output wire[DWIDTH-1:0] q0;
input clk;

reg [DWIDTH-1:0] ram[0:MEM_SIZE-1];
wire [AWIDTH-1:0] addr0_t0; 
reg [AWIDTH-1:0] addr0_t1; 
wire [DWIDTH-1:0] d0_t0; 
wire we0_t0; 
reg [DWIDTH-1:0] d0_t1; 
reg we0_t1; 
reg [DWIDTH-1:0] q0_t0;
reg [DWIDTH-1:0] q0_t1;

initial begin
    $readmemh("./hcal_cluster_hls_all_fadc_hits_pre_pre_e_V_ram.dat", ram);
end

assign addr0_t0 = addr0;
assign d0_t0 = d0;
assign we0_t0 = we0;
assign q0 = q0_t1;

always @(posedge clk)  
begin
    if (ce0) 
    begin
        addr0_t1 <= addr0_t0; 
        d0_t1 <= d0_t0;
        we0_t1 <= we0_t0;
        q0_t1 <= q0_t0;
    end
end


always @(posedge clk)  
begin 
    if (ce0) begin
        if (we0_t1) 
            ram[addr0_t1] <= d0_t1; 
        q0_t0 <= ram[addr0_t1];
    end
end


endmodule

`timescale 1 ns / 1 ps
module hcal_cluster_hls_all_fadc_hits_pre_pre_e_V(
    reset,
    clk,
    address0,
    ce0,
    we0,
    d0,
    q0);

parameter DataWidth = 32'd13;
parameter AddressRange = 32'd288;
parameter AddressWidth = 32'd9;
input reset;
input clk;
input[AddressWidth - 1:0] address0;
input ce0;
input we0;
input[DataWidth - 1:0] d0;
output[DataWidth - 1:0] q0;



hcal_cluster_hls_all_fadc_hits_pre_pre_e_V_ram hcal_cluster_hls_all_fadc_hits_pre_pre_e_V_ram_U(
    .clk( clk ),
    .addr0( address0 ),
    .ce0( ce0 ),
    .we0( we0 ),
    .d0( d0 ),
    .q0( q0 ));

endmodule

