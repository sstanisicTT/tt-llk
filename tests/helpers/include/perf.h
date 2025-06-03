#pragma once

#if defined(LLK_PERF)


template<
    bool USE_A,
    bool USE_B,
>
__attribute__((always_inline, noreturn)) inline void mock_math_for_unpack(uint32_t iterations) {
    while (NUM_UNPACK0_ITERATIONS-->0) {

        TTI_STALLWAIT(p_stall::STALL_MATH, (USE_A && p_stall::SRCA_VLD) | (USE_A && p_stall::SRCA_VLD))
        TTI_CLEARDVALID(0b01, 0, 0, 0, 0, 0);                
           }  

}

#endif