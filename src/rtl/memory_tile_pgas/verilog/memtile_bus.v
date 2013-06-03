// Based on:
// WISHBONE Connection Bus Top Level		                 
//  Author: Johny Chi, chisuhua@yahoo.com.cn
//
// Modified for the Wishbone Bus Generator
//  Author: Stefan Wallentowitz, stefan.wallentowitz@tum.de
//
// Copyright (C) 2000, 2011 Authors and OPENCORES.ORG
//
// This source file may be used and distributed without         
// restriction provided that this copyright statement is not    
// removed from the file and that any derivative work contains  
// the original copyright notice and the associated disclaimer. 
//                                                              
// This source file is free software; you can redistribute it   
// and/or modify it under the terms of the GNU Lesser General   
// Public License as published by the Free Software Foundation; 
// either version 2.1 of the License, or (at your option) any   
// later version.                                               
//                                                              
// This source is distributed in the hope that it will be       
// useful, but WITHOUT ANY WARRANTY; without even the implied   
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      
// PURPOSE.  See the GNU Lesser General Public License for more 
// details.                                                     
//                                                              
// You should have received a copy of the GNU Lesser General    
// Public License along with this source; if not, download it   
// from http://www.opencores.org/lgpl.shtml


// Automatically generated on: 28-11-2012 11:50:51
// Parameters:
// Masters:
//   [0]
//   [1]
// Slaves:
//   [0] 0x00000000-0x7fffffff



module memtile_bus( 
  m_0_dat_o, m_0_ack_o, m_0_err_o, m_0_rty_o,
  m_0_dat_i, m_0_adr_i, m_0_sel_i, m_0_we_i, m_0_cyc_i, m_0_stb_i, m_0_cab_i, 
  m_1_dat_o, m_1_ack_o, m_1_err_o, m_1_rty_o,
  m_1_dat_i, m_1_adr_i, m_1_sel_i, m_1_we_i, m_1_cyc_i, m_1_stb_i, m_1_cab_i, 
  s_0_dat_i, s_0_ack_i, s_0_err_i, s_0_rty_i,
  s_0_dat_o, s_0_adr_o, s_0_sel_o, s_0_we_o, s_0_cyc_o, s_0_stb_o, s_0_cab_o,
  clk_i, rst_i
);
////////////////////////////////////////////////////////////////////
//
// Module Parameters

   parameter dw = 32;
   parameter aw = 32;
   parameter sw = 4;


   parameter		cached_addr_1l = 32'h0000_0000;		// cacheable address range1: lower bound
   parameter		cached_addr_1w = 4 ;		        // fixed width MSB of cacheable address range1: 32'h0000_0000 ~ 32'h1fff_ffff
   parameter		cached_addr_2l = 32'h0010_0000;	        // cacheable address range2: lower bound
   parameter		cached_addr_2w = 12 ;		        // fixed width MSB of cacheable address range2: 32'h0010_0000 ~ 32'h001f_ffff
   parameter		cached_addr_3l = 32'h0500_0000;	        // cacheable address range3: lower bound
   parameter		cached_addr_3w = 8 ;		        // fixed width MSB of cacheable address range3: 32'h0500_0000 ~ 32'h05ff_ffff

  // DON'T CHANGE (AUTO-GENERATED)
  parameter s0_addr_w = 1;
  parameter s0_addr   = 1'd0;
 
   // Don't change
   parameter mbusw = aw + sw + dw +4;
   parameter sbusw = 3;
   parameter mselectw = 2;
   parameter sselectw = 1;
   parameter gw       = 1;

////////////////////////////////////////////////////////////////////
//
// Module IOs
//
   input		clk_i, rst_i;
   
  input [dw-1:0] m_0_dat_i;
   output [dw-1:0] 	m_0_dat_o;
   input [aw-1:0] 	m_0_adr_i;
   input [sw-1:0] 	m_0_sel_i;
   input 		m_0_we_i;
   input 		m_0_cyc_i;
   input 		m_0_stb_i;
   input 		m_0_cab_i;
   output 		m_0_ack_o;
   output 		m_0_err_o;
   output 		m_0_rty_o;
  input [dw-1:0] m_1_dat_i;
   output [dw-1:0] 	m_1_dat_o;
   input [aw-1:0] 	m_1_adr_i;
   input [sw-1:0] 	m_1_sel_i;
   input 		m_1_we_i;
   input 		m_1_cyc_i;
   input 		m_1_stb_i;
   input 		m_1_cab_i;
   output 		m_1_ack_o;
   output 		m_1_err_o;
   output 		m_1_rty_o;


  input [dw-1:0] s_0_dat_i;
  output [dw-1:0] 	s_0_dat_o;
  output [aw-1:0] 	s_0_adr_o;
  output [sw-1:0] 	s_0_sel_o;
  output 		s_0_we_o;
  output 		s_0_cyc_o;
  output 		s_0_stb_o;
  output 		s_0_cab_o;
  input 		s_0_ack_i;
  input 		s_0_err_i;
  input 		s_0_rty_i;




////////////////////////////////////////////////////////////////////
//
// Local wires
//

  wire  [mselectw -1:0] i_gnt_arb;
  wire  [gw-1:0]        gnt;
  reg   [sselectw -1:0] i_ssel_dec;
  reg	[mbusw -1:0]    i_bus_m;
  wire  [dw -1:0]       i_dat_s;
  wire  [sbusw -1:0]    i_bus_s;

  wire    bus_addr_cacheable;
  assign  bus_addr_cacheable = (i_bus_m[mbusw -1:mbusw - cached_addr_1w] == cached_addr_1l[aw - 1: aw - cached_addr_1w]) 
                          || (i_bus_m[mbusw -1:mbusw - cached_addr_2w] == cached_addr_2l[aw - 1: aw - cached_addr_2w]) 
                          || (i_bus_m[mbusw -1:mbusw - cached_addr_3w] == cached_addr_3l[aw - 1: aw - cached_addr_3w]);




  assign m_0_dat_o = i_dat_s;
  assign  {m_0_ack_o, m_0_err_o, m_0_rty_o} = i_bus_s & {3{i_gnt_arb[0]}};

  assign m_1_dat_o = i_dat_s;
  assign  {m_1_ack_o, m_1_err_o, m_1_rty_o} = i_bus_s & {3{i_gnt_arb[1]}};


assign i_bus_s = { s_0_ack_i,
                   s_0_err_i,
                   s_0_rty_i };


assign {s_0_adr_o, s_0_sel_o, s_0_dat_o, s_0_we_o, s_0_cab_o, s_0_cyc_o} = i_bus_m[mbusw -1:1];
assign s_0_stb_o = i_bus_m[1] & i_bus_m[0] & i_ssel_dec[0]; 


always @(*)
  case(gnt)
  1'd0:  i_bus_m = {m_0_adr_i, m_0_sel_i, m_0_dat_i, m_0_we_i, m_0_cab_i, m_0_cyc_i, m_0_stb_i};
  1'd1:  i_bus_m = {m_1_adr_i, m_1_sel_i, m_1_dat_i, m_1_we_i, m_1_cab_i, m_1_cyc_i, m_1_stb_i};

    default:i_bus_m =  72'b0;
endcase			

assign i_dat_s = i_ssel_dec[0] ? s_0_dat_i : {dw{1'b0}}; 

assign i_gnt_arb[0] = (gnt == 1'd0);
assign i_gnt_arb[1] = (gnt == 1'd1);


memtile_bus_arbiter u_memtile_bus_arbiter (
  .clk(clk_i), 
  .rst(rst_i),
  .req({m_1_cyc_i, m_0_cyc_i}),
  .gnt(gnt)
);

wire [7:0]	m_0_ssel_dec;
wire [7:0]	m_1_ssel_dec;


always @(*)
  case(gnt)
    1'd0: i_ssel_dec = m_0_ssel_dec;
    1'd1: i_ssel_dec = m_1_ssel_dec;

    default: i_ssel_dec = 7'b0;
  endcase

assign m_0_ssel_dec[0] = (m_0_adr_i[aw -1 : aw - s0_addr_w ] == s0_addr);

assign m_1_ssel_dec[0] = (m_1_adr_i[aw -1 : aw - s0_addr_w ] == s0_addr);


endmodule