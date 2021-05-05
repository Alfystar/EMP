//
// Created by alfy on 04/05/21.
//

#ifndef EMP_LIB_MACRO_H
#define EMP_LIB_MACRO_H


#define sizeofArray(x)  sizeof(x)/sizeof(x[0])

#define _debugCodeLevel(x,level) if(x<=level)

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#define modAdd(a,b,M) ((a+b)%M)
#define modSub(a,b,M) (((a-b)+M)%M)

#endif //EMP_LIB_MACRO_H
