#ifndef __B3D_DRAW_H__
#define __B3D_DRAW_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "b3d_types.h"
  extern void DrawTriangleColor(f32 x0, f32 y0, f32 z0,
                                f32 x1, f32 y1, f32 z1,
                                f32 x2, f32 y2, f32 z2,
                                fBuff_t* fbuff, zBuff_t* zbuff, fBuff_t color, s8 lightFact);
  extern void DrawTriangleTexture(f32 x0, f32 y0, f32 u0, f32 v0, f32 z0,
                                  f32 x1, f32 y1, f32 u1, f32 v1, f32 z1,
                                  f32 x2, f32 y2, f32 u2, f32 v2, f32 z2,
                                  fBuff_t* fbuff, zBuff_t* zbuff, texture_t* texture, s8 lightFact);

  extern void DrawSpaceLine(s32 Ax, s32 Ay, f32 Az, s32 Bx, s32 By, f32 Bz,
                                fBuff_t color, fBuff_t* pFrameBuff, zBuff_t* pZbuff);

  extern void DrawSpaceBitmap(f32 x0, f32 y0, f32 x1, f32 y1, f32 z,
                              f32 u0, f32 v0, f32 u1, f32 v1,
                              fBuff_t* fbuff, zBuff_t* zbuff, texture_t* texture, s8 lightFact);

  extern void ClearZbuff(zBuff_t* pZbuff, u32 length);
  extern void ClearFbuff(fBuff_t* pFbuff, u32 length,fBuff_t color);

#ifdef __cplusplus
}
#endif

#endif
