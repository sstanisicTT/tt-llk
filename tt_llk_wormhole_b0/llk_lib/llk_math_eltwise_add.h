#include "ckernel_include.h"
#include "llk_math_common.h"

using namespace ckernel;

inline void _llk_math_eltwise_add_simple_init_()
{
    // addrmod
    addr_mod_t {
        .srca = {.incr = 8},
        .srcb = {.incr = 8},
        .dest = {.incr = 8},
    }
        .set(ADDR_MOD_0);

}

inline void _llk_math_eltwise_add_simple_(const std::uint32_t num_faces, uint dst_index) {
    math::set_dst_write_addr<DstTileLayout::Default, DstTileShape::Tile32x32>(dst_index);
    
}
