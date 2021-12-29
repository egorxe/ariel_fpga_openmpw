/*
 * SPDX-FileCopyrightText: 2021 Moscow State University
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * SPDX-License-Identifier: Apache-2.0
 */

// This include is relative to $CARAVEL_PATH (see Makefile)
#include "verilog/dv/caravel/defs.h"

/*
	FPGA config loader via WB
*/

// Address defines
#define OFF_CONFIG_LBLOCK       0x100000
#define OFF_CONFIG_VRNODE       0x200000
#define OFF_CONFIG_HRNODE       0x300000
#define OFF_CONFIG_TAPCLK       0xE00000
#define OFF_CONFIG_RSTCFG       0xA00000

#define OFF_CNT_ADDR            0xF000F0

#define write_user_reg(off, val) {(*(volatile uint32_t*)(0x30000000 + off)) = val;}
#define read_user_reg(off) (*(volatile uint32_t*)(0x30000000 + off))

// Include bitstream data
#include "wb_cnt.txt_lb_bit.h"
#include "wb_cnt.txt_vn_bit.h"
#include "wb_cnt.txt_hn_bit.h"

void main()
{
	// Caravel setup stuff

	reg_spimaster_config = 0xa002;

    reg_mprj_io_37 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_36 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_35 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_34 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_33 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_32 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_31 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_30 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_29 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_28 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_27 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_26 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_25 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_24 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_23 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_22 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_21 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_20 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_19 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_18 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_17 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_16 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_15 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_14 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_13 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_12 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_11 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_10 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_9  = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_8  = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_7  = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_6  = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_5  = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_4  = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_3  = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_2  = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_1  = GPIO_MODE_USER_STD_BIDIRECTIONAL;
    reg_mprj_io_0  = GPIO_MODE_USER_STD_BIDIRECTIONAL;

    // Apply configuration
    reg_mprj_xfer = 1;
    while (reg_mprj_xfer == 1);
	reg_la2_oenb = reg_la2_iena = 0xFFFFFFFF;    // [95:64]

    // Reset FPGA & terminate pads
    write_user_reg(OFF_CONFIG_RSTCFG, 3);
    
    // Load config
    for (int i = 0; i < lblock_config_words; ++i)
    {
        // logic blocks
        write_user_reg(OFF_CONFIG_LBLOCK, lblock_config_data[i]);
        write_user_reg(OFF_CONFIG_TAPCLK, 1);
        write_user_reg(OFF_CONFIG_TAPCLK, 0);
    }
    
    for (int i = 0; i < vnode_config_words; ++i)
    {
        // vertical routing nodes
        write_user_reg(OFF_CONFIG_VRNODE, vnode_config_data[i]);
        write_user_reg(OFF_CONFIG_TAPCLK, 2);
        write_user_reg(OFF_CONFIG_TAPCLK, 0);
    }
    
    for (int i = 0; i < hnode_config_words; ++i)
    {
        // horizontal routing nodes
        write_user_reg(OFF_CONFIG_HRNODE, hnode_config_data[i]);
        write_user_reg(OFF_CONFIG_TAPCLK, 4);
        write_user_reg(OFF_CONFIG_TAPCLK, 0);
    }
    
    // Release pads & FPGA reset
    write_user_reg(OFF_CONFIG_RSTCFG, 1);
    write_user_reg(OFF_CONFIG_RSTCFG, 0);
    
    // Reset WB logic in FPGA fabric
    write_user_reg(OFF_CONFIG_RSTCFG, 8);
    write_user_reg(OFF_CONFIG_RSTCFG, 0);
    
    // Increment counter for test
    write_user_reg(OFF_CNT_ADDR, 0);
    read_user_reg(OFF_CNT_ADDR);
}
