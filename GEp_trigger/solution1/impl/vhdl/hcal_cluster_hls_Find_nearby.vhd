-- ==============================================================
-- RTL generated by Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
-- Version: 2020.2
-- Copyright (C) Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
-- 
-- ===========================================================

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity hcal_cluster_hls_Find_nearby is
port (
    ap_clk : IN STD_LOGIC;
    ap_rst : IN STD_LOGIC;
    ap_start : IN STD_LOGIC;
    ap_done : OUT STD_LOGIC;
    ap_idle : OUT STD_LOGIC;
    ap_ready : OUT STD_LOGIC;
    ch : IN STD_LOGIC_VECTOR (8 downto 0);
    pos_r : IN STD_LOGIC_VECTOR (2 downto 0);
    ap_return : OUT STD_LOGIC_VECTOR (8 downto 0) );
end;


architecture behav of hcal_cluster_hls_Find_nearby is 
    constant ap_const_logic_1 : STD_LOGIC := '1';
    constant ap_const_logic_0 : STD_LOGIC := '0';
    constant ap_ST_fsm_state1 : STD_LOGIC_VECTOR (2 downto 0) := "001";
    constant ap_ST_fsm_state2 : STD_LOGIC_VECTOR (2 downto 0) := "010";
    constant ap_ST_fsm_state3 : STD_LOGIC_VECTOR (2 downto 0) := "100";
    constant ap_const_lv32_0 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000000";
    constant ap_const_lv32_1 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000001";
    constant ap_const_lv3_6 : STD_LOGIC_VECTOR (2 downto 0) := "110";
    constant ap_const_lv3_5 : STD_LOGIC_VECTOR (2 downto 0) := "101";
    constant ap_const_lv3_7 : STD_LOGIC_VECTOR (2 downto 0) := "111";
    constant ap_const_lv3_3 : STD_LOGIC_VECTOR (2 downto 0) := "011";
    constant ap_const_lv3_0 : STD_LOGIC_VECTOR (2 downto 0) := "000";
    constant ap_const_lv3_4 : STD_LOGIC_VECTOR (2 downto 0) := "100";
    constant ap_const_lv3_2 : STD_LOGIC_VECTOR (2 downto 0) := "010";
    constant ap_const_lv3_1 : STD_LOGIC_VECTOR (2 downto 0) := "001";
    constant ap_const_lv32_2 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000010";
    constant ap_const_boolean_0 : BOOLEAN := false;
    constant ap_const_lv4_1 : STD_LOGIC_VECTOR (3 downto 0) := "0001";
    constant ap_const_lv5_1F : STD_LOGIC_VECTOR (4 downto 0) := "11111";
    constant ap_const_lv5_1 : STD_LOGIC_VECTOR (4 downto 0) := "00001";
    constant ap_const_lv4_F : STD_LOGIC_VECTOR (3 downto 0) := "1111";
    constant ap_const_lv9_0 : STD_LOGIC_VECTOR (8 downto 0) := "000000000";
    constant ap_const_boolean_1 : BOOLEAN := true;

attribute shreg_extract : string;
    signal ap_CS_fsm : STD_LOGIC_VECTOR (2 downto 0) := "001";
    attribute fsm_encoding : string;
    attribute fsm_encoding of ap_CS_fsm : signal is "none";
    signal ap_CS_fsm_state1 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_state1 : signal is "none";
    signal block_map_nx_V_address0 : STD_LOGIC_VECTOR (8 downto 0);
    signal block_map_nx_V_ce0 : STD_LOGIC;
    signal block_map_nx_V_q0 : STD_LOGIC_VECTOR (4 downto 0);
    signal block_map_ny_V_address0 : STD_LOGIC_VECTOR (8 downto 0);
    signal block_map_ny_V_ce0 : STD_LOGIC;
    signal block_map_ny_V_q0 : STD_LOGIC_VECTOR (3 downto 0);
    signal grp_fu_117_p2 : STD_LOGIC_VECTOR (3 downto 0);
    signal reg_145 : STD_LOGIC_VECTOR (3 downto 0);
    signal ap_CS_fsm_state2 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_state2 : signal is "none";
    signal grp_fu_124_p2 : STD_LOGIC_VECTOR (4 downto 0);
    signal reg_150 : STD_LOGIC_VECTOR (4 downto 0);
    signal grp_fu_131_p2 : STD_LOGIC_VECTOR (4 downto 0);
    signal reg_155 : STD_LOGIC_VECTOR (4 downto 0);
    signal grp_fu_138_p2 : STD_LOGIC_VECTOR (3 downto 0);
    signal reg_160 : STD_LOGIC_VECTOR (3 downto 0);
    signal nx_V_reg_185 : STD_LOGIC_VECTOR (4 downto 0);
    signal ny_V_reg_190 : STD_LOGIC_VECTOR (3 downto 0);
    signal grp_Find_channel_fu_97_ap_start : STD_LOGIC;
    signal grp_Find_channel_fu_97_ap_done : STD_LOGIC;
    signal grp_Find_channel_fu_97_ap_idle : STD_LOGIC;
    signal grp_Find_channel_fu_97_ap_ready : STD_LOGIC;
    signal grp_Find_channel_fu_97_nx : STD_LOGIC_VECTOR (4 downto 0);
    signal grp_Find_channel_fu_97_ny : STD_LOGIC_VECTOR (3 downto 0);
    signal grp_Find_channel_fu_97_ap_return : STD_LOGIC_VECTOR (8 downto 0);
    signal ap_CS_fsm_state3 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_state3 : signal is "none";
    signal ap_block_state3_on_subcall_done : BOOLEAN;
    signal grp_Find_channel_fu_97_ap_start_reg : STD_LOGIC := '0';
    signal zext_ln534_fu_165_p1 : STD_LOGIC_VECTOR (63 downto 0);
    signal ap_return_preg : STD_LOGIC_VECTOR (8 downto 0) := "000000000";
    signal ap_NS_fsm : STD_LOGIC_VECTOR (2 downto 0);
    signal ap_ce_reg : STD_LOGIC;

    component hcal_cluster_hls_Find_channel IS
    port (
        ap_clk : IN STD_LOGIC;
        ap_rst : IN STD_LOGIC;
        ap_start : IN STD_LOGIC;
        ap_done : OUT STD_LOGIC;
        ap_idle : OUT STD_LOGIC;
        ap_ready : OUT STD_LOGIC;
        nx : IN STD_LOGIC_VECTOR (4 downto 0);
        ny : IN STD_LOGIC_VECTOR (3 downto 0);
        ap_return : OUT STD_LOGIC_VECTOR (8 downto 0) );
    end component;


    component hcal_cluster_hls_Find_channel_block_map_nx_V IS
    generic (
        DataWidth : INTEGER;
        AddressRange : INTEGER;
        AddressWidth : INTEGER );
    port (
        clk : IN STD_LOGIC;
        reset : IN STD_LOGIC;
        address0 : IN STD_LOGIC_VECTOR (8 downto 0);
        ce0 : IN STD_LOGIC;
        q0 : OUT STD_LOGIC_VECTOR (4 downto 0) );
    end component;


    component hcal_cluster_hls_Find_channel_block_map_ny_V IS
    generic (
        DataWidth : INTEGER;
        AddressRange : INTEGER;
        AddressWidth : INTEGER );
    port (
        clk : IN STD_LOGIC;
        reset : IN STD_LOGIC;
        address0 : IN STD_LOGIC_VECTOR (8 downto 0);
        ce0 : IN STD_LOGIC;
        q0 : OUT STD_LOGIC_VECTOR (3 downto 0) );
    end component;



begin
    block_map_nx_V_U : component hcal_cluster_hls_Find_channel_block_map_nx_V
    generic map (
        DataWidth => 5,
        AddressRange => 288,
        AddressWidth => 9)
    port map (
        clk => ap_clk,
        reset => ap_rst,
        address0 => block_map_nx_V_address0,
        ce0 => block_map_nx_V_ce0,
        q0 => block_map_nx_V_q0);

    block_map_ny_V_U : component hcal_cluster_hls_Find_channel_block_map_ny_V
    generic map (
        DataWidth => 4,
        AddressRange => 288,
        AddressWidth => 9)
    port map (
        clk => ap_clk,
        reset => ap_rst,
        address0 => block_map_ny_V_address0,
        ce0 => block_map_ny_V_ce0,
        q0 => block_map_ny_V_q0);

    grp_Find_channel_fu_97 : component hcal_cluster_hls_Find_channel
    port map (
        ap_clk => ap_clk,
        ap_rst => ap_rst,
        ap_start => grp_Find_channel_fu_97_ap_start,
        ap_done => grp_Find_channel_fu_97_ap_done,
        ap_idle => grp_Find_channel_fu_97_ap_idle,
        ap_ready => grp_Find_channel_fu_97_ap_ready,
        nx => grp_Find_channel_fu_97_nx,
        ny => grp_Find_channel_fu_97_ny,
        ap_return => grp_Find_channel_fu_97_ap_return);





    ap_CS_fsm_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_CS_fsm <= ap_ST_fsm_state1;
            else
                ap_CS_fsm <= ap_NS_fsm;
            end if;
        end if;
    end process;


    ap_return_preg_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_return_preg <= ap_const_lv9_0;
            else
                if (((ap_const_logic_1 = ap_CS_fsm_state3) and (ap_const_boolean_0 = ap_block_state3_on_subcall_done))) then 
                    ap_return_preg <= grp_Find_channel_fu_97_ap_return;
                end if; 
            end if;
        end if;
    end process;


    grp_Find_channel_fu_97_ap_start_reg_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                grp_Find_channel_fu_97_ap_start_reg <= ap_const_logic_0;
            else
                if ((((pos_r = ap_const_lv3_1) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_2) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_4) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_0) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_3) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_7) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_5) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_6) and (ap_const_logic_1 = ap_CS_fsm_state2)))) then 
                    grp_Find_channel_fu_97_ap_start_reg <= ap_const_logic_1;
                elsif ((grp_Find_channel_fu_97_ap_ready = ap_const_logic_1)) then 
                    grp_Find_channel_fu_97_ap_start_reg <= ap_const_logic_0;
                end if; 
            end if;
        end if;
    end process;

    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((ap_const_logic_1 = ap_CS_fsm_state2)) then
                nx_V_reg_185 <= block_map_nx_V_q0;
                ny_V_reg_190 <= block_map_ny_V_q0;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((((pos_r = ap_const_lv3_7) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_5) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_6) and (ap_const_logic_1 = ap_CS_fsm_state2)))) then
                reg_145 <= grp_fu_117_p2;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((((pos_r = ap_const_lv3_0) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_3) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_5) and (ap_const_logic_1 = ap_CS_fsm_state2)))) then
                reg_150 <= grp_fu_124_p2;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((((pos_r = ap_const_lv3_2) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_4) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_7) and (ap_const_logic_1 = ap_CS_fsm_state2)))) then
                reg_155 <= grp_fu_131_p2;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((((pos_r = ap_const_lv3_1) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_2) and (ap_const_logic_1 = ap_CS_fsm_state2)) or ((pos_r = ap_const_lv3_0) and (ap_const_logic_1 = ap_CS_fsm_state2)))) then
                reg_160 <= grp_fu_138_p2;
            end if;
        end if;
    end process;

    ap_NS_fsm_assign_proc : process (ap_start, ap_CS_fsm, ap_CS_fsm_state1, ap_CS_fsm_state3, ap_block_state3_on_subcall_done)
    begin
        case ap_CS_fsm is
            when ap_ST_fsm_state1 => 
                if (((ap_start = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_state1))) then
                    ap_NS_fsm <= ap_ST_fsm_state2;
                else
                    ap_NS_fsm <= ap_ST_fsm_state1;
                end if;
            when ap_ST_fsm_state2 => 
                ap_NS_fsm <= ap_ST_fsm_state3;
            when ap_ST_fsm_state3 => 
                if (((ap_const_logic_1 = ap_CS_fsm_state3) and (ap_const_boolean_0 = ap_block_state3_on_subcall_done))) then
                    ap_NS_fsm <= ap_ST_fsm_state1;
                else
                    ap_NS_fsm <= ap_ST_fsm_state3;
                end if;
            when others =>  
                ap_NS_fsm <= "XXX";
        end case;
    end process;
    ap_CS_fsm_state1 <= ap_CS_fsm(0);
    ap_CS_fsm_state2 <= ap_CS_fsm(1);
    ap_CS_fsm_state3 <= ap_CS_fsm(2);

    ap_block_state3_on_subcall_done_assign_proc : process(pos_r, grp_Find_channel_fu_97_ap_done)
    begin
                ap_block_state3_on_subcall_done <= (((grp_Find_channel_fu_97_ap_done = ap_const_logic_0) and (pos_r = ap_const_lv3_1)) or ((grp_Find_channel_fu_97_ap_done = ap_const_logic_0) and (pos_r = ap_const_lv3_2)) or ((grp_Find_channel_fu_97_ap_done = ap_const_logic_0) and (pos_r = ap_const_lv3_4)) or ((grp_Find_channel_fu_97_ap_done = ap_const_logic_0) and (pos_r = ap_const_lv3_0)) or ((grp_Find_channel_fu_97_ap_done = ap_const_logic_0) and (pos_r = ap_const_lv3_3)) or ((grp_Find_channel_fu_97_ap_done = ap_const_logic_0) and (pos_r = ap_const_lv3_7)) or ((grp_Find_channel_fu_97_ap_done = ap_const_logic_0) and (pos_r = ap_const_lv3_5)) or ((grp_Find_channel_fu_97_ap_done = ap_const_logic_0) and (pos_r = ap_const_lv3_6)));
    end process;


    ap_done_assign_proc : process(ap_start, ap_CS_fsm_state1, ap_CS_fsm_state3, ap_block_state3_on_subcall_done)
    begin
        if ((((ap_start = ap_const_logic_0) and (ap_const_logic_1 = ap_CS_fsm_state1)) or ((ap_const_logic_1 = ap_CS_fsm_state3) and (ap_const_boolean_0 = ap_block_state3_on_subcall_done)))) then 
            ap_done <= ap_const_logic_1;
        else 
            ap_done <= ap_const_logic_0;
        end if; 
    end process;


    ap_idle_assign_proc : process(ap_start, ap_CS_fsm_state1)
    begin
        if (((ap_start = ap_const_logic_0) and (ap_const_logic_1 = ap_CS_fsm_state1))) then 
            ap_idle <= ap_const_logic_1;
        else 
            ap_idle <= ap_const_logic_0;
        end if; 
    end process;


    ap_ready_assign_proc : process(ap_CS_fsm_state3, ap_block_state3_on_subcall_done)
    begin
        if (((ap_const_logic_1 = ap_CS_fsm_state3) and (ap_const_boolean_0 = ap_block_state3_on_subcall_done))) then 
            ap_ready <= ap_const_logic_1;
        else 
            ap_ready <= ap_const_logic_0;
        end if; 
    end process;


    ap_return_assign_proc : process(grp_Find_channel_fu_97_ap_return, ap_CS_fsm_state3, ap_block_state3_on_subcall_done, ap_return_preg)
    begin
        if (((ap_const_logic_1 = ap_CS_fsm_state3) and (ap_const_boolean_0 = ap_block_state3_on_subcall_done))) then 
            ap_return <= grp_Find_channel_fu_97_ap_return;
        else 
            ap_return <= ap_return_preg;
        end if; 
    end process;


    block_map_nx_V_address0_assign_proc : process(ap_CS_fsm_state1, pos_r, ap_CS_fsm_state3, zext_ln534_fu_165_p1)
    begin
        if ((ap_const_logic_1 = ap_CS_fsm_state1)) then 
            block_map_nx_V_address0 <= zext_ln534_fu_165_p1(9 - 1 downto 0);
        elsif ((((pos_r = ap_const_lv3_1) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_2) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_4) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_0) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_3) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_7) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_5) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_6) and (ap_const_logic_1 = ap_CS_fsm_state3)))) then 
            block_map_nx_V_address0 <= ap_const_lv9_0;
        else 
            block_map_nx_V_address0 <= "XXXXXXXXX";
        end if; 
    end process;


    block_map_nx_V_ce0_assign_proc : process(ap_start, ap_CS_fsm_state1, pos_r, ap_CS_fsm_state3)
    begin
        if (((ap_start = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_state1))) then 
            block_map_nx_V_ce0 <= ap_const_logic_1;
        elsif ((((pos_r = ap_const_lv3_1) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_2) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_4) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_0) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_3) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_7) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_5) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_6) and (ap_const_logic_1 = ap_CS_fsm_state3)))) then 
            block_map_nx_V_ce0 <= ap_const_logic_0;
        else 
            block_map_nx_V_ce0 <= ap_const_logic_0;
        end if; 
    end process;


    block_map_ny_V_address0_assign_proc : process(ap_CS_fsm_state1, pos_r, ap_CS_fsm_state3, zext_ln534_fu_165_p1)
    begin
        if ((ap_const_logic_1 = ap_CS_fsm_state1)) then 
            block_map_ny_V_address0 <= zext_ln534_fu_165_p1(9 - 1 downto 0);
        elsif ((((pos_r = ap_const_lv3_1) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_2) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_4) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_0) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_3) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_7) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_5) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_6) and (ap_const_logic_1 = ap_CS_fsm_state3)))) then 
            block_map_ny_V_address0 <= ap_const_lv9_0;
        else 
            block_map_ny_V_address0 <= "XXXXXXXXX";
        end if; 
    end process;


    block_map_ny_V_ce0_assign_proc : process(ap_start, ap_CS_fsm_state1, pos_r, ap_CS_fsm_state3)
    begin
        if (((ap_start = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_state1))) then 
            block_map_ny_V_ce0 <= ap_const_logic_1;
        elsif ((((pos_r = ap_const_lv3_1) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_2) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_4) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_0) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_3) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_7) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_5) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_6) and (ap_const_logic_1 = ap_CS_fsm_state3)))) then 
            block_map_ny_V_ce0 <= ap_const_logic_0;
        else 
            block_map_ny_V_ce0 <= ap_const_logic_0;
        end if; 
    end process;

    grp_Find_channel_fu_97_ap_start <= grp_Find_channel_fu_97_ap_start_reg;

    grp_Find_channel_fu_97_nx_assign_proc : process(pos_r, reg_150, reg_155, nx_V_reg_185, ap_CS_fsm_state3)
    begin
        if ((((pos_r = ap_const_lv3_2) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_4) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_7) and (ap_const_logic_1 = ap_CS_fsm_state3)))) then 
            grp_Find_channel_fu_97_nx <= reg_155;
        elsif ((((pos_r = ap_const_lv3_0) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_3) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_5) and (ap_const_logic_1 = ap_CS_fsm_state3)))) then 
            grp_Find_channel_fu_97_nx <= reg_150;
        elsif ((((pos_r = ap_const_lv3_1) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_6) and (ap_const_logic_1 = ap_CS_fsm_state3)))) then 
            grp_Find_channel_fu_97_nx <= nx_V_reg_185;
        else 
            grp_Find_channel_fu_97_nx <= "XXXXX";
        end if; 
    end process;


    grp_Find_channel_fu_97_ny_assign_proc : process(pos_r, reg_145, reg_160, ny_V_reg_190, ap_CS_fsm_state3)
    begin
        if ((((pos_r = ap_const_lv3_1) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_2) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_0) and (ap_const_logic_1 = ap_CS_fsm_state3)))) then 
            grp_Find_channel_fu_97_ny <= reg_160;
        elsif ((((pos_r = ap_const_lv3_4) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_3) and (ap_const_logic_1 = ap_CS_fsm_state3)))) then 
            grp_Find_channel_fu_97_ny <= ny_V_reg_190;
        elsif ((((pos_r = ap_const_lv3_7) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_5) and (ap_const_logic_1 = ap_CS_fsm_state3)) or ((pos_r = ap_const_lv3_6) and (ap_const_logic_1 = ap_CS_fsm_state3)))) then 
            grp_Find_channel_fu_97_ny <= reg_145;
        else 
            grp_Find_channel_fu_97_ny <= "XXXX";
        end if; 
    end process;

    grp_fu_117_p2 <= std_logic_vector(unsigned(block_map_ny_V_q0) + unsigned(ap_const_lv4_1));
    grp_fu_124_p2 <= std_logic_vector(unsigned(block_map_nx_V_q0) + unsigned(ap_const_lv5_1F));
    grp_fu_131_p2 <= std_logic_vector(unsigned(block_map_nx_V_q0) + unsigned(ap_const_lv5_1));
    grp_fu_138_p2 <= std_logic_vector(unsigned(block_map_ny_V_q0) + unsigned(ap_const_lv4_F));
    zext_ln534_fu_165_p1 <= std_logic_vector(IEEE.numeric_std.resize(unsigned(ch),64));
end behav;