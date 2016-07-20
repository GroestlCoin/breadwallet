//
//  IntTypes.h
//  BreadWallet
//
//  Created by Sam Westrich on 7/20/16.
//  Copyright © 2016 Aaron Voisine. All rights reserved.
//

#ifndef IntTypes_h
#define IntTypes_h

typedef union _UInt512 {
    uint8_t u8[512/8];
    uint16_t u16[512/16];
    uint32_t u32[512/32];
    uint64_t u64[512/64];
} UInt512;

typedef union _UInt256 {
    uint8_t u8[256/8];
    uint16_t u16[256/16];
    uint32_t u32[256/32];
    uint64_t u64[256/64];
} UInt256;

typedef union _UInt160 {
    uint8_t u8[160/8];
    uint16_t u16[160/16];
    uint32_t u32[160/32];
} UInt160;

typedef union _UInt128 {
    uint8_t u8[128/8];
    uint16_t u16[128/16];
    uint32_t u32[128/32];
    uint64_t u64[128/64];
} UInt128;


#endif /* IntTypes_h */
