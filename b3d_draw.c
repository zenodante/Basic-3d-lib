#include "b3d_config.h"
#include "b3d_types.h"
#include "b3d_common.h"
#include <stdio.h>
#include <math.h>

__STATIC_FORCEINLINE void Tline(f32 a, u32 y, f32 b, f32 aZ, f32 bZ, f32 aU, f32 aV, f32 bU, f32 bV, s8 lightFactor, fBuff_t* fbuff, zBuff_t* zbuff, B3L_tex_t* tx);
__STATIC_FORCEINLINE void Cline(f32 a, u32 y, f32 b, f32 aZ, f32 bZ, fBuff_t* fbuff, zBuff_t* zbuff, fBuff_t color);
__STATIC_FORCEINLINE void DrawPixelWithTest(B3L_tex_t color, s32 x, s32 y, f32 z, fBuff_t* pFrameBuff, zBuff_t* pZbuff);
__STATIC_FORCEINLINE void DrawPixel(fBuff_t color, s32 x, s32 y, f32 z, fBuff_t* pFrameBuff, zBuff_t* pZbuff);
__STATIC_FORCEINLINE u32 GetZtestValue(f32 z);


#define UV_DATA_SHIFT       6



__STATIC_FORCEINLINE u32 GetZtestValue(f32 z) {
#if B3L_ARM  == 1
    return  VcvtF32ToU32_Fix(z);
    
#else
    return B3L_RoundingToS(z * (65535.0f));
#endif
}



void DrawTriangleColor(f32 x0, f32 y0, f32 z0,
                       f32 x1, f32 y1, f32 z1,
                       f32 x2, f32 y2, f32 z2,
                       fBuff_t* fbuff, zBuff_t* zbuff, fBuff_t color, s8 lightFact) {
  s32 y, yEnd;//current y and the end of the y loop
  if (color == 0) {
    return;
  }
  else {
#ifdef USING_COLOR_LEVEL
    s8 colorRow = color & 0xF0;
    s8 colorColumn = color & 0x0F;
    color = SatToU4(colorColumn + lightFact) + colorRow;
#endif
    color = color;
  }

  if (y0 > y1) {
    _swap_f32_t(y0, y1); _swap_f32_t(x0, x1); _swap_f32_t(z0, z1);
  }
  if (y1 > y2) {
    _swap_f32_t(y2, y1); _swap_f32_t(x2, x1);  _swap_f32_t(z2, z1);
  }
  if (y0 > y1) {
    _swap_f32_t(y0, y1); _swap_f32_t(x0, x1);  _swap_f32_t(z0, z1);
  }
 
  // if the triangle size in y is smaller than 1 pixel, then skip it
  f32 dy01 = (y1 - y0); f32 dy02 = (y2 - y0); f32 dy12 = (y2 - y1);
  s32 cy0 = B3L_CeilToS(y0); s32 cy1 = B3L_CeilToS(y1); s32 cy2 = B3L_CeilToS(y2); //int y
  //s32 stepUp = cy1 - cy0;
  //s32 stepDown = cy2 - cy1;
  f32  aZ; f32  bZ; f32  aX; f32  bX;
  
  f32  sy;
  if ((cy2 < 0) || ((cy2 - cy0) <= 1) || (cy0 >= RENDER_RESOLUTION_Y)) {
      return;
  }
  f32 dy02Inv = 1.0f / dy02;
  f32 dx02 = (x2 - x0) * dy02Inv;  f32 dz02 = (z2 - z0) * dy02Inv; 
  if ((cy1 < 1) || (cy0 == cy1)) {
      goto LOWER_PART_COLOR;
  }
  f32 dy01Inv = 1.0f / dy01;
  //x,z,u,v step size
  f32 dx01 = (x1 - x0) * dy01Inv;  f32 dz01 = (z1 - z0) * dy01Inv; 

  if (y0 < 0.0f) {
      bX = x0 - y0 * dx02;
      bZ = z0 - y0 * dz02;
      aX = x0 - y0 * dx01;
      aZ = z0 - y0 * dz01;
      y0 = 0.0f;
      cy0 = 0;
  }
  else {
      sy = cy0 - y0;
      aX =x0 + sy * dx01; bX =x0 + sy * dx02;
      aZ =z0 + sy * dz01; bZ =z0 + sy * dz02;
  }
  y = cy0;
  yEnd = B3L_MIN((cy1 - 1), RENDER_RESOLUTION_Y - 1);

  if ((aX + dx01) > (bX + dx02)) {
      for (; y <= yEnd; y++) {
          Cline(bX, y, aX, bZ, aZ, fbuff, zbuff, color);
          aX += dx01;  aZ = aZ + dz01;
          bX += dx02;  bZ = bZ + dz02;
      }
  }
  else {
      for (; y <= yEnd; y++) {
          Cline(aX, y, bX, aZ, bZ, fbuff, zbuff, color);
          aX += dx01;  aZ = aZ + dz01;
          bX += dx02;  bZ = bZ + dz02;
      }
  }
LOWER_PART_COLOR:
  if ((cy1 >= RENDER_RESOLUTION_Y) || (cy2 == cy1)) {
      return;
  }

  f32 dy12Inv = 1.0f / dy12;
  f32 dx12 = (x2 - x1) * dy12Inv;  f32 dz12 = (z2 - z1) * dy12Inv; 
  if (cy1 < 0) {
      aX = x1 - y1 * dx12;
      aZ = z1 - y1 * dz12;
      y1 = 0.0f;
      cy1 = 0;
      bX = x0 - y0 * dx02;
      bZ = z0 - y0 * dz02;
  }
  else {
      sy = cy1 - y1;
      aX = x1 + sy * dx12;
      aZ = z1 + sy * dz12;
      bX = x0 + (dy01 + sy) * dx02;
      bZ = z0 + (dy01 + sy) * dz02;
  }
  y = cy1;

  yEnd = B3L_MIN((cy2 - 1), RENDER_RESOLUTION_Y - 1);
  //printf("y:%d,yend:%d\n",y,yEnd);
  if (aX > bX) {
      for (; y <= yEnd; y++) {
          Cline(bX, y, aX, bZ, aZ, fbuff, zbuff, color);
          aX += dx12;  aZ = aZ + dz12;
          bX += dx02; bZ = bZ + dz02;
      }
  }
  else {
      for (; y <= yEnd; y++) {
          Cline(aX, y, bX, aZ, bZ, fbuff, zbuff, color);
          aX += dx12;  aZ = aZ + dz12;
          bX += dx02;  bZ = bZ + dz02;
      }
  }

}


void DrawTriangleTexture(f32 x0, f32 y0, f32 u0, f32 v0, f32 z0,
    f32 x1, f32 y1, f32 u1, f32 v1, f32 z1,
    f32 x2, f32 y2, f32 u2, f32 v2, f32 z2,
    fBuff_t* fbuff, zBuff_t* zbuff, B3L_tex_t* texture, s8 lightFact) {
    s32 y, yEnd;//current y and the end of the y loop
    if (y0 > y1) {
        _swap_f32_t(y0, y1); _swap_f32_t(x0, x1); _swap_f32_t(u0, u1); _swap_f32_t(v0, v1); _swap_f32_t(z0, z1);
    }
    if (y1 > y2) {
        _swap_f32_t(y2, y1); _swap_f32_t(x2, x1); _swap_f32_t(u2, u1); _swap_f32_t(v2, v1); _swap_f32_t(z2, z1);
    }
    if (y0 > y1) {
        _swap_f32_t(y0, y1); _swap_f32_t(x0, x1); _swap_f32_t(u0, u1); _swap_f32_t(v0, v1); _swap_f32_t(z0, z1);
    }
    // if the triangle size in y is smaller than 1 pixel, then skip it
    f32 dy01 = (y1 - y0); f32 dy02 = (y2 - y0); f32 dy12 = (y2 - y1);
    s32 cy0 = B3L_CeilToS(y0); s32 cy1 = B3L_CeilToS(y1); s32 cy2 = B3L_CeilToS(y2); //int y
    //s32 stepUp = cy1 - cy0;
    //s32 stepDown = cy2 - cy1;
    f32  aZ; f32  bZ; f32  aX; f32  bX;
    f32  aU; f32  bU; f32  aV; f32  bV;
    f32  sy;
    if ((cy2 < 0) || ((cy2 - cy0) <= 1)||(cy0>= RENDER_RESOLUTION_Y)){
        return;
    }
    f32 dy02Inv = 1.0f / dy02;
    f32 dx02 = (x2 - x0) * dy02Inv;  f32 dz02 = (z2 - z0) * dy02Inv; f32 du02 = (u2 - u0) * dy02Inv; f32 dv02 = (v2 - v0) * dy02Inv;
    if ((cy1 < 1)||(cy0==cy1)) {
        goto LOWER_PART_TEX;
    }
    f32 dy01Inv = 1.0f / dy01; 
    //x,z,u,v step size
    f32 dx01 = (x1 - x0) * dy01Inv;  f32 dz01 = (z1 - z0) * dy01Inv; f32 du01 = (u1 - u0) * dy01Inv; f32 dv01 = (v1 - v0) * dy01Inv;
   
    if (y0 < 0.0f) {
        bX = x0 - y0 * dx02;
        bZ = z0 - y0 * dz02;
        bU = u0 - y0 * du02;
        bV = v0 - y0 * dv02;
        aX = x0 - y0 * dx01;
        aZ = z0 - y0 * dz01;
        aU = u0 - y0 * du01;
        aV = v0 - y0 * dv01;
        y0 = 0.0f;
        cy0 = 0;
    }
    else {
        //aZ = z0;  bZ = z0; aX = x0; bX = x0;
        //aU = u0;  bU = u0; aV = v0; bV = v0;
        //cy0 = B3L_CeilToS(y0);
        sy = cy0 - y0;
        aX =x0 + sy * dx01; bX =x0 + sy * dx02;
        aZ =z0 + sy * dz01; bZ =z0 + sy * dz02;
        aU =u0 + sy * du01; bU =u0 + sy * du02;
        aV =v0 + sy * dv01; bV =v0 + sy * dv02;
    }
    y = cy0;
    yEnd = B3L_MIN((cy1 - 1), RENDER_RESOLUTION_Y - 1);

    if ((aX + dx01) > (bX + dx02)) {
        for (; y <= yEnd; y++) {
           
            //printf("bX:%.3f,aX:%.3f\n",bX,aX);
            //printf("bU:%.3f,bV:%.3f\n",bU,bV);
            //printf("aU:%.3f,aV:%.3f\n",aU,aV);
            Tline(bX, y, aX, bZ, aZ, bU, bV, aU, aV, lightFact, fbuff, zbuff, texture);
            aX += dx01; aU += du01; aV += dv01; aZ = aZ + dz01;
            bX += dx02; bU += du02; bV += dv02; bZ = bZ + dz02;
        }
    }
    else {
        for (; y <= yEnd; y++) {
            
            Tline(aX, y, bX, aZ, bZ, aU, aV, bU, bV, lightFact, fbuff, zbuff, texture);
            aX += dx01; aU += du01; aV += dv01; aZ = aZ + dz01;
            bX += dx02; bU += du02; bV += dv02; bZ = bZ + dz02;
        }
    }
LOWER_PART_TEX:
    if ((cy1 >= RENDER_RESOLUTION_Y)||(cy2==cy1)) {
        return;
    }

    f32 dy12Inv = 1.0f / dy12;
    f32 dx12 = (x2 - x1) * dy12Inv;  f32 dz12 = (z2 - z1) * dy12Inv; f32 du12 = (u2 - u1) * dy12Inv; f32 dv12 = (v2 - v1) * dy12Inv;
    if (cy1 < 0) {
        aX = x1 - y1 * dx12;
        aZ = z1 - y1 * dz12;
        aU = u1 - y1 * du12;
        aV = v1 - y1 * dv12;
        y1 = 0.0f;
        cy1 = 0;
        bX = x0 - y0 * dx02;
        bZ = z0 - y0 * dz02;
        bU = u0 - y0 * du02;
        bV = v0 - y0 * dv02;
    }
    else {
        sy = cy1 - y1;
        aX =x1 + sy * dx12;
        aZ =z1 + sy * dz12;
        aU =u1 + sy * du12;
        aV =v1 + sy * dv12;

        bX = x0 + (dy01+sy) * dx02;
        bZ = z0 + (dy01+sy) * dz02;
        bU = u0 + (dy01+sy) * du02;
        bV = v0 + (dy01+sy) * dv02;
    }
    y = cy1;

    yEnd = B3L_MIN((cy2-1), RENDER_RESOLUTION_Y - 1);
    //printf("y:%d,yend:%d\n",y,yEnd);
    if (aX > bX) {
        for (; y <= yEnd; y++) {
            Tline(bX, y, aX, bZ, aZ, bU, bV, aU, aV, lightFact, fbuff, zbuff, texture);
            aX += dx12; aU += du12; aV += dv12; aZ = aZ + dz12;
            bX += dx02; bU += du02; bV += dv02; bZ = bZ + dz02;
        }
    }
    else {
        for (; y <= yEnd; y++) {
            Tline(aX, y, bX, aZ, bZ, aU, aV, bU, bV, lightFact, fbuff, zbuff, texture);
            aX += dx12; aU += du12; aV += dv12; aZ = aZ + dz12;
            bX += dx02; bU += du02; bV += dv02; bZ = bZ + dz02;
        }
    }

}


__STATIC_FORCEINLINE  void Cline(f32 a, u32 y, f32 b, f32 aZ, f32 bZ, fBuff_t* fbuff, zBuff_t* zbuff, fBuff_t color) {

  f32 dx = b - a;
  f32 dxInv = 1.0f / dx;
  f32 dz = (bZ - aZ) * dxInv;


  if (a < 0.0f) {//left out of range
    aZ -= a * dz;
    a = 0.0f;
  }
  s32 ca = B3L_CeilToS(a);
  s32 cb = B3L_CeilToS(b);
  if ((ca >= RENDER_RESOLUTION_X) || (cb <= 0)) {
    return;
  }
  f32 sa = ca - a;
  aZ += dz * sa;

  s32 right = B3L_MIN(RENDER_RESOLUTION_X , cb);

  zBuff_t *pCurrentZ = zbuff + ca + RENDER_RESOLUTION_X * y;

  fBuff_t* pCurrentPixel = fbuff + ca + RENDER_X_SHIFT * y;
  u32 iZ;
  for (; ca < right; ca++) {
      iZ = GetZtestValue(aZ);
    if (iZ <= (u32)(*pCurrentZ)) {
      *pCurrentPixel = color;
      *pCurrentZ = (u16)iZ;
    }
   pCurrentPixel++;
   pCurrentZ++;
   aZ += dz;
  }

}
__STATIC_FORCEINLINE void Tline(f32 a, u32 y, f32 b, f32 aZ, f32 bZ, f32 aU, f32 aV, f32 bU, f32 bV, s8 lightFactor, fBuff_t* fbuff, zBuff_t* zbuff, B3L_tex_t* tx) {
  u8 size = tx[4];
  f32 dx = b - a;
  f32 dxInv = 1.0f / dx;
  f32 dz = (bZ - aZ) * dxInv;
  f32 du = (bU - aU) * dxInv;
  f32 dv = (bV - aV) * dxInv;
  s32 ca;
  if (a < 0.0f) {//left out of range
    aZ -= a * dz;
    aU -= a * du;
    aV -= a * dv;
    a = 0.0f;
    ca = 0;
  }
  else {
    ca = B3L_CeilToS(a);
    f32 sa = ca - a;
    aZ += dz * sa;
    aU += du * sa;
    aV += dv * sa;
  }
   
  s32 cb = B3L_CeilToS(b);
  if ((ca >= RENDER_RESOLUTION_X) || (cb <= 0)) {
    return;
  }
  
  s32 right = B3L_MIN(RENDER_RESOLUTION_X , cb);
  u8 color;
  zBuff_t* pCurrentZ = zbuff + ca + RENDER_RESOLUTION_X * y;

  fBuff_t* pCurrentPixel = fbuff + ca + RENDER_X_SHIFT * y;
  uint32_t uvShift;
  u8 colorRow;
  s8 colorColumn;
  u32 iZ;
  for (; ca < right; ca++) {
      iZ = GetZtestValue(aZ);
    if (iZ <= (u32)(*pCurrentZ)) {
      //get color
      uvShift = B3L_RoundingToS(aU) + size * B3L_RoundingToS(aV) + UV_DATA_SHIFT;
      color = tx[uvShift];
      if (color != 0) {
#ifdef USING_COLOR_LEVEL
        colorRow = color & 0xF0;
        colorColumn = color & 0x0F;
        color = SatToU4(colorColumn + lightFactor) + colorRow;
#endif
        
        *pCurrentPixel = color;
        
        *pCurrentZ = (u16)iZ;
        //}
      }
    }
    pCurrentPixel++;
    pCurrentZ++;
    aZ += dz;
    aU += du;
    aV += dv;
  }

}




__STATIC_FORCEINLINE void     DrawPixel(fBuff_t color, s32 x, s32 y, f32 z,
  fBuff_t* pFrameBuff, zBuff_t* zbuff) {
    zBuff_t* pCurrentZ = zbuff + x + RENDER_RESOLUTION_X * y;

    fBuff_t* pixel = pFrameBuff + (y * RENDER_X_SHIFT) + x;
    u32 iZ = GetZtestValue(z);
    if (iZ <= (u32)(*pCurrentZ)) {
        *pixel = color; 
        *pCurrentZ = iZ;
    }
}


__STATIC_FORCEINLINE void     DrawPixelWithTest(B3L_tex_t color, s32 x, s32 y, f32 z,
  fBuff_t* pFrameBuff, zBuff_t* zbuff) {
  if ((x < 0) || (y < 0) || (x >= RENDER_RESOLUTION_X) || (y >= RENDER_RESOLUTION_Y)) {
    return;
  }
  zBuff_t* pCurrentZ = zbuff + x + RENDER_RESOLUTION_X * y;
  
  fBuff_t* pixel = pFrameBuff + (y * RENDER_X_SHIFT) + x;
  u32 iZ = GetZtestValue(z);
  if (iZ <= (u32)(*pCurrentZ)) {
    *pixel = color;
    *pCurrentZ = iZ; 
  }
}


void DrawSpaceLine(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz, B3L_tex_t color,
                   fBuff_t* pFrameBuff, zBuff_t* pZbuff, s8 lightValue) {
    s32 Ax = B3L_CeilToS(ax);
    s32 Ay = B3L_CeilToS(ay);
    s32 Bx = B3L_CeilToS(bx);
    s32 By = B3L_CeilToS(by);
    //now A is in the left and B is in the right
    s32 steep = abs(Ay - By) > abs(Ax - Bx);
    //fBuff_t drawColor;
    if (steep) {
        _swap_int32_t(Ax, Ay);
        _swap_int32_t(Bx, By);
    }
    if (Ax > Bx) {
        //swap a,b point
        _swap_int32_t(Ax, Bx);
        _swap_int32_t(Ay, By);
        _swap_f32_t(az, bz);
    }
    s32 dx, dy;
    dx = Bx - Ax;
    dy = abs(By - Ay);
    f32 dz = (az - bz) / ((f32)dx);
    s32 err = dx / 2;
    s32 ystep;
#ifdef USING_COLOR_LEVEL
    u8 colorRow = color & 0xF0;
    s8 colorColumn = color & 0x0F;
    color = SatToU4(colorColumn + lightValue) + colorRow;
#endif
    if (Ay < By) {
        ystep = 1;
    }
    else {
        ystep = -1;
    }
    for (; Ax <= Bx; Ax++) {
        if (steep) {
            DrawPixelWithTest(color, Ay, Ax, az, pFrameBuff, pZbuff);
        }
        else {
            DrawPixelWithTest(color, Ax, Ay, az, pFrameBuff, pZbuff);
        }
        az = az + dz;
        err -= dy;
        if (err < 0) {
            Ay += ystep;
            err += dx;
        }
    }



}
//TODO need update
void DrawSpaceLine2(s32 Ax, s32 Ay, f32 Az, s32 Bx, s32 By, f32 Bz,
                   fBuff_t color, fBuff_t* pFrameBuff, zBuff_t* pZbuff) {
  //now A is in the left and B is in the right
    s32 steep = B3L_Absf(Ay - By) > B3L_Absf(Ax - Bx);
  //fBuff_t drawColor;
    if (steep) {
        _swap_int32_t(Ax, Ay);
        _swap_int32_t(Bx, By);
    }
    if (Ax > Bx) {
    //swap a,b point
        _swap_int32_t(Ax, Bx);
        _swap_int32_t(Ay, By);
        _swap_f32_t(Az, Bz);
    }
    s32 dx, dy;
    dx = Bx - Ax;
    dy = B3L_Absf(By - Ay);
    f32 dz = (Bz - Az) / ((f32)dx);
    s32 err = dx / 2;
    s32 ystep;
    if (Ay < By) {
        ystep = 1;
    }
    else {
        ystep = -1;
    }
    /*
    Cut line by x,y range
    */
    if (steep) {
        if (Bx >= RENDER_RESOLUTION_Y) {
            Bx = RENDER_RESOLUTION_Y;
        }

        if (Ax >= RENDER_RESOLUTION_Y) {
            Ax = RENDER_RESOLUTION_Y;
        }

    }
    else {
        if (Bx >= RENDER_RESOLUTION_X) {
            Bx = RENDER_RESOLUTION_X;
        }
        if (Ax >= RENDER_RESOLUTION_X) {
            Ax = RENDER_RESOLUTION_X;
        }
    }

    if ((Ax < 0) || (Ay < 0) || (Ax >= RENDER_RESOLUTION_X) || (Ay >= RENDER_RESOLUTION_Y)||\
    (Bx < 0) || (By < 0) || (Bx >= RENDER_RESOLUTION_X) || (By >= RENDER_RESOLUTION_Y)) {
        for (; Ax <= Bx; Ax++) {
            if (steep) {
                DrawPixelWithTest(color, Ay, Ax, Az, pFrameBuff, pZbuff);
            }
            else {
                DrawPixelWithTest(color, Ax, Ay, Az, pFrameBuff, pZbuff);
              }
            Az = Az + dz;
            err -= dy;
            if (err < 0) {
                Ay += ystep;
                err += dx;
            }
        }
    }
    else {
        for (; Ax <= Bx; Ax++) {
            if (steep) {
                DrawPixel(color, Ay, Ax, Az, pFrameBuff, pZbuff);
            }
            else {
                DrawPixel(color, Ax, Ay, Az, pFrameBuff, pZbuff);
            }
            Az = Az + dz;
            err -= dy;
            if (err < 0) {
                Ay += ystep;
                err += dx;
            }
        }
    }
}

/*---------------------------------------------------------------------------------------
Function: DrawSpaceBitmap

---------------------------------------------------------------------------------------*/
void DrawSpaceBitmap(f32 x0, f32 y0, f32 x1, f32 y1, f32 z,
                      f32 u0, f32 v0, f32 u1, f32 v1,
                      fBuff_t* fbuff, zBuff_t* zbuff, B3L_tex_t* texture, s8 lightFact) {
  s32 x,y, xEnd,yEnd;//current y and the end of the y loop
  if (y0 > y1) {
    _swap_f32_t(y0, y1);_swap_f32_t(v0, v1);
   
  }//make sure y0 <=y1
  if (x0 > x1) {
    _swap_f32_t(x0, x1); _swap_f32_t(u0, u1); 
  }//make sure x0 <= x1
  //if draw area totally out of range, skip following drawing

  if ((y0 >= RENDER_RESOLUTION_Y)||(y1 <0.0f)||(x0>= RENDER_RESOLUTION_X)||(x1<0.0f)||((y1-y0)<=1.0f)||((x1-x0)<=1.0f)) {
      return;
    }
  f32 dy = y1 - y0;
  f32 dx = x1 - x0;
  f32 du = (u1 - u0)/dx;
  f32 dv = (v1 - v0)/dy;
  if (y0 < 0.0f) {
    v0 -= dv * y0;
    y0 = 0.0f;
  }
  if (x0 < 0.0f) {
    u0 -= du * x0;
    x0 = 0.0f;
  }
  s32 sy = B3L_CeilToS(y0);
  s32 sx = B3L_CeilToS(x0);
  u0 += (sx - x0) * du;
  v0 += (sy - y0) * dv;
  yEnd = B3L_MIN((RENDER_RESOLUTION_Y - 1), B3L_CeilToS(y1)-1); 
  xEnd = B3L_MIN((RENDER_RESOLUTION_X - 1), B3L_CeilToS(x1)-1);
  u32 uvShift;
  f32 currentU;
  u8 size = texture[4];
  B3L_tex_t color;
  u8 colorRow;
  s8 colorColumn;
  fBuff_t* pCurrentPixel;
  zBuff_t * pCurrentZ;
  s32 svMultSize;
   u32 iZ = GetZtestValue(z);
  for (y = sy ; y <= yEnd; y++) {
    //draw line
    currentU = u0;
    pCurrentPixel = fbuff+ y * RENDER_X_SHIFT + sx;
    pCurrentZ = zbuff + sx + RENDER_RESOLUTION_X * y;
    //zShift = RENDER_RESOLUTION_X * y + sx;
    svMultSize = B3L_RoundingToS(v0)*size;
    for (x = sx; x <= xEnd; x++) {

      if (iZ<=*pCurrentZ) {
         uvShift = B3L_RoundingToS(currentU) + svMultSize + UV_DATA_SHIFT;
         color = texture[uvShift];
         if (color != 0) {
#ifdef USING_COLOR_LEVEL
           colorRow = color & 0xF0;
           colorColumn = color & 0x0F;
           color = SatToU4(colorColumn + lightFact) + colorRow;
#endif
           *pCurrentPixel = color;
           
           *pCurrentZ = iZ;
           
         }
      }
      pCurrentPixel++;
      pCurrentZ++;
      currentU += du; 
    }
    v0 += dv;
  }

 }
 

void ClearZbuff(zBuff_t* pZbuff, u32 length) {  
    length = length >> 1;

   int32_t i = length & 0x0000000F;
   length = length >> 4;
   u32 *Addr=(u32 *)pZbuff;
   u32 value = 0xFFFFFFFF;
   while (length--) {
     *Addr = value; Addr+=1; *Addr = value; Addr+=1; *Addr = value; Addr+=1;
     *Addr = value; Addr+=1; *Addr = value; Addr+=1; *Addr = value; Addr+=1;
     *Addr = value; Addr+=1; *Addr = value; Addr+=1; *Addr = value; Addr+=1;
     *Addr = value; Addr+=1; *Addr = value; Addr+=1; *Addr = value; Addr+=1;
     *Addr = value; Addr+=1; *Addr = value; Addr+=1; *Addr = value; Addr+=1;
     *Addr = value; Addr+=1;
   }
   switch (i) {
   case 15:*Addr = value; Addr+=1;
   case 14:*Addr = value; Addr+=1;
   case 13:*Addr = value; Addr+=1;
   case 12:*Addr = value; Addr+=1;
   case 11:*Addr = value; Addr+=1;
   case 10:*Addr = value; Addr+=1;
   case 9:*Addr = value; Addr+=1;
   case 8:*Addr = value; Addr+=1;
   case 7:*Addr = value; Addr+=1;
   case 6:*Addr = value; Addr+=1;
   case 5:*Addr = value; Addr+=1;
   case 4:*Addr = value; Addr+=1;
   case 3:*Addr = value; Addr+=1;
   case 2:*Addr = value; Addr+=1;
   case 1:*Addr = value;
   case 0:break;
   }
 }


void ClearFbuff(fBuff_t* pFbuff, u32 length, fBuff_t color) {
#if fBuff_t == u8
  length = length >> 2;
  int32_t i = length & 0x0000000F;
  length = length >> 4;
  u32 *Addr=((u32 *)pFbuff);
  u32 value = color | (color << 8) | (color << 16) | (color << 24);
  while (length--) {
    *Addr = value; Addr+=1; *Addr = value; Addr+=1; *Addr = value; Addr+=1;
    *Addr = value; Addr+=1; *Addr = value; Addr+=1; *Addr = value; Addr+=1;
    *Addr = value; Addr+=1; *Addr = value; Addr+=1; *Addr = value; Addr+=1;
    *Addr = value; Addr+=1; *Addr = value; Addr+=1; *Addr = value; Addr+=1;
    *Addr = value; Addr+=1; *Addr = value; Addr+=1; *Addr = value; Addr+=1;
    *Addr = value; Addr+=1;
  }
  switch (i) {
  case 15:*Addr = value; Addr+=1;
  case 14:*Addr = value; Addr+=1;
  case 13:*Addr = value; Addr+=1;
  case 12:*Addr = value; Addr+=1;
  case 11:*Addr = value; Addr+=1;
  case 10:*Addr = value; Addr+=1;
  case 9:*Addr = value; Addr+=1;
  case 8:*Addr = value; Addr+=1;
  case 7:*Addr = value; Addr+=1;
  case 6:*Addr = value; Addr+=1;
  case 5:*Addr = value; Addr+=1;
  case 4:*Addr = value; Addr+=1;
  case 3:*Addr = value; Addr+=1;
  case 2:*Addr = value; Addr+=1;
  case 1:*Addr = value;
  case 0:break;
  }



#endif
}
