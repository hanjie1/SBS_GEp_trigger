-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
-- Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity hcal_cluster_hls_fiber_map_V_rom is 
    generic(
             DWIDTH     : integer := 7; 
             AWIDTH     : integer := 9; 
             MEM_SIZE    : integer := 288
    ); 
    port (
          addr0      : in std_logic_vector(AWIDTH-1 downto 0); 
          ce0       : in std_logic; 
          q0         : out std_logic_vector(DWIDTH-1 downto 0);
          clk       : in std_logic
    ); 
end entity; 


architecture rtl of hcal_cluster_hls_fiber_map_V_rom is 

signal addr0_tmp : std_logic_vector(AWIDTH-1 downto 0); 
type mem_array is array (0 to MEM_SIZE-1) of std_logic_vector (DWIDTH-1 downto 0); 
signal mem : mem_array := (
    0 to 1=> "0000000", 2 to 3=> "0000001", 4 to 5=> "0000010", 6 to 7=> "0000011", 
    8 to 9=> "0000100", 10 to 11=> "0000101", 12 to 13=> "0000000", 14 to 15=> "0000001", 
    16 to 17=> "0000010", 18 to 19=> "0000011", 20 to 21=> "0000100", 22 to 23=> "0000101", 
    24 to 25=> "0000110", 26 to 27=> "0000111", 28 to 29=> "0001000", 30 to 31=> "0001001", 
    32 to 33=> "0001010", 34 to 35=> "0001011", 36 to 37=> "0000110", 38 to 39=> "0000111", 
    40 to 41=> "0001000", 42 to 43=> "0001001", 44 to 45=> "0001010", 46 to 47=> "0001011", 
    48 to 49=> "0001100", 50 to 51=> "0001101", 52 to 53=> "0001110", 54 to 55=> "0001111", 
    56 to 57=> "0010000", 58 to 59=> "0010001", 60 to 61=> "0001100", 62 to 63=> "0001101", 
    64 to 65=> "0001110", 66 to 67=> "0001111", 68 to 69=> "0010000", 70 to 71=> "0010001", 
    72 to 73=> "0010010", 74 to 75=> "0010011", 76 to 77=> "0010100", 78 to 79=> "0010101", 
    80 to 81=> "0010110", 82 to 83=> "0010111", 84 to 85=> "0010010", 86 to 87=> "0010011", 
    88 to 89=> "0010100", 90 to 91=> "0010101", 92 to 93=> "0010110", 94 to 95=> "0010111", 
    96 to 97=> "0011000", 98 to 99=> "0011001", 100 to 101=> "0011010", 102 to 103=> "0011011", 
    104 to 105=> "0011100", 106 to 107=> "0011101", 108 to 109=> "0011000", 110 to 111=> "0011001", 
    112 to 113=> "0011010", 114 to 115=> "0011011", 116 to 117=> "0011100", 118 to 119=> "0011101", 
    120 to 121=> "0011110", 122 to 123=> "0011111", 124 to 125=> "0100000", 126 to 127=> "0100001", 
    128 to 129=> "0100010", 130 to 131=> "0100011", 132 to 133=> "0011110", 134 to 135=> "0011111", 
    136 to 137=> "0100000", 138 to 139=> "0100001", 140 to 141=> "0100010", 142 to 143=> "0100011", 
    144 to 145=> "0100100", 146 to 147=> "0100101", 148 to 149=> "0100110", 150 to 151=> "0100111", 
    152 to 153=> "0101000", 154 to 155=> "0101001", 156 to 157=> "0100100", 158 to 159=> "0100101", 
    160 to 161=> "0100110", 162 to 163=> "0100111", 164 to 165=> "0101000", 166 to 167=> "0101001", 
    168 to 169=> "0101010", 170 to 171=> "0101011", 172 to 173=> "0101100", 174 to 175=> "0101101", 
    176 to 177=> "0101110", 178 to 179=> "0101111", 180 to 181=> "0101010", 182 to 183=> "0101011", 
    184 to 185=> "0101100", 186 to 187=> "0101101", 188 to 189=> "0101110", 190 to 191=> "0101111", 
    192 to 193=> "0110000", 194 to 195=> "0110001", 196 to 197=> "0110010", 198 to 199=> "0110011", 
    200 to 201=> "0110100", 202 to 203=> "0110101", 204 to 205=> "0110000", 206 to 207=> "0110001", 
    208 to 209=> "0110010", 210 to 211=> "0110011", 212 to 213=> "0110100", 214 to 215=> "0110101", 
    216 to 217=> "0110110", 218 to 219=> "0110111", 220 to 221=> "0111000", 222 to 223=> "0111001", 
    224 to 225=> "0111010", 226 to 227=> "0111011", 228 to 229=> "0110110", 230 to 231=> "0110111", 
    232 to 233=> "0111000", 234 to 235=> "0111001", 236 to 237=> "0111010", 238 to 239=> "0111011", 
    240 to 241=> "0111100", 242 to 243=> "0111101", 244 to 245=> "0111110", 246 to 247=> "0111111", 
    248 to 249=> "1000000", 250 to 251=> "1000001", 252 to 253=> "0111100", 254 to 255=> "0111101", 
    256 to 257=> "0111110", 258 to 259=> "0111111", 260 to 261=> "1000000", 262 to 263=> "1000001", 
    264 to 265=> "1000010", 266 to 267=> "1000011", 268 to 269=> "1000100", 270 to 271=> "1000101", 
    272 to 273=> "1000110", 274 to 275=> "1000111", 276 to 277=> "1000010", 278 to 279=> "1000011", 
    280 to 281=> "1000100", 282 to 283=> "1000101", 284 to 285=> "1000110", 286 to 287=> "1000111" );


begin 


memory_access_guard_0: process (addr0) 
begin
      addr0_tmp <= addr0;
--synthesis translate_off
      if (CONV_INTEGER(addr0) > mem_size-1) then
           addr0_tmp <= (others => '0');
      else 
           addr0_tmp <= addr0;
      end if;
--synthesis translate_on
end process;

p_rom_access: process (clk)  
begin 
    if (clk'event and clk = '1') then
        if (ce0 = '1') then 
            q0 <= mem(CONV_INTEGER(addr0_tmp)); 
        end if;
    end if;
end process;

end rtl;

Library IEEE;
use IEEE.std_logic_1164.all;

entity hcal_cluster_hls_fiber_map_V is
    generic (
        DataWidth : INTEGER := 7;
        AddressRange : INTEGER := 288;
        AddressWidth : INTEGER := 9);
    port (
        reset : IN STD_LOGIC;
        clk : IN STD_LOGIC;
        address0 : IN STD_LOGIC_VECTOR(AddressWidth - 1 DOWNTO 0);
        ce0 : IN STD_LOGIC;
        q0 : OUT STD_LOGIC_VECTOR(DataWidth - 1 DOWNTO 0));
end entity;

architecture arch of hcal_cluster_hls_fiber_map_V is
    component hcal_cluster_hls_fiber_map_V_rom is
        port (
            clk : IN STD_LOGIC;
            addr0 : IN STD_LOGIC_VECTOR;
            ce0 : IN STD_LOGIC;
            q0 : OUT STD_LOGIC_VECTOR);
    end component;



begin
    hcal_cluster_hls_fiber_map_V_rom_U :  component hcal_cluster_hls_fiber_map_V_rom
    port map (
        clk => clk,
        addr0 => address0,
        ce0 => ce0,
        q0 => q0);

end architecture;


