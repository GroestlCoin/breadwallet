//
//  IntTypes.h
//  BreadWallet
//
//  Created by Quantum Explorer on 7/20/16.
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


#define uint512_eq(a, b)\
((a).u64[0] == (b).u64[0] && (a).u64[1] == (b).u64[1] && (a).u64[2] == (b).u64[2] && (a).u64[3] == (b).u64[3] &&\
(a).u64[4] == (b).u64[4] && (a).u64[5] == (b).u64[5] && (a).u64[6] == (b).u64[6] && (a).u64[7] == (b).u64[7])
#define uint256_eq(a, b)\
((a).u64[0] == (b).u64[0] && (a).u64[1] == (b).u64[1] && (a).u64[2] == (b).u64[2] && (a).u64[3] == (b).u64[3])
#define uint160_eq(a, b)\
((a).u32[0] == (b).u32[0] && (a).u32[1] == (b).u32[1] && (a).u32[2] == (b).u32[2] && (a).u32[3] == (b).u32[3] &&\
(a).u32[4] == (b).u32[4])
#define uint128_eq(a, b) ((a).u64[0] == (b).u64[0] && (a).u64[1] == (b).u64[1])

#define uint256_supeq(a, b) (a.u64[3] > b.u64[3]) || ((a.u64[3] == b.u64[3]) && ((a.u64[2] > b.u64[2]) || ((a.u64[2] == b.u64[2]) && ((a.u64[1] > b.u64[1]) || ((a.u64[1] == b.u64[1]) && (a.u64[0] >= b.u64[0]))))))

#define uint256_sup(a, b) (a.u64[3] > b.u64[3]) || ((a.u64[3] == b.u64[3]) && ((a.u64[2] > b.u64[2]) || ((a.u64[2] == b.u64[2]) && ((a.u64[1] > b.u64[1]) || ((a.u64[1] == b.u64[1]) && (a.u64[0] > b.u64[0]))))))

#define uint512_is_zero(u)\
(((u).u64[0] | (u).u64[1] | (u).u64[2] | (u).u64[3] | (u).u64[4] | (u).u64[5] | (u).u64[6] | (u).u64[7]) == 0)
#define uint256_is_zero(u) (((u).u64[0] | (u).u64[1] | (u).u64[2] | (u).u64[3]) == 0)
#define uint160_is_zero(u) (((u).u32[0] | (u).u32[1] | (u).u32[2] | (u).u32[3] | (u).u32[4]) == 0)
#define uint128_is_zero(u) (((u).u64[0] | (u).u64[1]) == 0)

#define uint512_obj(u) ([NSValue value:(u).u8 withObjCType:@encode(UInt512)])
#define uint256_obj(u) ([NSValue value:(u).u8 withObjCType:@encode(UInt256)])
#define uint160_obj(u) ([NSValue value:(u).u8 withObjCType:@encode(UInt160)])
#define uint128_obj(u) ([NSValue value:(u).u8 withObjCType:@encode(UInt128)])

#define UINT512_ZERO ((UInt512) { .u64 = { 0, 0, 0, 0, 0, 0, 0, 0 } })
#define UINT256_ZERO ((UInt256) { .u64 = { 0, 0, 0, 0 } })
#define UINT160_ZERO ((UInt160) { .u32 = { 0, 0, 0, 0, 0 } })
#define UINT128_ZERO ((UInt128) { .u64 = { 0, 0 } })


#endif /* IntTypes_h */
