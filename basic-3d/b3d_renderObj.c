#include "b3d_common.h"
#include "b3d_types.h"
#include "b3d_math.h"
#include "b3d_config.h"
#include "b3d.h"
#include <stdio.h>
#pragma GCC optimize("-O3")


/*-------------------------------------------------------------------------------------------------
Private Function declaration
-------------------------------------------------------------------------------------------------*/
__STATIC_FORCEINLINE void  Vect3Xmat4ToScreen4(vect3_t* pV, mat4_t* pMat, vect4_t* pResult);
__STATIC_FORCEINLINE void Vect3Xmat4WithTest_f(vect3_t* pV, mat4_t* pMat, screen3f_t* pResult);
__STATIC_FORCEINLINE bool Vect4BoundTest(vect4_t* pV);
__STATIC_FORCEINLINE u32 BoundBoxTest(f32* Boundbox, mat4_t* pMat);
__STATIC_FORCEINLINE u32 BitmapBoundBoxTest(B3LObj_t* pObj, mat4_t* pMat);
__STATIC_FORCEINLINE bool TriangleFaceToViewer_f(f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2);
__STATIC_FORCEINLINE s8  CalLightFactor(f32 normalDotLight, f32  lightFactor0, f32  lightFactor1);
static void ClipLineInScreen(screen3f_t* a, screen3f_t* b);
static void ClipTexPoint(u32 v0, u32 v1, u32 v2, u32 i0, u32 i1, u32 i2, f32 nearPlane, vect3_t* pVect, mat4_t* pMat, u8* pUV, B3L_clip_t* pC0, B3L_clip_t* pC1);
static void ClipColorPoint(u32 v0, u32 v1, u32 v2, f32 nearPlane, vect3_t* pVect, mat4_t* pMat, B3L_clip_t* pC0, B3L_clip_t* pC1);
static void RenderPolygon(B3LObj_t* pObj, render_t* pRender, mat4_t* pMat, u32 bboxResult);
static void RenderTexMesh(B3LObj_t* pObj, render_t* pRender, mat4_t* pO2CMat, mat4_t* pO2WMat, u32 renderLevel, u32 bboxResult);
static void RenderColorMesh(B3LObj_t* pObj, render_t* pRender, mat4_t* pO2CMat, mat4_t* pO2WMat, u32 renderLevel, u32 bboxResult);
static void RenderBitmap(B3LObj_t* pObj, render_t* pRender, mat4_t* pMat);
static void UpdateParticleGenerator(B3LObj_t* pObj, render_t* pRender, mat4_t* O2Cmat, mat4_t* O2WMat,u32 timePassed);
static vect3_t* MeshGetNormal(B3L_Mesh_t* start, u16 vectNum, u16 triNum);
static vect3_t* GetPolygonVect(B3L_Polygon_t* pPoly);
static u16* GetPolygonLin(B3L_Polygon_t* pPoly, u16 vectNum);
static vect3_t * GetVect(B3L_Mesh_t *start,u16 vectNum, u16 triNum);
static u16 GetID(B3L_Mesh_t* start);
static u16 GetTriNum(B3L_Mesh_t* start);
static u16 GetLinNum(B3L_Polygon_t* start);
static u16 GetVectNum(B3L_Mesh_t* start);
static u16 GetUVNum(B3L_Mesh_t* start);
static u16 GetNormalNum(B3L_Mesh_t* start);
static u8 * GetUV(B3L_Mesh_t *start,u16 vectNum, u16 triNum);
static u16 * GetTriIdx(B3L_Mesh_t *start,u16 vectNum, u16 triNum);
static f32 * GetBoundBox(B3L_Mesh_t *start);
static void DrawNearPlaneClipTriTexture(u32 cullingState, fBuff_t* pFrameBuff, zBuff_t* pZBuff, f32 nearPlane, 
                                        u32 vect0Idx, u32 vect1Idx, u32 vect2Idx,u32 clipCheck, u32 index,
                                        vect3_t* pVectSource, screen3f_t* pVectTarget,mat4_t* pMat, u8* pUV, 
                                         B3L_tex_t* pTexture, s8 lightValue);
static void DrawNearPlaneClipTriColor(u32 cullingState, fBuff_t* pFrameBuff, zBuff_t* pZBuff, f32 nearPlane,
                                        u32 vect0Idx, u32 vect1Idx, u32 vect2Idx, u32 clipCheck, 
                                        vect3_t* pVectSource, screen3f_t* pVectTarget, mat4_t* pMat, 
                                        B3L_tex_t color, s8 lightValue);
/*-------------------------------------------------------------------------------------------------
Private Functions 
-------------------------------------------------------------------------------------------------*/


__STATIC_FORCEINLINE void  Vect3Xmat4ToScreen4(vect3_t* pV, mat4_t* pMat, vect4_t* pResult) {
    f32 x = pV->x; f32 y = pV->y; f32 z = pV->z;
    f32 rx, ry, rz, rw;
    //u32 testResult = 0;
#define dotCol(col)\
        ((x*(pMat->m##col##0)) +\
        (y*(pMat->m##col##1)) +\
        (z*(pMat->m##col##2)) +\
        (pMat->m##col##3))

    rx = dotCol(0);
    ry = dotCol(1);
    rz = dotCol(2);
    rw = dotCol(3);


    f32 factor = 1.0f / (rw);//prevent div zero error
    f32 screenX = (HALF_RESOLUTION_X + rx * factor * HALF_RESOLUTION_X);
    f32 screenY = (HALF_RESOLUTION_Y - ry * factor * HALF_RESOLUTION_Y);
    rz = rz * factor;
    pResult->w = rw;
    pResult->x = screenX;
    pResult->y = screenY;
    pResult->z = rz;
#undef dotCol

}



__STATIC_FORCEINLINE void  Vect3Xmat4WithTest_f(vect3_t* pV, mat4_t* pMat, screen3f_t* pResult) {
    f32 x = pV->x; f32 y = pV->y; f32 z = pV->z;
    f32 rx, ry, rz, rw;
    u32 testResult = 0;
#define dotCol(col)\
        ((x*(pMat->m##col##0)) +\
        (y*(pMat->m##col##1)) +\
        (z*(pMat->m##col##2)) +\
        (pMat->m##col##3))

    rx = dotCol(0);
    ry = dotCol(1);
    rz = dotCol(2);
    rw = dotCol(3);

    if (rz <= 0.0f) {//if the near plane clip, then don't do the calculation, set bit and return directly
        B3L_SET(testResult, B3L_NEAR_PLANE_CLIP);
        pResult->test = testResult;
        return;
    }
    else if ((rx < rw) && (rx > -rw) && (ry < rw) && (ry > -rw) && (rz < rw)) {
        B3L_SET(testResult, B3L_IN_SPACE);
    }
    f32 factor = 1.0f / (rw);//prevent div zero error
    f32 screenX = (HALF_RESOLUTION_X + rx * factor * HALF_RESOLUTION_X);
    f32 screenY = (HALF_RESOLUTION_Y - ry * factor * HALF_RESOLUTION_Y);
    rz = rz * factor;
    pResult->test = testResult;
    pResult->x = screenX;
    pResult->y = screenY;
    pResult->z = rz;
#undef dotCol

}


__STATIC_FORCEINLINE bool Vect4BoundTest(vect4_t* pV) {
    f32 x = pV->x; f32 y = pV->y; f32 z = pV->z; f32 w = pV->w;
    if ((x <= w) && (x >= -w) && (y <= w) && (y >= -w) && (z > 0) && (z <= w)) {
        return true;
    }
    else {
        return false;
    }

}




static vect3_t* MeshGetNormal(B3L_Mesh_t* start, u16 vectNum, u16 triNum) {
    u8* u8Start = (u8*)start;
    u16 uvNum = GetUVNum(start);
    vect3_t* pNormal;
    if (uvNum != 0) {
        //this mesh array has uv section
        pNormal = (vect3_t*)(u8Start + MESH_HEAD_SHIFT + 24 + ((u32)vectNum) * 12 + ((u32)triNum) * 12);
    }
    else {
        if (triNum & 0x0001) {
            //odd num, add 2 dummy bytes to align 4 for the f32 normal 
            pNormal = (vect3_t*)(u8Start + MESH_HEAD_SHIFT + 24 + ((u32)vectNum) * 12 + ((u32)triNum) * 6 + 2);
        }
        else {
            pNormal = (vect3_t*)(u8Start + MESH_HEAD_SHIFT + 24 + ((u32)vectNum) * 12 + ((u32)triNum) * 6);
        }
    }
    return pNormal;
}


static vect3_t* GetPolygonVect(B3L_Polygon_t* pPoly) {
    u8* u8Start = (u8*)pPoly;
    vect3_t* pVect = (vect3_t*)(u8Start + MESH_HEAD_SHIFT + 24);
    return pVect;
}
static u16* GetPolygonLin(B3L_Polygon_t* pPoly, u16 vectNum) {
    u8* u8Start = (u8*)pPoly;
    u16* pLin = (u16*)(u8Start + MESH_HEAD_SHIFT + 24 + ((u32)vectNum) * 12);
    return pLin;
}

static u16 GetID(B3L_Mesh_t* start) {
    return ((u32*)(start))[0];
}

static u16 GetTriNum(B3L_Mesh_t* start) {
    return ((u16*)(start))[3];
}

static u16 GetLinNum(B3L_Polygon_t* start) {
    return ((u16*)(start))[3];
}
static u16 GetVectNum(B3L_Mesh_t* start) {
    return ((u16*)(start))[2];
}
static u16 GetUVNum(B3L_Mesh_t * start) {
    return ((u16*)(start))[4];
}

static u16 GetNormalNum(B3L_Mesh_t* start) {
    return ((u16*)(start))[5];
}

static vect3_t * GetVect(B3L_Mesh_t *start,u16 vectNum, u16 triNum){
    u8 * u8Start = (u8 *)start;
    vect3_t* pVect = (vect3_t *)(u8Start + MESH_HEAD_SHIFT + 24);
    return pVect;
}
static u8 * GetUV(B3L_Mesh_t *start,u16 vectNum, u16 triNum){
    u8 * u8Start = (u8 *)start;
    u8* pUV = u8Start + MESH_HEAD_SHIFT + 24 + ((u32)vectNum) * 12 + ((u32)triNum) * 6;
    return pUV;
}
static u16 * GetTriIdx(B3L_Mesh_t *start,u16 vectNum, u16 triNum){
    u8 * u8Start = (u8 *)start;
    u16* pTri = (u16 *)(u8Start + MESH_HEAD_SHIFT + 24 + ((u32)vectNum)*12);
    return pTri;
}

static f32 * GetBoundBox(B3L_Mesh_t *start){
    u8 * u8Start = (u8 *)start;
    f32* pBound = (f32 *)(u8Start + MESH_HEAD_SHIFT);
    return pBound;
}

static void ClipLineInScreen(screen3f_t* a, screen3f_t* b) { 
    screen3f_t* temp;
    //test left x
    s32 cax = B3L_CeilToS(a->x);
    s32 cbx = B3L_CeilToS(b->x);
    s32 cay = B3L_CeilToS(a->y);
    s32 cby = B3L_CeilToS(b->y);
    f32 rry = (f32)(RENDER_RESOLUTION_Y - 1);
    f32 rrx = (f32)(RENDER_RESOLUTION_X - 1);

    if ((a->x) > (b->x)) {
        temp = b; b = a;a = temp;
    }
    f32 ax = a->x;f32 ay = a->y;f32 az = a->z;
    f32 bx = b->x;f32 by = b->y;f32 bz = b->z;
    f32 dx, dy;
    f32 factor;
    if (cax != cbx) {
        if ((ax) < 0.0f) {
                //do clip
            dx = bx - ax;
            factor = 1.0f / dx;
            ay = ay - (by - ay) * factor * ax;
            az = az - (bz - az) * factor * ax;
            ax = 0.0f;
        }
    
        if ((bx) > rrx) {
                //do clip
            dx = bx - ax;
            factor = 1.0f / dx;
            by = by - (by - ay) * factor * (bx - rrx);
            bz = bz - (bz - az) * factor * (bx - rrx);
            bx = rrx;
        }
    }
    
    if (cay != cby) {
        if ((ay) > (by)) {
            _swap_f32_t(ax, bx); _swap_f32_t(ay, by); _swap_f32_t(az, bz);
        }
        if (((ay) < 0.0f)&&(cay != cby)) {
            //do clip
            dy = by - ay;
            factor = 1.0f / dy;
            ax =ax - (bx - ax) * factor * ay;
            az =az - (bz - az) * factor * ay;
            ay = 0.0f;
        }
    
        if ((by) > rry) {
            //do clip
            dy = by - ay;
            factor = 1.0f / dy;
            bx = bx - (bx - ax) * factor * (by - rry);
            bz = bz - (bz - az) * factor * (by - rry);
            by = rry;
        }
    }
    a->x = ax; a->y = ay; a->z = az;
    b->x = bx; b->y = by; b->z = bz;
}


static void ClipColorPoint(u32 v0, u32 v1, u32 v2,  f32 nearPlane, vect3_t* pVect, mat4_t* pMat, B3L_clip_t* pC0, B3L_clip_t* pC1) {
    vect4_t vect0, vect1, vect2;
    B3L_Vect3Xmat4(pVect + v0, pMat, &vect0);
    B3L_Vect3Xmat4(pVect + v1, pMat, &vect1);
    B3L_Vect3Xmat4(pVect + v2, pMat, &vect2);
    f32 rwFactor = 1.0f / nearPlane;
    f32 factor = (vect0.w - nearPlane) / (vect0.w - vect1.w);
    pC0->x = HALF_RESOLUTION_X + HALF_RESOLUTION_X * (vect0.x - factor * (vect0.x - vect1.x)) * rwFactor;
    pC0->y = HALF_RESOLUTION_Y - HALF_RESOLUTION_Y * (vect0.y - factor * (vect0.y - vect1.y)) * rwFactor;

    factor = (vect0.w - nearPlane) / (vect0.w - vect2.w);
    pC1->x = HALF_RESOLUTION_X + HALF_RESOLUTION_X * (vect0.x - factor * (vect0.x - vect2.x)) * rwFactor;
    pC1->y = HALF_RESOLUTION_Y - HALF_RESOLUTION_Y * (vect0.y - factor * (vect0.y - vect2.y)) * rwFactor;

}



static void ClipTexPoint(u32 v0, u32 v1, u32 v2, u32 i0, u32 i1, u32 i2, f32 nearPlane,
                         vect3_t* pVect, mat4_t* pMat, u8* pUV, B3L_clip_t* pC0, B3L_clip_t* pC1) {
    vect4_t vect0, vect1, vect2;
    B3L_Vect3Xmat4(pVect + v0, pMat, &vect0);
    B3L_Vect3Xmat4(pVect + v1, pMat, &vect1);
    B3L_Vect3Xmat4(pVect + v2, pMat, &vect2);
    f32 rwFactor = 1.0f / nearPlane;
    f32 factor = (vect0.w - nearPlane) / (vect0.w - vect1.w);
    pC0->x = HALF_RESOLUTION_X + HALF_RESOLUTION_X * (vect0.x - factor * (vect0.x - vect1.x)) * rwFactor;
    pC0->y = HALF_RESOLUTION_Y - HALF_RESOLUTION_Y * (vect0.y - factor * (vect0.y - vect1.y)) * rwFactor;
    f32 mapu0 = (f32)(pUV[i0]);
    f32 mapu1 = (f32)(pUV[i1]);
    f32 mapv0 = (f32)(pUV[i0 + 1]);
    f32 mapv1 = (f32)(pUV[i1 + 1]);
    pC0->u = mapu0 - factor * (mapu0 - mapu1);
    pC0->v = mapv0 - factor * (mapv0 - mapv1);
    factor = (vect0.w - nearPlane) / (vect0.w - vect2.w);
    pC1->x = HALF_RESOLUTION_X + HALF_RESOLUTION_X * (vect0.x - factor * (vect0.x - vect2.x)) * rwFactor;
    pC1->y = HALF_RESOLUTION_Y - HALF_RESOLUTION_Y * (vect0.y - factor * (vect0.y - vect2.y)) * rwFactor;
    f32 mapu2 = (f32)(pUV[i2]);
    f32 mapv2 = (f32)(pUV[i2 + 1]);
    pC1->u = mapu0 - factor * (mapu0 - mapu2);
    pC1->v = mapv0 - factor * (mapv0 - mapv2);
}




__STATIC_FORCEINLINE bool TriangleFaceToViewer_f(f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2) {
    f32 winding =
        (x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0);
    // ^ cross product for points with z == 0
    return winding <= 0.0f ? true : false;
}


#define BOX_IN_SPACE             (16u)
#define BOX_NEAR_PLANE_CLIP      (0u)

__STATIC_FORCEINLINE  u32 vectBoundTest(f32 x, f32 y, f32 z, mat4_t *pMat) {
    
    f32 rx, ry, rz, rw;
    u32 testResult = 0;
#define dotCol(col)\
        ((x*(pMat->m##col##0)) +\
        (y*(pMat->m##col##1)) +\
        (z*(pMat->m##col##2)) +\
        (pMat->m##col##3))

    rx = dotCol(0);
    ry = dotCol(1);
    rz = dotCol(2);
    rw = dotCol(3);
#undef dotCol
//get the vect in clip space

    if (rz < 0.0f) {//if the near plane clip, then don't do the calculation, set bit and return directly
        B3L_SET(testResult, BOX_NEAR_PLANE_CLIP);
    }
    else if ((rx < rw) && (rx > -rw) && (ry < rw) && (ry > -rw) && (rz < rw)) {
        B3L_SET(testResult, BOX_IN_SPACE);
    }
    return testResult;
}

__STATIC_FORCEINLINE u32 BoundBoxTest(f32* Boundbox, mat4_t* pMat) {
    u32 result;
    f32 maxX = Boundbox[0]; f32 maxY = Boundbox[1]; f32 maxZ = Boundbox[2];
    f32 minX = Boundbox[3]; f32 minY = Boundbox[4]; f32 minZ = Boundbox[5];
    result = vectBoundTest(maxX, maxY, maxZ, pMat);
    result += vectBoundTest(maxX, maxY, minZ, pMat);
    result += vectBoundTest(maxX, minY, minZ, pMat);
    result += vectBoundTest(minX, minY, minZ, pMat);
    result += vectBoundTest(minX, minY, maxZ, pMat);
    result += vectBoundTest(minX, maxY, maxZ, pMat);
    result += vectBoundTest(maxX, minY, maxZ, pMat);
    result += vectBoundTest(minX, maxY, minZ, pMat);
    return result;
}

__STATIC_FORCEINLINE s8  CalLightFactor(f32 normalDotLight, f32  lightFactor0, f32  lightFactor1) {
    normalDotLight += lightFactor0;
    normalDotLight *= lightFactor1;
    return ((s8)B3L_RoundingToS(normalDotLight));

}


static void RenderPolygon(B3LObj_t* pObj, render_t* pRender, mat4_t* pMat, u32 bboxResult) {
    int32_t i;
    //s8 lightValue = ((s8)((pObj->state & OBJ_SPECIAL_LIGHT_MASK) >> OBJ_SPECIAL_LIGHT_SHIFT)) - 16;
    s8 lightValue = GET_OBJ_FIX_LIGHT_VALUE(pObj);
    B3L_Polygon_t *pPoly = (B3L_Polygon_t*)(pObj->pResource0);
    B3L_tex_t* pColor = (B3L_tex_t*)(pObj->pResource1)+ TEXTURE_HEAD_SHIFT;
    u16 vectNum = GetVectNum((B3L_Mesh_t *)pPoly);
    u16 linNum = GetLinNum(pPoly);
    vect4_t* pVectTarget = pRender->pVectBuff;
    fBuff_t* pFrameBuff = pRender->pFrameBuff;
    zBuff_t* pZBuff = pRender->pZBuff;
    u32 state = pObj->state;
    vect3_t* pVectSource = GetPolygonVect(pPoly);
    u16* pLine = GetPolygonLin(pPoly, vectNum);
    //convert all the vects
    if ((bboxResult >> BOX_IN_SPACE) == 8) {
        for (i = vectNum - 1; i >= 0; i--) {
            Vect3Xmat4ToScreen4(pVectSource + i, pMat, pVectTarget + i);
        }
    }
    else {
        for (i = vectNum - 1; i >= 0; i--) {
            Vect3Xmat4WithTest_f(pVectSource + i, pMat, (screen3f_t*)pVectTarget + i);
        }
    }
    u16 aIdx, bIdx;
    if ((bboxResult >> BOX_IN_SPACE) == 8) {
    //fully in clip space
        for (i = linNum-1; i >=0; i--) {
            //draw all line directly
            aIdx = pLine[i * 2];
            bIdx = pLine[i * 2 + 1];
            f32 ax = pVectTarget[aIdx].x;
            f32 ay = pVectTarget[aIdx].y;
            f32 az = pVectTarget[aIdx].z;
            f32 bx = pVectTarget[bIdx].x;
            f32 by = pVectTarget[bIdx].y;
            f32 bz = pVectTarget[bIdx].z;
            DrawSpaceLine(ax, ay, az, bx, by, bz, pColor[i],pFrameBuff,pZBuff, lightValue);
        }
    }
    else {
        for (i = linNum - 1; i >= 0; i--) {
            aIdx = pLine[i * 2];
            bIdx = pLine[i * 2 + 1];
            u32 result0 = ((screen3f_t*)pVectTarget)[aIdx].test;
            u32 result1 = ((screen3f_t*)pVectTarget)[bIdx].test;
            u32 triVisable = result0 + (result1 << 1);
            //u32 inSpaceCheck = triVisable >> 3;
            u32 clipCheck = triVisable & 0x00000007;
            if (clipCheck ==0) {// line not cross the near plane
                f32 ax = pVectTarget[aIdx].x;
                f32 ay = pVectTarget[aIdx].y;
                f32 az = pVectTarget[aIdx].z;
                f32 bx = pVectTarget[bIdx].x;
                f32 by = pVectTarget[bIdx].y;
                f32 bz = pVectTarget[bIdx].z;
                DrawSpaceLine(ax,ay,az,bx,by,bz, pColor[i], pFrameBuff, pZBuff, lightValue);
            }
        }
    }
}


static void RenderColorMesh(B3LObj_t* pObj, render_t* pRender, mat4_t* pO2CMat, mat4_t* pO2WMat, u32 renderLevel, u32 bboxResult) {
    int32_t i;
    s8 lightValue = 0;
    B3L_Mesh_t* pMesh = (B3L_Mesh_t*)(pObj->pResource0);
    B3L_tex_t* pColor = (B3L_tex_t*)(pObj->pResource1)+ TEXTURE_HEAD_SHIFT;
    u16 vectNum = GetVectNum(pMesh);
    u16 triNum = GetTriNum(pMesh);
    vect3_t* pVectSource = GetVect(pMesh, vectNum, triNum);
    u16* pTriIdx = GetTriIdx(pMesh, vectNum, triNum);
    vect4_t* pVectTarget = pRender->pVectBuff;
    fBuff_t* pFrameBuff = pRender->pFrameBuff;
    zBuff_t* pZBuff = pRender->pZBuff;
    u32 state = pObj->state;
    if ((bboxResult >> BOX_IN_SPACE) == 8) {
        for (i = vectNum - 1; i >= 0; i--) {
            Vect3Xmat4ToScreen4(pVectSource + i, pO2CMat, pVectTarget + i);
        }
    }
    else {
        for (i = vectNum - 1; i >= 0; i--) {
            //Vect3Xmat4ToScreen4(pVectSource + i, pMat, pVectTarget + i);
            Vect3Xmat4WithTest_f(pVectSource + i, pO2CMat, (screen3f_t*)pVectTarget + i);
        }
    }
#ifdef USING_COLOR_LEVEL
    if (B3L_TEST(state, OBJ_SPECIAL_LIGHT_VALUE)) {
        lightValue = ((s8)((state & OBJ_SPECIAL_LIGHT_MASK) >> OBJ_SPECIAL_LIGHT_SHIFT)) - 16;
    }
    else {
        lightValue = pRender->lvl1Light;
    }
#endif
#ifdef USING_LIGHT   
    if ((GetNormalNum(pMesh) == 0) && (renderLevel == 0)) {
        renderLevel = 1; // no normal there! downgrade the render level
    }
    vect3_t* pNormal;
    vect4_t pointToLightVect;
    f32 normalFact, normalDotLight;
    f32 lightFactor0 = 0.0f;
    f32 lightFactor1 = 0.0f;
    vect3_t normalVect;
    if (renderLevel == 0) {
        pNormal = MeshGetNormal(pMesh, vectNum, triNum);
        B3L_InvertMat4(pO2WMat, pO2WMat);//do the invert operation on o2w matrix
        if (B3L_TEST(pRender->light.state, LIGHT_TYPE_BIT) == POINT_LIGHT) {
            //dot light, calculate the vect point  to light from obj (both already in camera space)
            B3L_Vect3Xmat4(&(pRender->light.lightVect), pO2WMat, &pointToLightVect);
            //normalize the vect       
        }
        else {
            //parallel light, the point to light vect is already in camera space
            B3L_Vect3Xmat3inMat4Format(&(pRender->light.lightVect), pO2WMat, &pointToLightVect);
            //normalize the vect
        }

        B3L_Vect3Normalize((vect3_t *)&pointToLightVect, (vect3_t*)&pointToLightVect);

        lightFactor0 = pRender->light.factor_0;
        lightFactor1 = pRender->light.factor_1;
    }


#endif
    u32 cullingState = (B3L_TEST(state, OBJ_BACKFACE_CULLING)) >> OBJ_BACKFACE_CULLING;
    u32 vect0Idx, vect1Idx, vect2Idx;
    f32 x0, y0, x1, y1, x2, y2;
    B3L_tex_t color;
    if ((bboxResult >> BOX_IN_SPACE) == 8) {

        for (i = triNum - 1; i >= 0; i--) {
            //printf("%d\n",i);
            vect0Idx = pTriIdx[i * 3];
            vect1Idx = pTriIdx[i * 3 + 1];
            vect2Idx = pTriIdx[i * 3 + 2];
            x0 = pVectTarget[vect0Idx].x;
            y0 = pVectTarget[vect0Idx].y;
            x1 = pVectTarget[vect1Idx].x;
            y1 = pVectTarget[vect1Idx].y;
            x2 = pVectTarget[vect2Idx].x;
            y2 = pVectTarget[vect2Idx].y;
            if (cullingState) {
                bool backFaceCullingResult = TriangleFaceToViewer_f(x0, y0, x1, y1, x2, y2);
                if (backFaceCullingResult) {
                    continue;
                }
            }
#ifdef USING_LIGHT
            if (renderLevel == 0) {
                vect3_t* currentNormal = pNormal + i;
                normalDotLight = currentNormal->x * pointToLightVect.x + currentNormal->y * pointToLightVect.y + currentNormal->z * pointToLightVect.z;
                lightValue = CalLightFactor(normalDotLight, lightFactor0, lightFactor1);
            }
#endif
            color = pColor[i];
            DrawTriangleColor(
                x0, y0,  pVectTarget[vect0Idx].z,
                x1, y1,  pVectTarget[vect1Idx].z,
                x2, y2,  pVectTarget[vect2Idx].z,
                pFrameBuff, pZBuff, color, lightValue);

        }
    }
    else {
        //partly out of range objs
#if B3L_DO_NEAR_PLANE_CLIP == 1
        B3L_clip_t  c0, c1;
        f32 nearPlane = pRender->nearPlane;
#endif
        for (i = triNum - 1; i >= 0; i--) {
            vect0Idx = pTriIdx[i * 3];
            vect1Idx = pTriIdx[i * 3 + 1];
            vect2Idx = pTriIdx[i * 3 + 2];
            x0 = pVectTarget[vect0Idx].x;
            y0 = pVectTarget[vect0Idx].y;
            x1 = pVectTarget[vect1Idx].x;
            y1 = pVectTarget[vect1Idx].y;
            x2 = pVectTarget[vect2Idx].x;
            y2 = pVectTarget[vect2Idx].y;

            screen3f_t* pVectTestTarget = (screen3f_t*)pVectTarget;
            u32 result0 = pVectTestTarget[vect0Idx].test;
            u32 result1 = pVectTestTarget[vect1Idx].test;
            u32 result2 = pVectTestTarget[vect2Idx].test;

            u32 triVisable = result0 + (result1 << 1) + (result2 << 2);
            u32 inSpaceCheck = triVisable >> 3;
            u32 clipCheck = triVisable & 0x00000007;
            if (inSpaceCheck) {// at least one point inside the clip space
#ifdef USING_LIGHT
                if (renderLevel == 0) {
                    vect3_t* currentNormal = pNormal + i;
                    normalDotLight = currentNormal->x * pointToLightVect.x + currentNormal->y * pointToLightVect.y + currentNormal->z * pointToLightVect.z;
                    lightValue = CalLightFactor(normalDotLight, lightFactor0, lightFactor1);
                }
#endif
                color = pColor[i];
                if (clipCheck == 0) {
                    if (cullingState) {
                        bool backFaceCullingResult = TriangleFaceToViewer_f(x0, y0, x1, y1, x2, y2);
                        if (backFaceCullingResult) {
                            continue;
                        }
                    }
                    //draw tri with check for no near plane clip necessary!
                    
                    DrawTriangleColor(
                        x0, y0, pVectTarget[vect0Idx].z,
                        x1, y1,  pVectTarget[vect1Idx].z,
                        x2, y2,  pVectTarget[vect2Idx].z,
                        pFrameBuff, pZBuff, color, lightValue);
                }
#if B3L_DO_NEAR_PLANE_CLIP == 1
                else {
                    DrawNearPlaneClipTriColor(cullingState, pFrameBuff, pZBuff, nearPlane, vect0Idx, vect1Idx, vect2Idx,
                        clipCheck, pVectSource, pVectTestTarget, pO2CMat, color, lightValue);
                }
#endif                
            }
        }
    }
}



static void RenderTexMesh(B3LObj_t* pObj, render_t* pRender, mat4_t* pO2CMat, mat4_t* pO2WMat, u32 renderLevel, u32 bboxResult) {
    int32_t i;
    s8 lightValue = 0;
    B3L_Mesh_t* pMesh = (B3L_Mesh_t*)(pObj->pResource0);
    B3L_tex_t* pTexture = (B3L_tex_t*)(pObj->pResource1);
    u16 vectNum = GetVectNum(pMesh);
    u16 triNum = GetTriNum(pMesh);
    vect3_t* pVectSource = GetVect(pMesh, vectNum, triNum);
    u16* pTriIdx = GetTriIdx(pMesh, vectNum, triNum);
    u8* pUV = GetUV(pMesh, vectNum, triNum);
    vect4_t* pVectTarget = pRender->pVectBuff;
    //convert all vect into clip space
    fBuff_t* pFrameBuff = pRender->pFrameBuff;
    zBuff_t* pZBuff = pRender->pZBuff;
    u32 state = pObj->state;
    if ((bboxResult >> BOX_IN_SPACE) == 8) {
        for (i = vectNum - 1; i >= 0; i--) {
            Vect3Xmat4ToScreen4(pVectSource + i, pO2CMat, pVectTarget + i);
        }
    }
    else {
        for (i = vectNum - 1; i >= 0; i--) {
            Vect3Xmat4WithTest_f(pVectSource + i, pO2CMat, (screen3f_t*)pVectTarget + i);
        }
    }

#ifdef USING_COLOR_LEVEL
    if (B3L_TEST(state, OBJ_SPECIAL_LIGHT_VALUE)) {
        lightValue = ((s8)((state & OBJ_SPECIAL_LIGHT_MASK) >> OBJ_SPECIAL_LIGHT_SHIFT)) - 16;
    }
    else {
        lightValue = pRender->lvl1Light;
    }
#endif
#ifdef USING_LIGHT   
    if ((GetNormalNum(pMesh) == 0)&&(renderLevel == 0)) {
        renderLevel = 1; // no normal there! downgrade the render level
    }
    vect3_t* pNormal;
    vect4_t pointToLightVect;
    f32 normalFact, normalDotLight;
    f32 lightFactor0, lightFactor1;
    vect3_t normalVect;
    if (renderLevel == 0) {
        pNormal = MeshGetNormal(pMesh, vectNum, triNum);
            B3L_InvertMat4(pO2WMat, pO2WMat);//do the invert operation on o2w matrix
        if (B3L_TEST(pRender->light.state, LIGHT_TYPE_BIT) == POINT_LIGHT) {
            //dot light, calculate the vect point  to light from obj (both already in camera space)
            
            B3L_Vect3Xmat4(&(pRender->light.lightVect), pO2WMat, &pointToLightVect);
            //normalize the vect
            
        }
        else {
            //parallel light, the point to light vect is already in camera space
            B3L_Vect3Xmat3inMat4Format(&(pRender->light.lightVect), pO2WMat, &pointToLightVect);
            //normalize the vect
            
        }

        B3L_Vect3Normalize((vect3_t*)&pointToLightVect, (vect3_t*)&pointToLightVect);

        lightFactor0 = pRender->light.factor_0;
        lightFactor1 = pRender->light.factor_1;
    }

#endif
    u32 cullingState = (B3L_TEST(state, OBJ_BACKFACE_CULLING)) >> OBJ_BACKFACE_CULLING;
    u32 vect0Idx, vect1Idx, vect2Idx;
    f32 x0, y0, x1, y1, x2, y2;
    if ((bboxResult >> BOX_IN_SPACE) == 8) {

        for (i = triNum - 1; i >= 0; i--) {
            //printf("%d\n",i);
            vect0Idx = pTriIdx[i * 3];
            vect1Idx = pTriIdx[i * 3 + 1];
            vect2Idx = pTriIdx[i * 3 + 2];
            x0 = pVectTarget[vect0Idx].x;
            y0 = pVectTarget[vect0Idx].y;
            x1 = pVectTarget[vect1Idx].x;
            y1 = pVectTarget[vect1Idx].y;
            x2 = pVectTarget[vect2Idx].x;
            y2 = pVectTarget[vect2Idx].y;
            if (cullingState) {
                bool backFaceCullingResult = TriangleFaceToViewer_f(x0, y0, x1, y1, x2, y2);
                if (backFaceCullingResult) {
                    continue;
                }
            }
#ifdef USING_LIGHT
            if (renderLevel == 0) {
                //printf("index %d, normal index %d\n", i, pNormalIdx[i]);
                //printf("lightValue %d\n", normalLightBuff[pNormalIdx[i]]);
                //lightValue =normalLightBuff[pNormalIdx[i]];
                //B3L_Vect3XMat3(pNormal + i, pRmat, &normalVect);
                //dot multi light and normalvect to get the light factor
                vect3_t* currentNormal = pNormal + i;
                normalDotLight = currentNormal->x * pointToLightVect.x + currentNormal->y * pointToLightVect.y + currentNormal->z * pointToLightVect.z;
                lightValue = CalLightFactor(normalDotLight, lightFactor0, lightFactor1);
            }
#endif

            DrawTriangleTexture(
                x0, y0, (f32)(pUV[i * 6]), (f32)(pUV[i * 6 + 1]), pVectTarget[vect0Idx].z,
                x1, y1, (f32)(pUV[i * 6 + 2]), (f32)(pUV[i * 6 + 3]), pVectTarget[vect1Idx].z,
                x2, y2, (f32)(pUV[i * 6 + 4]), (f32)(pUV[i * 6 + 5]), pVectTarget[vect2Idx].z,
                pFrameBuff, pZBuff, pTexture, lightValue);

        }
    }
    else {
        //partly out of range objs
#if B3L_DO_NEAR_PLANE_CLIP == 1
        B3L_clip_t  c0, c1;
        f32 nearPlane = pRender->nearPlane;
#endif
        for (i = triNum - 1; i >= 0; i--) {
            vect0Idx = pTriIdx[i * 3];
            vect1Idx = pTriIdx[i * 3 + 1];
            vect2Idx = pTriIdx[i * 3 + 2];
            x0 = pVectTarget[vect0Idx].x;
            y0 = pVectTarget[vect0Idx].y;
            x1 = pVectTarget[vect1Idx].x;
            y1 = pVectTarget[vect1Idx].y;
            x2 = pVectTarget[vect2Idx].x;
            y2 = pVectTarget[vect2Idx].y;

            screen3f_t* pVectTestTarget = (screen3f_t*)pVectTarget;
            u32 result0 = pVectTestTarget[vect0Idx].test;
            u32 result1 = pVectTestTarget[vect1Idx].test;
            u32 result2 = pVectTestTarget[vect2Idx].test;

            u32 triVisable = result0 + (result1 << 1) + (result2 << 2);
            u32 inSpaceCheck = triVisable >> 3;
            u32 clipCheck = triVisable & 0x00000007;
            if (inSpaceCheck) {// at least one point inside the clip space
#ifdef USING_LIGHT
                if (renderLevel == 0) {
                    vect3_t* currentNormal = pNormal + i;
                    normalDotLight = currentNormal->x * pointToLightVect.x + currentNormal->y * pointToLightVect.y + currentNormal->z * pointToLightVect.z;
                    lightValue = CalLightFactor(normalDotLight, lightFactor0, lightFactor1);
                }
#endif
                if (clipCheck == 0) {
                    if (cullingState) {
                        bool backFaceCullingResult = TriangleFaceToViewer_f(x0, y0, x1, y1, x2, y2);
                        if (backFaceCullingResult) {
                            continue;
                        }
                    }
                    //draw tri with check for no near plane clip necessary!
                    DrawTriangleTexture(
                        x0, y0, (f32)(pUV[i * 6]), (f32)(pUV[i * 6 + 1]), pVectTarget[vect0Idx].z,
                        x1, y1, (f32)(pUV[i * 6 + 2]), (f32)(pUV[i * 6 + 3]), pVectTarget[vect1Idx].z,
                        x2, y2, (f32)(pUV[i * 6 + 4]), (f32)(pUV[i * 6 + 5]), pVectTarget[vect2Idx].z,
                        pFrameBuff, pZBuff, pTexture, lightValue);
                }
#if B3L_DO_NEAR_PLANE_CLIP == 1
                else {
                    DrawNearPlaneClipTriTexture(cullingState,pFrameBuff,pZBuff,nearPlane, vect0Idx, vect1Idx, vect2Idx,
                        clipCheck, i, pVectSource, pVectTestTarget, pO2CMat, pUV, pTexture, lightValue);
                }
#endif                
            }
        }
    }
}

static void DrawNearPlaneClipTriTexture(u32 cullingState, fBuff_t* pFrameBuff, zBuff_t* pZBuff, f32 nearPlane,u32 vect0Idx, u32 vect1Idx, u32 vect2Idx,
    u32 clipCheck, u32 index, vect3_t* pVectSource, screen3f_t* pVectTarget,
    mat4_t* pMat, u8* pUV, B3L_tex_t* pTexture, s8 lightValue) {
    B3L_clip_t  c0, c1;
    u32 idxTemp;
    if ((clipCheck == 5) || (clipCheck == 2)) {
        idxTemp = vect0Idx;
        vect0Idx = vect1Idx;
        vect1Idx = vect2Idx;
        vect2Idx = idxTemp;
    }
    else if ((clipCheck == 3) || (clipCheck == 4)) {
        idxTemp = vect0Idx;
        vect0Idx = vect2Idx;
        vect2Idx = vect1Idx;
        vect1Idx = idxTemp;    
    }
    if ((clipCheck == 5) || (clipCheck == 3) || (clipCheck == 6)) {
        //draw 1 triangle
        ClipTexPoint(vect0Idx, vect1Idx, vect2Idx, index * 6, index * 6 + 2, index * 6 + 4, nearPlane, pVectSource, pMat, pUV, &c0, &c1);
        f32 x0 = pVectTarget[vect0Idx].x;
        f32 y0 = pVectTarget[vect0Idx].y;
        if (cullingState) {
            bool backFaceCullingResult = TriangleFaceToViewer_f(x0, y0, c0.x, c0.y, c1.x, c1.y);
            if (backFaceCullingResult) {
                return;
            }
        }
        DrawTriangleTexture(x0, y0, (f32)(pUV[index * 6]), (f32)(pUV[index * 6 + 1]), pVectTarget[vect0Idx].z,
            c0.x, c0.y, c0.u, c0.v, 0.0f, c1.x, c1.y, c1.u, c1.v, 0.0f,
            pFrameBuff, pZBuff, pTexture, lightValue);
    }
    if ((clipCheck == 1) || (clipCheck == 2) || (clipCheck == 4)) {
        f32 x1 = pVectTarget[vect1Idx].x;
        f32 y1 = pVectTarget[vect1Idx].y;
        f32 x2 = pVectTarget[vect2Idx].x;
        f32 y2 = pVectTarget[vect2Idx].y;
        ClipTexPoint(vect0Idx, vect1Idx, vect2Idx, index * 6, index * 6 + 2, index * 6 + 4, nearPlane, pVectSource, pMat, pUV, &c0, &c1);
        if (cullingState) {
            bool backFaceCullingResult = TriangleFaceToViewer_f(c0.x, c0.y, x1, y1, x2, y2);
            if (backFaceCullingResult) {
                return;
            }
        }
        DrawTriangleTexture(x1, y1, (f32)(pUV[index * 6 + 2]), (f32)(pUV[index * 6 + 3]), pVectTarget[vect1Idx].z,
            x2, y2, (f32)(pUV[index * 6 + 4]), (f32)(pUV[index * 6 + 5]), pVectTarget[vect2Idx].z,
            c0.x, c0.y, c0.u, c0.v, 0.0f, pFrameBuff, pZBuff, pTexture, lightValue);
        DrawTriangleTexture(x2, y2, (f32)(pUV[index * 6 + 4]), (f32)(pUV[index * 6 + 5]), pVectTarget[vect2Idx].z,
            c0.x, c0.y, c0.u, c0.v, 0.0f, c1.x, c1.y, c1.u, c1.v, 0.0f,
            pFrameBuff, pZBuff, pTexture, lightValue);
    }
}

static void DrawNearPlaneClipTriColor(u32 cullingState, fBuff_t* pFrameBuff, zBuff_t* pZBuff, f32 nearPlane,
                                        u32 vect0Idx, u32 vect1Idx, u32 vect2Idx, u32 clipCheck,
                                        vect3_t* pVectSource, screen3f_t* pVectTarget, mat4_t* pMat,
                                        B3L_tex_t color, s8 lightValue) {
    //ClipColorPoint(u32 v0, u32 v1, u32 v2, f32 nearPlane, vect3_t * pVect, mat4_t * pMat, B3L_clip_t * pC0, B3L_clip_t * pC1)
    B3L_clip_t  c0, c1;
    u32 idxTemp;
    if ((clipCheck == 5) || (clipCheck == 2)) {
        idxTemp = vect0Idx;
        vect0Idx = vect1Idx;
        vect1Idx = vect2Idx;
        vect2Idx = idxTemp;
    }
    else if ((clipCheck == 3) || (clipCheck == 4)) {
        idxTemp = vect0Idx;
        vect0Idx = vect2Idx;
        vect2Idx = vect1Idx;
        vect1Idx = idxTemp;
    }
    if ((clipCheck == 5) || (clipCheck == 3) || (clipCheck == 6)) {
        //draw 1 triangle
        ClipColorPoint(vect0Idx, vect1Idx, vect2Idx,  nearPlane, pVectSource, pMat, &c0, &c1);
        f32 x0 = pVectTarget[vect0Idx].x;
        f32 y0 = pVectTarget[vect0Idx].y;
        if (cullingState) {
            bool backFaceCullingResult = TriangleFaceToViewer_f(x0, y0, c0.x, c0.y, c1.x, c1.y);
            if (backFaceCullingResult) {
                return;
            }
        }
        DrawTriangleColor(x0, y0,  pVectTarget[vect0Idx].z,
            c0.x, c0.y,0.0f, c1.x, c1.y, 0.0f,
            pFrameBuff, pZBuff, color, lightValue);
    }
    if ((clipCheck == 1) || (clipCheck == 2) || (clipCheck == 4)) {
        f32 x1 = pVectTarget[vect1Idx].x;
        f32 y1 = pVectTarget[vect1Idx].y;
        f32 x2 = pVectTarget[vect2Idx].x;
        f32 y2 = pVectTarget[vect2Idx].y;
        ClipColorPoint(vect0Idx, vect1Idx, vect2Idx, nearPlane, pVectSource, pMat, &c0, &c1);
        if (cullingState) {
            bool backFaceCullingResult = TriangleFaceToViewer_f(c0.x, c0.y, x1, y1, x2, y2);
            if (backFaceCullingResult) {
                return;
            }
        }
        DrawTriangleColor(x1, y1,  pVectTarget[vect1Idx].z,
            x2, y2,  pVectTarget[vect2Idx].z,
            c0.x, c0.y,  0.0f, pFrameBuff, pZBuff, color, lightValue);
        DrawTriangleColor(x2, y2, pVectTarget[vect2Idx].z,
            c0.x, c0.y,  0.0f, c1.x, c1.y,  0.0f,
            pFrameBuff, pZBuff, color, lightValue);
    }

}
__STATIC_FORCEINLINE u32 BitmapBoundBoxTest(B3LObj_t* pObj, mat4_t* pMat) {
    //f32 scaleMax = B3L_MAX(pObj->transform.scale.x, pObj->transform.scale.y);
    u32 result;
    f32 maxX = 0.5f; f32 maxY = 0.5f; f32 maxZ = 0.5f;
    f32 minX = -0.5f; f32 minY = -0.5f; f32 minZ = -0.5f;
    result = vectBoundTest(maxX, maxY, maxZ, pMat);
    result += vectBoundTest(maxX, maxY, minZ, pMat);
    result += vectBoundTest(maxX, minY, minZ, pMat);
    result += vectBoundTest(minX, minY, minZ, pMat);
    result += vectBoundTest(minX, minY, maxZ, pMat);
    result += vectBoundTest(minX, maxY, maxZ, pMat);
    result += vectBoundTest(maxX, minY, maxZ, pMat);
    result += vectBoundTest(minX, maxY, minZ, pMat);
    return result;
}
static void RenderBitmap(B3LObj_t* pObj, render_t* pRender, mat4_t* pMat) {
    //vect3_t *pV = &(pObj->transform.translation);

    //f32 x = pV->x; f32 y = pV->y; f32 z = pV->z;
    f32 rx, ry, rz, rw;
    u32 testResult = 0;

    rz = pMat->m23;
    
    if (rz < 0.0f) {//if the near plane clip, then don't do the calculation, set bit and return directly
        return;
    }
    rx = pMat->m03;
    ry = pMat->m13;
    rw = pMat->m33;

    f32 factor = 1.0f / (rw);//prevent div zero error
    f32 screenX = (HALF_RESOLUTION_X + rx * factor * HALF_RESOLUTION_X);
    f32 screenY = (HALF_RESOLUTION_Y - ry * factor * HALF_RESOLUTION_Y);
    f32 sizeX, sizeY, sizeFactor;
    //sizeFactor = 0.5f * factor * HALF_RESOLUTION_X;
    rz = rz * factor;
    if (B3L_TEST(pRender->camera.state, OTHROGRAPHIC_PROJECT)) {
        sizeFactor = 0.5f*HALF_RESOLUTION_X * (pRender->camera.focalLength);
        //printf("x: %.3f,y: %.3f\n", screenX, screenY);      
    }
    else {
        sizeFactor = 0.5f*factor * HALF_RESOLUTION_X;    
    }
    
    sizeX = (pObj->transform.scale.x) * sizeFactor;
    sizeY = (pObj->transform.scale.y) * sizeFactor;
    f32 tu = (f32)(((u32)(pObj->pResource1)) & 0x000000FF);
    f32 tv = (f32)((((u32)(pObj->pResource1)) & 0x0000FF00) >> 8);
    f32 bu = (f32)((((u32)(pObj->pResource1)) & 0x00FF0000) >> 16);
    f32 bv = (f32)((((u32)(pObj->pResource1)) & 0xFF000000) >> 24);
    s8 lightFact = ((s8)(((pObj->state) & OBJ_SPECIAL_LIGHT_MASK) >> OBJ_SPECIAL_LIGHT_SHIFT)) - 16;
    DrawSpaceBitmap(screenX - sizeX, screenY -sizeY, screenX + sizeX, screenY +sizeY, rz,
                    tu, tv, bu, bv,pRender->pFrameBuff, pRender->pZBuff,
                    (B3L_tex_t *)(pObj->pResource0), lightFact);

}

static void UpdateParticleGenerator(B3LObj_t* pObj, render_t*  pRender, mat4_t*  O2Cmat, mat4_t*  O2WMat,u32 timePassed) {
    //update active particles
    //draw generate box?
    particleGenerator_t* pGen = ((particleGenerator_t*)(pObj->pResource1));
    if (B3L_TEST(pObj->state, OBJ_PARTICLE_GEN_ACTIVE)) { //if the generator is active, add new particles
        pGen->pParticleGenFunc(pRender, pObj, O2WMat, timePassed);
    }
    pGen->pParticleUpdateFunc(pRender, pObj, O2WMat, timePassed);
}

/*-------------------------------------------------------------------------------------------------
Public Function
-------------------------------------------------------------------------------------------------*/
void RenderObjs(render_t* pRender,u32 time) {
    static u32 oldTime = 0;
    u32 timePassed = time - oldTime;
    oldTime = time;
    mat4_t O2Cmat; 
    //mat3_t objMat;
    mat4_t O2WMat;
    u32 state;
    u32 renderLevel;
    f32 distance;
    //get the list enter point obj
    B3LObj_t* pCurrentObj = pRender->scene.pActiveObjs;
    f32 lvl0Distance = pRender->lvl0Distance;
    f32 lvl1Distance = pRender->lvl1Distance;

    while (pCurrentObj != (B3LObj_t*)NULL) {
        state = pCurrentObj->state;     
        if (B3L_TEST(state, OBJ_VISUALIZABLE) == 0) {  //obj visual is false
            pCurrentObj = pCurrentObj->next;
            continue;
        }

        //also process mother obj rotation chain
         B3L_Create_O2W_O2C_Matrix(pCurrentObj, &(pRender->camera.camW2CMat),&(O2WMat),&O2Cmat);
        //test boundBoxTestFactor to check if the obj out of clip range
        //Boundbox testing, to check potential near plane clip is necessary or not
        u32 result=0;
        if (((state & OBJ_TYPE_MASK)==(1<<MESH_OBJ))||((state & OBJ_TYPE_MASK) == (1 << NOTEX_MESH_OBJ))||((state & OBJ_TYPE_MASK) == (1 << POLYGON_OBJ))){
            //obj with boundbox
            f32 *pBoundBox = GetBoundBox((B3L_Mesh_t *)(pCurrentObj->pResource0));
            result = BoundBoxTest(pBoundBox, &O2Cmat);
        }else if ((state & OBJ_TYPE_MASK) == (1 << BITMAP_OBJ)) {
            result = BitmapBoundBoxTest(pCurrentObj,&O2Cmat);
        }
        else {//for particle generation obj, always update it
            result = 1 << BOX_IN_SPACE;
        }
        
        if ((result >> BOX_IN_SPACE) == 0) { //both of 8 points are not in the clip range 
            pCurrentObj = pCurrentObj->next;
            continue;
        }
        //calculate the distance
        f32 x = O2Cmat.m03;
        f32 y = O2Cmat.m13;
        f32 z = O2Cmat.m23;
        distance = B3L_Sqrtf(x * x + y * y + z * z);
        //decide render level info        
        if (B3L_TEST(state, OBJ_IGNORE_RENDER_LEVEL)) {
            renderLevel = (state & OBJ_RENDER_LEVEL_MASK) >> OBJ_FIX_RENDER_LEVEL_SHIFT;
        }
        else {
            if (distance < lvl0Distance) {
                renderLevel = 0;
            }
            else if (distance < lvl1Distance) {
                renderLevel = 1;
            }
            else {
                renderLevel = 2;
            }
        }
        switch (state & OBJ_TYPE_MASK) {
        case (1 << MESH_OBJ):
            RenderTexMesh(pCurrentObj, pRender, &O2Cmat, &O2WMat,renderLevel,result);
            break;
        case (1 << POLYGON_OBJ):
            RenderPolygon(pCurrentObj, pRender, &O2Cmat, result);
            break;
        case (1 << NOTEX_MESH_OBJ):
            RenderColorMesh(pCurrentObj, pRender, &O2Cmat, &O2WMat, renderLevel, result);
            break;
        case (1<< BITMAP_OBJ):
            RenderBitmap(pCurrentObj, pRender, &O2Cmat);
            break;
        case (1<<PARTICLE_GEN_OBJ):
            UpdateParticleGenerator(pCurrentObj, pRender, &O2Cmat, &O2WMat, timePassed);
            break;
        }
        //point to the next obj
        pCurrentObj = pCurrentObj->next;
    }

}



u32 B3L_GetMeshResouceSize(B3L_Mesh_t* pMesh) {
    u32 triNum = GetTriNum(pMesh);
    u32 vectNum = GetVectNum(pMesh);
    u32 uvNum = GetUVNum(pMesh);
    u32 norNum = GetNormalNum(pMesh);
    u32 returnVal = 0;
    if (uvNum != 0) {
        returnVal = vectNum * 12 + MESH_HEAD_SHIFT + 24 + triNum * 12;
    }
    else {
        if ((triNum & 0x01) == 0x01) {
            returnVal = vectNum * 12 + MESH_HEAD_SHIFT + 24 + triNum * 6+2;
        }
        else {
            returnVal = vectNum * 12 + MESH_HEAD_SHIFT + 24 + triNum * 6;
        }

    }
    if (norNum != 0) {
        returnVal += norNum * 12;
    }

    return returnVal;
}

u32 B3L_GetTexResouceSize(B3L_tex_t* pTexture) {
    u32 size = ((u16 *)pTexture)[2];
    return (size * size) + TEXTURE_HEAD_SHIFT;
}

u32 B3L_GetPolyResouceSize(B3L_Polygon_t* pPoly) {
    u32 linNum = GetLinNum(pPoly);
    u32 vectNum = GetVectNum((B3L_Mesh_t*)pPoly);


    u32 returnVal = vectNum * 12 + MESH_HEAD_SHIFT + 24 + linNum * 4;


    return returnVal;
}


