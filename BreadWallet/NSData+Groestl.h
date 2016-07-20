//
//  NSData+Groestl.h
//  DashWallet
//
//  Created by Quantum Explorer on 3/22/15.
//  Copyright (c) 2015 Aaron Voisine. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IntTypes.h"

@interface NSData(Groestl)

-(UInt512)groestl512;
-(UInt256)groestl512_32Bits;
-(UInt512)groestl512_2;
-(UInt256)groestl512_2_32Bits;

@end
