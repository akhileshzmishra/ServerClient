//
//  XLDependency.h
//  CustomizableClientServer
//
//  Created by Akhilesh Mishra on 31/03/16.
//  Copyright © 2016 Akhilesh Mishra. All rights reserved.
//

#ifndef XLDependency_h
#define XLDependency_h

#ifndef _C_11_COMPILER
#define _C_11_COMPILER
#endif

//Here any macro can be programmatically added.
#ifndef __UNIX__ //__UNIX__
#define __UNIX__
#endif


//Switch on for windows os
#ifndef __WINDOWS__
//#define __WINDOWS__
#endif


//Switch on for Mac os
#ifndef __MAC_OS_
#define __MAC_OS_

#ifndef __UNIX__
#define __UNIX__
#endif


#endif


#endif /* XLDependency_h */
