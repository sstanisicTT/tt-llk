// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "sanitizer/settings.h"
#include "sanitizer/types.h"


#ifndef LLK_SAN_ENABLE

    #define LLK_SAN_ASSERT(condition, message)

#elif defined(LLK_SAN_SETTING_ASSERT)

    #include "llk_assert.h"

    #define LLK_SAN_ASSERT(condition, message)  LLK_ASSERT((condition), "llk::san | " message)

#elif defined(LLK_SAN_SETTING_PRINT)

    #ifdef ENV_LLK_INFRA
        #error "llk_san: error: LLK_SAN_SETTING_PRINT is not supported in LLK INFRA, only in metal"
    #endif

    #include "api/debug/device_print.h"

    #define LLK_SAN_ASSERT(condition, message)              \
        do {                                                \
            if (!(condition)) {                             \
                DEVICE_PRINT("llk::san | " message "\n");   \
            }                                               \
        } while (0)

#else
    // todo(sstanisic): better message
    #error "llk_san: fault: What a Terrible Failure"
#endif

#define LLK_SAN_PANIC(condition, message) LLK_SAN_ASSERT(!(condition), message)


#if LLK_SAN_SETTING_PEDANTIC == false
    #define LLK_SAN_PEDANTIC_ASSERT(condition, message)
#elif LLK_SAN_SETTING_PEDANTIC == true
    #define LLK_SAN_PEDANTIC_ASSERT(condition, message) LLK_SAN_ASSERT((condition), "pedantic | " message)
#else
    #error "llk_san: fault: invalid value for LLK_SAN_SETTING_PEDANTIC"
#endif

#define LLK_SAN_PEDANTIC_PANIC(condition, message) LLK_SAN_PEDANTIC_ASSERT(!(condition), message)
#define LLK_SAN_PEDANTIC_MSG(message) LLK_SAN_PEDANTIC_ASSERT(false, message)


#if LLK_SAN_SETTING_WARN == false
    #define LLK_SAN_WARN_ASSERT(condition, message)
#elif LLK_SAN_SETTING_WARN == true
    #define LLK_SAN_WARN_ASSERT(condition, message) LLK_SAN_ASSERT((condition), "warn     | " message)
#else
    #error "llk_san: fault: invalid value for LLK_SAN_SETTING_WARN"
#endif

#define LLK_SAN_WARN_PANIC(condition, message) LLK_SAN_WARN_ASSERT(!(condition), message)
#define LLK_SAN_WARN_MSG(message) LLK_SAN_WARN_ASSERT(false, message)


#if LLK_SAN_SETTING_ERROR == false
    #define LLK_SAN_ERROR_ASSERT(condition, message)
#elif LLK_SAN_SETTING_ERROR == true
    #define LLK_SAN_ERROR_ASSERT(condition, message) LLK_SAN_ASSERT((condition), "error    | " message)
#else
    #error "llk_san: fault: invalid value for LLK_SAN_SETTING_ERROR"
#endif

#define LLK_SAN_ERROR_PANIC(condition, message) LLK_SAN_ERROR_ASSERT(!(condition), message)
#define LLK_SAN_ERROR_MSG(message) LLK_SAN_ERROR_ASSERT(false, message)


#if LLK_SAN_SETTING_FAULT == false
    #define LLK_SAN_FAULT_ASSERT(condition, message)
#elif LLK_SAN_SETTING_FAULT == true
    #define LLK_SAN_FAULT_ASSERT(condition, message) LLK_SAN_ASSERT((condition), "fault    | " message)
#else
    #error "llk_san: fault: invalid value for LLK_SAN_SETTING_FAULT"
#endif

#define LLK_SAN_FAULT_PANIC(condition, message) LLK_SAN_FAULT_ASSERT(!(condition), message)
#define LLK_SAN_FAULT_MSG(message) LLK_SAN_FAULT_ASSERT(false, message)


#if LLK_SAN_SETTING_INFO == false
    #define LLK_SAN_INFO_ASSERT(condition, message)
#elif LLK_SAN_SETTING_INFO == true
    #define LLK_SAN_INFO_ASSERT(condition, message) LLK_SAN_ASSERT((condition), "info     | " message)
#else
    #error "llk_san: fault: invalid value for LLK_SAN_SETTING_INFO"
#endif

#define LLK_SAN_INFO_PANIC(condition, message) LLK_SAN_INFO_ASSERT(!(condition), message)
#define LLK_SAN_INFO_MSG(message) LLK_SAN_INFO_ASSERT(false, message)


#if LLK_SAN_SETTING_INTERNAL == false
    #define LLK_SAN_INTERNAL_ASSERT(condition, message)
#elif LLK_SAN_SETTING_INTERNAL == true
    #define LLK_SAN_INTERNAL_ASSERT(condition, message) LLK_SAN_ASSERT((condition), "internal | " message)
#else
    #error "llk_san: fault: invalid value for LLK_SAN_SETTING_INTERNAL"
#endif

#define LLK_SAN_INTERNAL_PANIC(condition, message) LLK_SAN_INTERNAL_ASSERT(!(condition), message)
#define LLK_SAN_INTERNAL_MSG(message) LLK_SAN_INTERNAL_ASSERT(false, message)
