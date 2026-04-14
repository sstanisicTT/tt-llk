
// settings can be manipulated externally by setting defines when building.


// switches
// LLK_SAN_ENABLE : Master switch

// LLK_SAN_SETTING_ASSERT : Once sanitizer check is tripped, asserts (can be useful for debugging using triage)
// LLK_SAN_SETTING_PRINT : Once sanitizer check is tripped, prints a message using DEVICE_PRINT (useful if many sanitizer checks are tripped)

// LLK_SAN_SETTING_PEDANTIC
// LLK_SAN_SETTING_WARN
// LLK_SAN_SETTING_ERROR

// LLK_SAN_SETTING_INFO
// LLK_SAN_SETTING_FAULT

// LLK_SAN_SETTING_INTERNAL

#pragma once

#define LLK_SAN_ENABLE
#define LLK_SAN_SETTING_PRINT true
#define LLK_SAN_SETTING_ERROR false

#ifndef LLK_SAN_ENABLE

    #if defined(LLK_SAN_SETTING_ASSERT)
        #error "LLK_SAN_SETTING_ASSERT is set but LLK_SAN_ENABLE is not defined"
    #endif
    #define LLK_SAN_SETTING_ASSERT false

    #if defined(LLK_SAN_SETTING_PRINT)
        #error "LLK_SAN_SETTING_PRINT is set but LLK_SAN_ENABLE is not defined"
    #endif
    #define LLK_SAN_SETTING_PRINT false

    #if defined(LLK_SAN_SETTING_PEDANTIC)
        #error "LLK_SAN_SETTING_PEDANTIC is set but LLK_SAN_ENABLE is not defined"
    #endif
    #define LLK_SAN_SETTING_PEDANTIC false

    #if defined(LLK_SAN_SETTING_WARN)
        #error "LLK_SAN_SETTING_WARN is set but LLK_SAN_ENABLE is not defined"
    #endif
    #define LLK_SAN_SETTING_WARN false

    #if defined(LLK_SAN_SETTING_ERROR)
        #error "LLK_SAN_SETTING_ERROR is set but LLK_SAN_ENABLE is not defined"
    #endif
    #define LLK_SAN_SETTING_ERROR false

    #if defined(LLK_SAN_SETTING_INFO)
        #error "LLK_SAN_SETTING_INFO is set but LLK_SAN_ENABLE is not defined"
    #endif
    #define LLK_SAN_SETTING_INFO false

    #if defined(LLK_SAN_SETTING_FAULT)
        #error "LLK_SAN_SETTING_FAULT is set but LLK_SAN_ENABLE is not defined"
    #endif
    #define LLK_SAN_SETTING_FAULT false

#else

    #if defined(LLK_SAN_SETTING_ASSERT) && defined(LLK_SAN_SETTING_PRINT)
        #error "LLK_SAN_SETTING_ASSERT and LLK_SAN_SETTING_PRINT cannot both be defined"
    #endif

    #if !defined(LLK_SAN_SETTING_ASSERT) && !defined(LLK_SAN_SETTING_PRINT)
        // If override is not provided, default to assert
        #define LLK_SAN_SETTING_ASSERT true
    #endif

    #if !defined(LLK_SAN_SETTING_PEDANTIC)
        // If override is not provided, default to pedantic
        #define LLK_SAN_SETTING_PEDANTIC false
    #endif

    #if !defined(LLK_SAN_SETTING_WARN)
        #define LLK_SAN_SETTING_WARN true
    #endif

    #if !defined(LLK_SAN_SETTING_ERROR)
        #define LLK_SAN_SETTING_ERROR true
    #endif

    #if !defined(LLK_SAN_SETTING_INFO)
        #define LLK_SAN_SETTING_INFO true
    #endif

    #if !defined(LLK_SAN_SETTING_FAULT)
        #define LLK_SAN_SETTING_FAULT false
    #endif

    #if !defined(LLK_SAN_SETTING_INTERNAL)
        #define LLK_SAN_SETTING_INTERNAL false
    #endif

#endif


