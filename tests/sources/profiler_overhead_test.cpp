// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <type_traits>

#include "ckernel.h"
#include "llk_defs.h"
#include "profiler.h"

// Globals
uint32_t unp_cfg_context        = 0;
uint32_t pack_sync_tile_dst_ptr = 0;

#ifdef LLK_TRISC_UNPACK

#include "profiler.h"

void run_kernel()
{
    // measure length of zones of different sizes

    {
        ZONE_SCOPED("OVERHEAD");
    }

    for (uint32_t i = 1; i < 32; i++)
    {
        uint32_t cnt = i;
        {
            ZONE_SCOPED("OVERHEAD");
        loop:
            asm volatile("addi %0, %1, -1" : "=r"(cnt) : "r"(cnt));
            asm volatile("nop");
            asm volatile("nop");
            asm volatile("nop");
            asm volatile("nop");
            asm volatile("nop");
            asm volatile("nop");
            asm volatile("nop");
            asm volatile("nop");
            asm volatile goto("bgtu %0, zero, %l1" : : "r"(cnt) : : loop);
        }
    }
}

#endif

#ifdef LLK_TRISC_MATH

void run_kernel()
{
    // Only unpack kernel is measuring profiler overhead
}

#endif

#ifdef LLK_TRISC_PACK

void run_kernel()
{
    // Only unpack kernel is measuring profiler overhead
}

#endif
