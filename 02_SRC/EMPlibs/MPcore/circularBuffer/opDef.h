//
// Created by alfy on 06/05/21.
//

#ifndef EMP_LIB_02_SRC_EMPLIBS_CIRCULARBUFFER_MODULAROPERATION_H
#define EMP_LIB_02_SRC_EMPLIBS_CIRCULARBUFFER_MODULAROPERATION_H

#define errorRet ((u_int16_t)-1)  // Variable is unsigned, so the result will be 65535


#define modAdd(a, b, M) ((a + b) % M)
#define modSub(a, b, M) (((a - b) + M) % M)

// General Cast
#define M_To(T, p)            reinterpret_cast<T>(p)                       // Cast to T

#define M_ToCharPtr(p)        reinterpret_cast<char*>(p)                   // Cast to char*
#define M_ToWCharPtr(p)       reinterpret_cast<wchar_t*>(p)                // Cast to wchar_t*
#define M_ToConstCharPtr(p)   reinterpret_cast<const char*>(p)             // Cast to const char*
#define M_ToConstWCharPtr(p)  reinterpret_cast<const wchar_t*>(p)          // Cast to const wchar_t*
#define M_ToUCharPtr(p)       reinterpret_cast<unsigned char*>(p)          // Cast to unsigned char*
#define M_ToConstUCharPtr(p)  reinterpret_cast<const unsigned char*>(p)    // Cast to const unsigned char*
#define M_ToUCharPtr(n)       reinterpret_cast<unsigned char*>(n)          // Cast to unsigned char*
#define M_ToVoidPtr(p)        reinterpret_cast<void*>(p)                   // Cast to void*
#define M_ToConstVoidPtr(p)   reinterpret_cast<const void*>(p)             // Cast to const void*
#define M_ToIntPtr(n)         reinterpret_cast<int*>(n)                    // Cast to int*
#define M_ToConstIntPtr(p)    reinterpret_cast<const int*>(p)              // Cast to const int*
#define M_ToDoublePtr(n)      reinterpret_cast<double*>(n)                 // Cast to double*
#define M_ToConstDoublePtr(n) reinterpret_cast<const double*>(n)           // Cast to const double*
#define M_ToBoolPtr(n)        reinterpret_cast<bool*>(n)                   // Cast to bool*
#define M_ToConstBoolPtr(n)   reinterpret_cast<const bool*>(n)             // Cast to const bool*

#endif // EMP_LIB_02_SRC_EMPLIBS_CIRCULARBUFFER_MODULAROPERATION_H
