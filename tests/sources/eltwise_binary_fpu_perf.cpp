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

#define TILE_CNT 128

#ifdef LLK_TRISC_UNPACK

#include "llk_unpack_AB.h"
#include "llk_unpack_common.h"
#include "params.h"

template <bool USE_A, bool USE_B>
__attribute__((always_inline)) inline void mock_unpack_for_math(uint32_t iterations)
{
    while (iterations-- > 0)
    {
        TTI_SETDVALID(0b11);
    //    TTI_STALLWAIT(p_stall::STALL_UNPACK, p_stall::SRCA_CLR | p_stall::SRCB_CLR);
    }
}

void run_kernel()
{
    // return;
    volatile uint32_t* const src_a = reinterpret_cast<volatile uint32_t*>(0x1a000);
    volatile uint32_t* const src_b = reinterpret_cast<volatile uint32_t*>(0x1e000);

    {
        ZONE_SCOPED("INIT")
        _llk_unpack_AB_hw_configure_<false, StochRndType::None>(UNPACK_A_IN, UNPACK_B_IN, UNPACK_A_OUT, UNPACK_B_OUT, 8, false, 4);
        _llk_unpack_AB_init_<BroadcastType::NONE>(16, 4, false, false, false);
        tensix_sync();
    }
    // mock_unpack_for_math<true, true>(30000);
    // return;
    {
        ZONE_SCOPED("TILE-LOOP")
        for (uint32_t tile = 0; tile < TILE_CNT; tile++)
        {
            _llk_unpack_AB_<BroadcastType::NONE>(L1_ADDRESS(src_a + tile * 0x1000), L1_ADDRESS(src_b + tile * 0x1000), false);
        }
        tensix_sync();
    }
}

#endif

#ifdef LLK_TRISC_MATH

#include "llk_math_common.h"
#include "llk_math_eltwise_binary.h"
#include "params.h"

template <bool USE_A, bool USE_B>
__attribute__((always_inline)) inline void mock_math_for_unpack(uint32_t iterations)
{
    while (iterations-- > 0)
    {
        TTI_STALLWAIT(p_stall::STALL_MATH, p_stall::SRCA_VLD | p_stall::SRCB_VLD);
        TTI_CLEARDVALID(p_setrwc::CLR_A | p_setrwc::CLR_B, 0);
    }
}

void run_kernel()
{
    {
        ZONE_SCOPED("INIT")
        _llk_math_pack_sync_init_<DstSync::SyncHalf, false>();
        _llk_math_hw_configure_<false>(MATH_FORMAT, MATH_FORMAT);
        _llk_math_eltwise_binary_init_<ELWADD, BroadcastType::NONE>(4, false, false);
        tensix_sync();
    }
    mock_math_for_unpack<true, true>(TILE_CNT*4);
    return;
    {
        ZONE_SCOPED("TILE-LOOP")
        for (uint32_t tile = 0; tile < TILE_CNT; tile++)
        {
            //_llk_math_wait_for_dest_available_<DstSync::SyncHalf>();
            _llk_math_eltwise_binary_<EltwiseBinaryType::ELWADD, BroadcastType::NONE, DstSync::SyncHalf>(4, 0, false);
            //_llk_math_dest_section_done_<DstSync::SyncHalf, false>();
        }
        tensix_sync();
    }
}

#endif

#ifdef LLK_TRISC_PACK

#include "llk_pack.h"
#include "llk_pack_common.h"
#include "params.h"

void run_kernel()
{
    return;
    volatile uint32_t* const dst = reinterpret_cast<volatile uint32_t*>(0x1E000);
    {
        ZONE_SCOPED("INIT")
        _llk_pack_hw_configure_<false, is_fp32_dest_acc_en>(PACK_IN, PACK_OUT, 16 * 16 * 4);
        _llk_pack_init_<false, false, DstTileFaceLayout::RowMajor, false>(PACK_OUT);
        _llk_pack_dest_init_<DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false, false>();
        tensix_sync();
    }
    {
        ZONE_SCOPED("TILE-LOOP")
        for (uint32_t tile = 0; tile < TILE_CNT; tile++)
        {
            _llk_packer_wait_for_math_done_();
            _llk_pack_<DstSync::SyncHalf, false, false>(0, L1_ADDRESS(dst + tile * 0x1000));
            _llk_pack_dest_section_done_<DstSync::SyncHalf, false>();
        }
        tensix_sync();
    }
}

#endif
