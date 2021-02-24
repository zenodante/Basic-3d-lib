#ifndef __B3D_COMMON_H__
#define __B3D_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "b3d_types.h"
#include "b3d_config.h"
#include <math.h>




#if B3L_ARM  != 1 
#define __attribute__(A)
#endif

#ifndef   __ASM
#define   __ASM         __asm
#endif

#ifndef   __STATIC_FORCEINLINE                 
#define __STATIC_FORCEINLINE                   __attribute__((always_inline)) static inline
#endif

  /*Useful macros--------------------------------------------------------------*/
#define B3L_SET(PIN,N)  (PIN |=  (1u<<N))
#define B3L_CLR(PIN,N)  (PIN &= ~(1u<<N))
#define B3L_TEST(PIN,N) (PIN &   (1u<<N))

#define B3L_logVec2(v)\
  printf("Vector 2: %.3f %.3f\n",((v).x),((v).y))
#define B3L_logVec3(v)\
  printf("Vector 3: %.3f %.3f %.3f\n",((v).x),((v).y),((v).z))
#define B3L_logVec4(v)\
  printf("Vector 4: %.3f %.3f %.3f %.3f\n",((v).x),((v).y),((v).z),((v).w))
#define B3L_logMat4(m)\
  printf("Matrix4:\n  %.3f %.3f %.3f %.3f\n  %.3f %.3f %.3f %.3f\n  %.3f %.3f %.3f %.3f\n  %.3f %.3f %.3f %.3f\n"\
   ,(m).m00,(m).m10,(m).m20,(m).m30,\
    (m).m01,(m).m11,(m).m21,(m).m31,\
    (m).m02,(m).m12,(m).m22,(m).m32,\
    (m).m03,(m).m13,(m).m23,(m).m33)
#define B3L_logMat3(m)\
  printf("Matrix3:\n  %.3f %.3f %.3f \n  %.3f %.3f %.3f \n  %.3f %.3f %.3f \n"\
   ,(m).m00,(m).m10,(m).m20,\
    (m).m01,(m).m11,(m).m21,\
    (m).m02,(m).m12,(m).m22)

#ifndef _swap_f32_t
#define _swap_f32_t(a, b) { f32 t = a; a = b; b = t; }
#endif
#ifndef _swap_int32_t
#define _swap_int32_t(a, b) { int32_t t = a; a = b; b = t; }
#endif
#ifndef _swap_uint32_t
#define _swap_uint32_t(a, b) { uint32_t t = a; a = b; b = t; }
#endif
#define B3L_MIN(a,b)      ((a) >= (b) ? (b) : (a))
#define B3L_MAX(a,b)      ((a) >= (b) ? (a) : (b))


#if  B3L_ARM == 1
  __STATIC_FORCEINLINE f32      B3L_Sqrtf(f32 in);
  __STATIC_FORCEINLINE f32      B3L_Absf(f32 in);
__STATIC_FORCEINLINE u32 VcvtF32ToU32_Fix(f32 in);
  __STATIC_FORCEINLINE s32 arm_F32toFixPointI32 (f32 x, s32 bits);
#else
#define B3L_Sqrtf(a)   sqrtf(a)
#define B3L_Absf(a)    fabsf(a)
    
#endif 
  __STATIC_FORCEINLINE s32    B3L_CeilToS(f32 in);  
  __STATIC_FORCEINLINE s32      B3L_RoundingToS(f32 in);
 // __STATIC_FORCEINLINE s32      B3L_RoundingToU(f32 in);
  __STATIC_FORCEINLINE f32      FastInvertSqrt(f32 x);
  //__STATIC_FORCEINLINE s32      VcvtF32ToS32_Fix(f32 in);
  //__STATIC_FORCEINLINE f32      VcvtS32ToF32_Fix(s32 in);
  __STATIC_FORCEINLINE u32      SatToU4(s32 in);
  __STATIC_FORCEINLINE u32      SatToU8(s32 in);
  __STATIC_FORCEINLINE u32      SatToU16(s32 in);
  __STATIC_FORCEINLINE f32      Interp_f(f32 x1, f32 x2, f32 t);
  __STATIC_FORCEINLINE s32      Clamp_i(s32 v, s32 v1, s32 v2);
  __STATIC_FORCEINLINE f32      Clamp_f(f32 v, f32 v1, f32 v2);

  /*Function defines-----------------------------------------------------------*/
/*-----------------------------------------------------------------------------
Math function
-----------------------------------------------------------------------------*/
#if B3L_ARM  == 1
  __STATIC_FORCEINLINE f32 B3L_Sqrtf(f32 in) {
    f32 result;
    __ASM("vsqrt.f32 %0,%1" : "=t"(result) : "t"(in));
    return (result);
  }
  __STATIC_FORCEINLINE f32 B3L_Absf(f32 in) {
    f32 result;
    __ASM("vabs.f32 %0,%1" : "=t"(result) : "t"(in));
    return (result);
  }



  __STATIC_FORCEINLINE  u32 VcvtF32ToU32_Fix(f32 x)
  {
      u32 y;
      __ASM(
          "VCVT.u32.f32 %[y], %[x], #16"
          : [y] "=t" (y) /* output */
          : [x] "0" (x) /* input(s) */
          : /* list of clobbered registers */);
      return y;
  }

  __STATIC_FORCEINLINE s32 arm_F32toFixPointI32 (f32 x, s32 bits)
  {
    s32 y;
    __asm__ __volatile__ (
      "VCVT.s32.f32 %[y], %[x], %[bits]"
        : [y] "=t" (y) /* output */
        : [x] "0" (x), [bits] "I" (bits) /* input(s) */
        : /* list of clobbered registers */);
    return y;
  }

  __STATIC_FORCEINLINE u32   SatToU4(s32 in) {
    u32 result;
    __ASM("usat %0, %1, %2" : "=r" (result) : "I" (4), "r" (in));
    return result;
  }


  __STATIC_FORCEINLINE u32   SatToU8(s32 in) {
    u32 result;
    __ASM("usat %0, %1, %2" : "=r" (result) : "I" (8), "r" (in));
    return result;
  }


  __STATIC_FORCEINLINE u32   SatToU16(s32 in) {
    u32 result;
    __ASM("usat %0, %1, %2" : "=r" (result) : "I" (16), "r" (in));
    return result;
  }

  __STATIC_FORCEINLINE s32   B3L_RoundingToS(f32 in) {
    s32 result;
    __ASM("vcvtr.s32.f32 %0,%1" : "=t"(result) : "t"(in));
    return result;
  }

  __STATIC_FORCEINLINE s32   B3L_CeilToS(f32 in) {
    s32 result;
    result = B3L_RoundingToS(in + 0.5f);
    //__ASM ("vcvt.s32.f32 %0,%1" : "=t"(result) : "t"(in));
    return result;
  }
  /*
  __STATIC_FORCEINLINE s32   B3L_RoundingToU(f32 in) {
    u32 result;
    __ASM("vcvt.u32.f32 %0,%1" : "=t"(result) : "t"(in));
    return result;
  }
  */
#else
  /*
  __STATIC_FORCEINLINE s32   VcvtF32ToS32_Fix(f32 in) {
    return ((s32)(in * ((f32)(1 << B3L_FIX_BITS))));
  }
  __STATIC_FORCEINLINE f32   VcvtS32ToF32_Fix(s32 in) {
    return ((f32)in) / ((f32)(1 << B3L_FIX_BITS));
  }
*/


  __STATIC_FORCEINLINE u32   SatToU4(s32 in) {
    const int32_t max = ((1U << 4) - 1U);
    return B3L_MAX(0, B3L_MIN(max, in));
  }


  __STATIC_FORCEINLINE u32   SatToU8(s32 in) {
    const int32_t max = ((1U << 8) - 1U);
    return B3L_MAX(0, B3L_MIN(max, in));
  }
  __STATIC_FORCEINLINE u32   SatToU16(s32 in) {
    const int32_t max = ((1U << 16) - 1U);
    return B3L_MAX(0, B3L_MIN(max, in));
  }
  
  __STATIC_FORCEINLINE s32   B3L_RoundingToS(f32 in) {
    return (s32)roundf(in);
  }
/*
  __STATIC_FORCEINLINE s32   B3L_RoundingToU(f32 in) {
    return (u32)roundf(in);
  }
*/
  __STATIC_FORCEINLINE s32   B3L_CeilToS(f32 in) {
      return (u32)ceilf(in);
  }
#endif //end of B3L_ARM

  __STATIC_FORCEINLINE f32 Clamp_f(f32 v, f32 v1, f32 v2) {
    return v > v1 ? (v < v2 ? v : v2) : v1;
  }

  __STATIC_FORCEINLINE s32 Clamp_i(s32 v, s32 v1, s32 v2) {
    return v >= v1 ? (v <= v2 ? v : v2) : v1;
  }

  //inv sqrt black magic from quake 
  __STATIC_FORCEINLINE f32 FastInvertSqrt(f32 x) {
    f32 xhalf = 0.5f * x;
    int i = *(int*)&x;            // store floating-point bits in integer
    i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
    x = *(float*)&i;              // convert new bits into float
    x = x * (1.5f - xhalf * x * x);     // One round of Newton's method
    return x;
  }

  __STATIC_FORCEINLINE f32 Interp_f(f32 x1, f32 x2, f32 t) {
    return x1 + (x2 - x1) * t;
  }



#ifdef __cplusplus
}
#endif

#endif
