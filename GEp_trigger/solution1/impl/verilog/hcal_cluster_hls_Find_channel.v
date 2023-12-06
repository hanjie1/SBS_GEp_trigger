// ==============================================================
// RTL generated by Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Version: 2020.2
// Copyright (C) Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

`timescale 1 ns / 1 ps 

module hcal_cluster_hls_Find_channel (
        ap_clk,
        ap_rst,
        ap_start,
        ap_done,
        ap_idle,
        ap_ready,
        nx,
        ny,
        ap_return
);

parameter    ap_ST_fsm_state1 = 5'd1;
parameter    ap_ST_fsm_state2 = 5'd2;
parameter    ap_ST_fsm_state3 = 5'd4;
parameter    ap_ST_fsm_state4 = 5'd8;
parameter    ap_ST_fsm_state5 = 5'd16;

input   ap_clk;
input   ap_rst;
input   ap_start;
output   ap_done;
output   ap_idle;
output   ap_ready;
input  [4:0] nx;
input  [3:0] ny;
output  [8:0] ap_return;

reg ap_done;
reg ap_idle;
reg ap_ready;
reg[8:0] ap_return;

(* fsm_encoding = "none" *) reg   [4:0] ap_CS_fsm;
wire    ap_CS_fsm_state1;
wire   [8:0] block_map_nx_V_address0;
reg    block_map_nx_V_ce0;
wire   [4:0] block_map_nx_V_q0;
wire   [8:0] block_map_ny_V_address0;
reg    block_map_ny_V_ce0;
wire   [3:0] block_map_ny_V_q0;
wire   [0:0] icmp_ln166_fu_139_p2;
reg   [0:0] icmp_ln166_reg_198;
wire   [0:0] icmp_ln166_1_fu_155_p2;
reg   [0:0] icmp_ln166_1_reg_202;
wire   [0:0] icmp_ln878_fu_161_p2;
reg   [0:0] icmp_ln878_reg_206;
wire    ap_CS_fsm_state2;
wire   [8:0] ich_V_1_fu_167_p2;
reg   [8:0] ich_V_1_reg_210;
wire   [63:0] zext_ln534_fu_173_p1;
reg   [63:0] zext_ln534_reg_215;
wire   [0:0] icmp_ln870_fu_178_p2;
reg   [0:0] icmp_ln870_reg_225;
wire    ap_CS_fsm_state3;
reg   [8:0] ch_V_reg_90;
wire    ap_CS_fsm_state4;
wire   [0:0] icmp_ln870_1_fu_183_p2;
reg   [8:0] this_V_write_assign_ph_reg_102;
reg   [8:0] ap_phi_mux_this_V_write_assign_phi_fu_118_p6;
reg   [8:0] this_V_write_assign_reg_114;
wire    ap_CS_fsm_state5;
wire   [5:0] zext_ln166_fu_129_p1;
wire   [5:0] add_ln166_fu_133_p2;
wire   [4:0] zext_ln166_1_fu_145_p1;
wire   [4:0] add_ln166_1_fu_149_p2;
reg   [8:0] ap_return_preg;
reg   [4:0] ap_NS_fsm;
wire    ap_ce_reg;

// power-on initialization
initial begin
#0 ap_CS_fsm = 5'd1;
#0 ap_return_preg = 9'd0;
end

hcal_cluster_hls_Find_channel_block_map_nx_V #(
    .DataWidth( 5 ),
    .AddressRange( 288 ),
    .AddressWidth( 9 ))
block_map_nx_V_U(
    .clk(ap_clk),
    .reset(ap_rst),
    .address0(block_map_nx_V_address0),
    .ce0(block_map_nx_V_ce0),
    .q0(block_map_nx_V_q0)
);

hcal_cluster_hls_Find_channel_block_map_ny_V #(
    .DataWidth( 4 ),
    .AddressRange( 288 ),
    .AddressWidth( 9 ))
block_map_ny_V_U(
    .clk(ap_clk),
    .reset(ap_rst),
    .address0(block_map_ny_V_address0),
    .ce0(block_map_ny_V_ce0),
    .q0(block_map_ny_V_q0)
);

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_CS_fsm <= ap_ST_fsm_state1;
    end else begin
        ap_CS_fsm <= ap_NS_fsm;
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_return_preg <= 9'd0;
    end else begin
        if ((1'b1 == ap_CS_fsm_state5)) begin
            ap_return_preg <= ap_phi_mux_this_V_write_assign_phi_fu_118_p6;
        end
    end
end

always @ (posedge ap_clk) begin
    if (((icmp_ln166_1_fu_155_p2 == 1'd0) & (icmp_ln166_fu_139_p2 == 1'd0) & (1'b1 == ap_CS_fsm_state1) & (ap_start == 1'b1))) begin
        ch_V_reg_90 <= 9'd0;
    end else if (((1'b1 == ap_CS_fsm_state4) & (((icmp_ln870_reg_225 == 1'd0) & (icmp_ln878_reg_206 == 1'd0)) | ((icmp_ln870_1_fu_183_p2 == 1'd0) & (icmp_ln878_reg_206 == 1'd0))))) begin
        ch_V_reg_90 <= ich_V_1_reg_210;
    end
end

always @ (posedge ap_clk) begin
    if (((icmp_ln870_1_fu_183_p2 == 1'd1) & (icmp_ln870_reg_225 == 1'd1) & (icmp_ln878_reg_206 == 1'd0) & (1'b1 == ap_CS_fsm_state4))) begin
        this_V_write_assign_ph_reg_102 <= ch_V_reg_90;
    end else if (((icmp_ln878_fu_161_p2 == 1'd1) & (1'b1 == ap_CS_fsm_state2))) begin
        this_V_write_assign_ph_reg_102 <= 9'd511;
    end
end

always @ (posedge ap_clk) begin
    if (((icmp_ln166_1_reg_202 == 1'd0) & (icmp_ln166_reg_198 == 1'd0) & (1'b1 == ap_CS_fsm_state5))) begin
        this_V_write_assign_reg_114 <= this_V_write_assign_ph_reg_102;
    end else if ((((icmp_ln166_1_fu_155_p2 == 1'd1) & (icmp_ln166_fu_139_p2 == 1'd0) & (1'b1 == ap_CS_fsm_state1) & (ap_start == 1'b1)) | ((icmp_ln166_fu_139_p2 == 1'd1) & (1'b1 == ap_CS_fsm_state1) & (ap_start == 1'b1)))) begin
        this_V_write_assign_reg_114 <= 9'd511;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state2)) begin
        ich_V_1_reg_210 <= ich_V_1_fu_167_p2;
        icmp_ln878_reg_206 <= icmp_ln878_fu_161_p2;
    end
end

always @ (posedge ap_clk) begin
    if (((icmp_ln166_fu_139_p2 == 1'd0) & (1'b1 == ap_CS_fsm_state1))) begin
        icmp_ln166_1_reg_202 <= icmp_ln166_1_fu_155_p2;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state1)) begin
        icmp_ln166_reg_198 <= icmp_ln166_fu_139_p2;
    end
end

always @ (posedge ap_clk) begin
    if (((icmp_ln878_reg_206 == 1'd0) & (1'b1 == ap_CS_fsm_state3))) begin
        icmp_ln870_reg_225 <= icmp_ln870_fu_178_p2;
    end
end

always @ (posedge ap_clk) begin
    if (((icmp_ln878_fu_161_p2 == 1'd0) & (1'b1 == ap_CS_fsm_state2))) begin
        zext_ln534_reg_215[8 : 0] <= zext_ln534_fu_173_p1[8 : 0];
    end
end

always @ (*) begin
    if (((1'b1 == ap_CS_fsm_state5) | ((1'b1 == ap_CS_fsm_state1) & (ap_start == 1'b0)))) begin
        ap_done = 1'b1;
    end else begin
        ap_done = 1'b0;
    end
end

always @ (*) begin
    if (((1'b1 == ap_CS_fsm_state1) & (ap_start == 1'b0))) begin
        ap_idle = 1'b1;
    end else begin
        ap_idle = 1'b0;
    end
end

always @ (*) begin
    if (((icmp_ln166_1_reg_202 == 1'd0) & (icmp_ln166_reg_198 == 1'd0) & (1'b1 == ap_CS_fsm_state5))) begin
        ap_phi_mux_this_V_write_assign_phi_fu_118_p6 = this_V_write_assign_ph_reg_102;
    end else begin
        ap_phi_mux_this_V_write_assign_phi_fu_118_p6 = this_V_write_assign_reg_114;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state5)) begin
        ap_ready = 1'b1;
    end else begin
        ap_ready = 1'b0;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state5)) begin
        ap_return = ap_phi_mux_this_V_write_assign_phi_fu_118_p6;
    end else begin
        ap_return = ap_return_preg;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state2)) begin
        block_map_nx_V_ce0 = 1'b1;
    end else begin
        block_map_nx_V_ce0 = 1'b0;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state3)) begin
        block_map_ny_V_ce0 = 1'b1;
    end else begin
        block_map_ny_V_ce0 = 1'b0;
    end
end

always @ (*) begin
    case (ap_CS_fsm)
        ap_ST_fsm_state1 : begin
            if (((icmp_ln166_1_fu_155_p2 == 1'd0) & (icmp_ln166_fu_139_p2 == 1'd0) & (1'b1 == ap_CS_fsm_state1) & (ap_start == 1'b1))) begin
                ap_NS_fsm = ap_ST_fsm_state2;
            end else if (((1'b1 == ap_CS_fsm_state1) & (ap_start == 1'b1) & ((icmp_ln166_1_fu_155_p2 == 1'd1) | (icmp_ln166_fu_139_p2 == 1'd1)))) begin
                ap_NS_fsm = ap_ST_fsm_state5;
            end else begin
                ap_NS_fsm = ap_ST_fsm_state1;
            end
        end
        ap_ST_fsm_state2 : begin
            ap_NS_fsm = ap_ST_fsm_state3;
        end
        ap_ST_fsm_state3 : begin
            ap_NS_fsm = ap_ST_fsm_state4;
        end
        ap_ST_fsm_state4 : begin
            if (((1'b1 == ap_CS_fsm_state4) & (((icmp_ln870_reg_225 == 1'd0) & (icmp_ln878_reg_206 == 1'd0)) | ((icmp_ln870_1_fu_183_p2 == 1'd0) & (icmp_ln878_reg_206 == 1'd0))))) begin
                ap_NS_fsm = ap_ST_fsm_state2;
            end else begin
                ap_NS_fsm = ap_ST_fsm_state5;
            end
        end
        ap_ST_fsm_state5 : begin
            ap_NS_fsm = ap_ST_fsm_state1;
        end
        default : begin
            ap_NS_fsm = 'bx;
        end
    endcase
end

assign add_ln166_1_fu_149_p2 = ($signed(zext_ln166_1_fu_145_p1) + $signed(5'd31));

assign add_ln166_fu_133_p2 = ($signed(zext_ln166_fu_129_p1) + $signed(6'd63));

assign ap_CS_fsm_state1 = ap_CS_fsm[32'd0];

assign ap_CS_fsm_state2 = ap_CS_fsm[32'd1];

assign ap_CS_fsm_state3 = ap_CS_fsm[32'd2];

assign ap_CS_fsm_state4 = ap_CS_fsm[32'd3];

assign ap_CS_fsm_state5 = ap_CS_fsm[32'd4];

assign block_map_nx_V_address0 = zext_ln534_fu_173_p1;

assign block_map_ny_V_address0 = zext_ln534_reg_215;

assign ich_V_1_fu_167_p2 = (ch_V_reg_90 + 9'd1);

assign icmp_ln166_1_fu_155_p2 = ((add_ln166_1_fu_149_p2 > 5'd11) ? 1'b1 : 1'b0);

assign icmp_ln166_fu_139_p2 = ((add_ln166_fu_133_p2 > 6'd23) ? 1'b1 : 1'b0);

assign icmp_ln870_1_fu_183_p2 = ((block_map_ny_V_q0 == ny) ? 1'b1 : 1'b0);

assign icmp_ln870_fu_178_p2 = ((block_map_nx_V_q0 == nx) ? 1'b1 : 1'b0);

assign icmp_ln878_fu_161_p2 = ((ch_V_reg_90 == 9'd288) ? 1'b1 : 1'b0);

assign zext_ln166_1_fu_145_p1 = ny;

assign zext_ln166_fu_129_p1 = nx;

assign zext_ln534_fu_173_p1 = ch_V_reg_90;

always @ (posedge ap_clk) begin
    zext_ln534_reg_215[63:9] <= 55'b0000000000000000000000000000000000000000000000000000000;
end

endmodule //hcal_cluster_hls_Find_channel