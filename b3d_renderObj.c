#include "b3d_common.h"
#include "b3d_types.h"
#include "b3d_math.h"
#include "b3d_config.h"
#include "b3d.h"
#include <stdio.h>



/*-------------------------------------------------------------------------------------------------
Private Function declaration
-------------------------------------------------------------------------------------------------*/
__STATIC_FORCEINLINE u32 BoundBoxTest(f32* Boundbox, mat4_t* pMat);
__STATIC_FORCEINLINE bool TriangleFaceToViewer_f(f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2);

static void RenderTexMesh(B3LObj_t* pObj, render_t* pRender, mat4_t* pMat, mat3_t* pRmat, u32 renderLevel, u32 bboxResult);
static s8  CalLightFactor(f32 normalDotLight, f32  lightFactor0, f32  lightFactor1);
static void ClipPoint(u32 v0, u32 v1, u32 v2, u32 i0, u32 i1, u32 i2, f32 nearPlane, vect3_t* pVect, mat4_t* pMat, u8* pUV, B3L_clip_t* pC0, B3L_clip_t* pC1);

static vect3_t * GetNormal(B3L_Mesh_t *start,u16 vectNum, u16 triNum);
static vect3_t * GetVect(B3L_Mesh_t *start,u16 vectNum, u16 triNum);
static u8 * GetUV(B3L_Mesh_t *start,u16 vectNum, u16 triNum);
static u16 * GetTriIdx(B3L_Mesh_t *start,u16 vectNum, u16 triNum);
static f32 * GetBoundBox(B3L_Mesh_t *start);
/*-------------------------------------------------------------------------------------------------
Private Functions 
-------------------------------------------------------------------------------------------------*/
static void DrawNearPlaneClipTriTexture(u32 cullingState,render_t* pRender,u32 vect0Idx,u32 vect1Idx,u32 vect2Idx,
                                        u32 clipCheck,u32 index,vect3_t* pVectSource,screen3f_t* pVectTarget,
                                        mat4_t* pMat,u8* pUV,texture_t* pTexture,s8 lightValue);

static vect3_t * GetNormal(B3L_Mesh_t *start,u16 vectNum, u16 triNum){
    u8 * u8Start = (u8 *)start;
    //u16 vectNum = ((u16*)(start))[0];
    //u16 triNum = ((u16*)(start))[1];
    vect3_t* pNormal = (vect3_t *)(u8Start +28 +vectNum*12+ triNum*12); 
    return pNormal;
}
static vect3_t * GetVect(B3L_Mesh_t *start,u16 vectNum, u16 triNum){
    u8 * u8Start = (u8 *)start;
    vect3_t* pVect = (vect3_t *)(u8Start +28); 
    return pVect;
}
static u8 * GetUV(B3L_Mesh_t *start,u16 vectNum, u16 triNum){
    u8 * u8Start = (u8 *)start;
    //u16 vectNum = ((u16*)(start))[0];
    //u16 triNum = ((u16*)(start))[1];
    u8* pUV = u8Start + 28 + vectNum * 12 + triNum * 6; 
    return pUV;
}
static u16 * GetTriIdx(B3L_Mesh_t *start,u16 vectNum, u16 triNum){
    u8 * u8Start = (u8 *)start;
    //u16 vectNum = ((u16*)(start))[0];
    //u16 triNum = ((u16*)(start))[1];
    u16* pTri = (u16 *)(u8Start + 28 + vectNum*12); 
    return pTri;
}

static f32 * GetBoundBox(B3L_Mesh_t *start){
    u8 * u8Start = (u8 *)start;
    //u16 vectNum = ((u16*)(start))[0];
    //u16 triNum = ((u16*)(start))[1];
    f32* pBound = (f32 *)(u8Start + 4); 
    return pBound;
}

static void ClipPoint(u32 v0, u32 v1, u32 v2, u32 i0, u32 i1, u32 i2, f32 nearPlane, vect3_t* pVect, mat4_t* pMat, u8* pUV, B3L_clip_t* pC0, B3L_clip_t* pC1) {
    vect4_t vect0, vect1, vect2;
    Vect3Xmat4(pVect + v0, pMat, &vect0);
    Vect3Xmat4(pVect + v1, pMat, &vect1);
    Vect3Xmat4(pVect + v2, pMat, &vect2);
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
/*
__STATIC_FORCEINLINE void Vect3Xmat4(vect3_t* pV, mat4_t* pMat, vect4_t* pResult) {
    f32 x = pV->x; f32 y = pV->y; f32 z = pV->z;
#define dotCol(col)\
        ((x*(pMat->m##col##0)) +\
        (y*(pMat->m##col##1)) +\
        (z*(pMat->m##col##2)) +\
        (pMat->m##col##3))

    pResult->x = dotCol(0);
    pResult->y = dotCol(1);
    pResult->z = dotCol(2);
    pResult->w = dotCol(3);
#undef dotCol
}
*/

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

static s8  CalLightFactor(f32 normalDotLight, f32  lightFactor0, f32  lightFactor1) {
    normalDotLight += lightFactor0;
    normalDotLight *= lightFactor1;
    return ((s8)B3L_RoundingToS(normalDotLight));

}

static void RenderTexMesh(B3LObj_t* pObj, render_t* pRender, mat4_t* pMat, mat3_t* pRmat, u32 renderLevel, u32 bboxResult) {
    int32_t i;
    s8 lightValue = 0;
    B3L_Mesh_t* pMesh = (B3L_Mesh_t*)(pObj->pResource0);
    texture_t* pTexture = (texture_t*)(pObj->pResource1);
    u16 vectNum = ((u16*)(pMesh))[0];
    u16 triNum = ((u16*)(pMesh))[1];
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
            Vect3Xmat4ToScreen4(pVectSource + i, pMat, pVectTarget + i);
        }
    }
    else {
        for (i = vectNum - 1; i >= 0; i--) {
            //Vect3Xmat4ToScreen4(pVectSource + i, pMat, pVectTarget + i);
            Vect3Xmat4WithTest_f(pVectSource + i, pMat, (screen3f_t*)pVectTarget + i);
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
    //u16 normalNum = ((u16*)(pMesh))[2];
    vect3_t* pNormal = GetNormal(pMesh, vectNum, triNum);
    //u16* pNormalIdx = GetNormalIdx(pMesh,vectNum,triNum,normalNum);
    f32 lightX, lightY, lightZ;
    f32 normalFact, normalDotLight;
    f32 lightFactor0, lightFactor1;
    vect3_t normalVect;
    if (renderLevel == 0) {
        if (B3L_TEST(pRender->light.state, LIGHT_TYPE_BIT) == POINT_LIGHT) {
            //dot light, calculate the vect point  to light from obj (both already in camera space)
            vect3_t* translation = &(pObj->transform.translation);
            lightX = pRender->light.lightVect.x - translation->x;
            lightY = pRender->light.lightVect.y - translation->y;
            lightZ = pRender->light.lightVect.z - translation->z;
            normalFact = FastInvertSqrt(lightX * lightX + lightY * lightY + lightZ * lightZ);
            lightX = lightX * normalFact;
            lightY = lightY * normalFact;
            lightZ = lightZ * normalFact;
        }
        else {
            //parallel light, the point to light vect is already in camera space
            lightX = pRender->light.lightVect.x;
            lightY = pRender->light.lightVect.y;
            lightZ = pRender->light.lightVect.z;
        }
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
                B3L_Vect3MulMat3(pNormal + i, pRmat, &normalVect);
                //dot multi light and normalvect to get the light factor
                normalDotLight = normalVect.x * lightX + normalVect.y * lightY + normalVect.z * lightZ;
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
                    // lightValue =normalLightBuff[pNormalIdx[i]];
                    B3L_Vect3MulMat3(pNormal + i, pRmat, &normalVect);
                    //dot multi light and normalvect to get the light factor
                    normalDotLight = normalVect.x * lightX + normalVect.y * lightY + normalVect.z * lightZ;
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
                    DrawNearPlaneClipTriTexture(cullingState, pRender, vect0Idx, vect1Idx, vect2Idx,
                        clipCheck, i, pVectSource, pVectTestTarget, pMat, pUV, pTexture, lightValue);
                }
#endif                
            }
        }
    }
}

static void DrawNearPlaneClipTriTexture(u32 cullingState, render_t* pRender, u32 vect0Idx, u32 vect1Idx, u32 vect2Idx,
    u32 clipCheck, u32 index, vect3_t* pVectSource, screen3f_t* pVectTarget,
    mat4_t* pMat, u8* pUV, texture_t* pTexture, s8 lightValue) {
    fBuff_t* pFrameBuff = pRender->pFrameBuff;
    zBuff_t* pZBuff = pRender->pZBuff;
    B3L_clip_t  c0, c1;
    f32 nearPlane = pRender->nearPlane;
   
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
    f32 x0 = pVectTarget[vect0Idx].x;
    f32 y0 = pVectTarget[vect0Idx].y;
    f32 x1 = pVectTarget[vect1Idx].x;
    f32 y1 = pVectTarget[vect1Idx].y;
    f32 x2 = pVectTarget[vect2Idx].x;
    f32 y2 = pVectTarget[vect2Idx].y;
    if ((clipCheck == 5) || (clipCheck == 3) || (clipCheck == 6)) {
        //draw 1 triangle
        ClipPoint(vect0Idx, vect1Idx, vect2Idx, index * 6, index * 6 + 2, index * 6 + 4, nearPlane, pVectSource, pMat, pUV, &c0, &c1);
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
        ClipPoint(vect0Idx, vect1Idx, vect2Idx, index * 6, index * 6 + 2, index * 6 + 4, nearPlane, pVectSource, pMat, pUV, &c0, &c1);
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


static void DrawNearPlaneClipTriTexture2(u32 cullingState, render_t* pRender, u32 vect0Idx, u32 vect1Idx, u32 vect2Idx,
                                        u32 clipCheck, u32 index, vect3_t* pVectSource, screen3f_t* pVectTarget,
                                        mat4_t* pMat, u8* pUV, texture_t* pTexture, s8 lightValue) {
    fBuff_t* pFrameBuff = pRender->pFrameBuff;
    zBuff_t* pZBuff = pRender->pZBuff;
    B3L_clip_t  c0, c1;
    f32 nearPlane = pRender->nearPlane;
    f32 x0 = pVectTarget[vect0Idx].x;
    f32 y0 = pVectTarget[vect0Idx].y;
    f32 x1 = pVectTarget[vect1Idx].x;
    f32 y1 = pVectTarget[vect1Idx].y;
    f32 x2 = pVectTarget[vect2Idx].x;
    f32 y2 = pVectTarget[vect2Idx].y;
    switch (clipCheck) {
    case 1:  //vect 0 outside
        ClipPoint(vect0Idx, vect1Idx, vect2Idx, index * 6, index * 6 + 2, index * 6 + 4, nearPlane, pVectSource, pMat, pUV, &c0, &c1);
        if (cullingState) {
            bool backFaceCullingResult = TriangleFaceToViewer_f(c0.x, c0.y,x1, y1, x2, y2);
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
        break;
    case 6: //vect 0 inside
        ClipPoint(vect0Idx, vect1Idx, vect2Idx, index * 6, index * 6 + 2, index * 6 + 4, nearPlane, pVectSource, pMat, pUV, &c0, &c1);
        if (cullingState) {
            bool backFaceCullingResult = TriangleFaceToViewer_f(x0, y0, c0.x, c0.y, c1.x, c1.y);
            if (backFaceCullingResult) {
                return;
            }
        }
        DrawTriangleTexture(x0, y0, (f32)(pUV[index * 6]), (f32)(pUV[index * 6 + 1]), pVectTarget[vect0Idx].z,
            c0.x, c0.y, c0.u, c0.v, 0.0f, c1.x, c1.y, c1.u, c1.v, 0.0f,
            pFrameBuff, pZBuff, pTexture, lightValue);
        break;
    case 5: //vect 1 inside
        ClipPoint(vect1Idx, vect0Idx, vect2Idx, index * 6 + 2, index * 6, index * 6 + 4, nearPlane, pVectSource, pMat, pUV, &c0, &c1);
        if (cullingState) {
            bool backFaceCullingResult = TriangleFaceToViewer_f(c0.x, c0.y,x1, y1,  c1.x, c1.y);
            if (backFaceCullingResult) {
                return;
            }
        }
        DrawTriangleTexture(x1, y1, (f32)(pUV[index * 6 + 2]), (f32)(pUV[index * 6 + 3]), pVectTarget[vect1Idx].z,
            c0.x, c0.y, c0.u, c0.v, 0.0f, c1.x, c1.y, c1.u, c1.v, 0.0f,
            pFrameBuff, pZBuff, pTexture, lightValue);
        break;
    case 2:  //vect 1 outside
        ClipPoint(vect1Idx, vect0Idx, vect2Idx, index * 6 + 2, index * 6, index * 6 + 4, nearPlane, pVectSource, pMat, pUV, &c0, &c1);
        if (cullingState) {
            bool backFaceCullingResult = TriangleFaceToViewer_f( c1.x, c1.y,x2, y2, x0, y0);
            if (backFaceCullingResult) {
                return;
            }
        }
        DrawTriangleTexture(x0, y0, (f32)(pUV[index * 6]), (f32)(pUV[index * 6 + 1]), pVectTarget[vect0Idx].z,
            x2, y2, (f32)(pUV[index * 6 + 4]), (f32)(pUV[index * 6 + 5]), pVectTarget[vect2Idx].z,
            c0.x, c0.y, c0.u, c0.v, 0.0f,
            pFrameBuff, pZBuff, pTexture, lightValue);
        DrawTriangleTexture(x2, y2, (f32)(pUV[index * 6 + 4]), (f32)(pUV[index * 6 + 5]), pVectTarget[vect2Idx].z,
            c0.x, c0.y, c0.u, c0.v, 0.0f, c1.x, c1.y, c1.u, c1.v, 0.0f,
            pFrameBuff, pZBuff, pTexture, lightValue);
        break;
    case 3://vect 2 inside
        ClipPoint(vect2Idx, vect0Idx, vect1Idx, index * 6 + 4, index * 6, index * 6 + 2, nearPlane, pVectSource, pMat, pUV, &c0, &c1);
        if (cullingState) {
            bool backFaceCullingResult = TriangleFaceToViewer_f(c1.x, c1.y,x2, y2, c0.x, c0.y );
            if (backFaceCullingResult) {
                return;
            }
        }
        DrawTriangleTexture(x2, y2, (f32)(pUV[index * 6 + 4]), (f32)(pUV[index * 6 + 5]), pVectTarget[vect2Idx].z,
            c0.x, c0.y, c0.u, c0.v, 0.0f, c1.x, c1.y, c1.u, c1.v, 0.0f,
            pFrameBuff, pZBuff, pTexture, lightValue);
        break;
    case 4://vect 2 outside
        ClipPoint(vect2Idx, vect0Idx, vect1Idx, index * 6 + 4, index * 6, index * 6 + 2, nearPlane, pVectSource, pMat, pUV, &c0, &c1);
        if (cullingState) {
            bool backFaceCullingResult = TriangleFaceToViewer_f(x0, y0, x1, y1, c1.x, c1.y);
            if (backFaceCullingResult) {
                return;
            }
        }
        DrawTriangleTexture(x0, y0, (f32)(pUV[index * 6]), (f32)(pUV[index * 6 + 1]), pVectTarget[vect0Idx].z,
            x1, y1, (f32)(pUV[index * 6 + 2]), (f32)(pUV[index * 6 + 3]), pVectTarget[vect1Idx].z,
            c0.x, c0.y, c0.u, c0.v, 0.0f,
            pFrameBuff, pZBuff, pTexture, lightValue);
        DrawTriangleTexture(x1, y1, (f32)(pUV[index * 6 + 2]), (f32)(pUV[index * 6 + 3]), pVectTarget[vect1Idx].z,
            c0.x, c0.y, c0.u, c0.v, 0.0f, c1.x, c1.y, c1.u, c1.v, 0.0f,
            pFrameBuff, pZBuff, pTexture, lightValue);
        break;
    }//end of switch   
}
/*-------------------------------------------------------------------------------------------------
Public Function
-------------------------------------------------------------------------------------------------*/
void RenderObjs(render_t* pRender) {
    mat4_t mat; 
    mat3_t objMat;
    u32 state;
    u32 renderLevel;
    f32 distance;
    //get the list enter point obj
    B3LObj_t* pCurrentObj = pRender->scene.pActiveMeshObjs;
    f32 lvl0Distance = pRender->lvl0Distance;
    f32 lvl1Distance = pRender->lvl1Distance;

    while (pCurrentObj != (B3LObj_t*)NULL) {
        state = pCurrentObj->state;     
        if (B3L_TEST(state, OBJ_VISUALIZABLE) == 0) {  //obj visual is false
            pCurrentObj = pCurrentObj->next;
            continue;
        }
        B3L_QuaternionToMatrix(&(pCurrentObj->transform.quaternion), &(objMat));
        B3L_MakeO2CMatrix(&(objMat), &(pCurrentObj->transform.scale),
            &(pCurrentObj->transform.translation), &(pRender->camera.camW2CMat), &mat);
        //test boundBoxTestFactor to check if the obj out of clip range
        //Boundbox testing, to check potential near plane clip is necessary or not
        u32 result=0;
        if ((state & OBJ_TYPE_MASK)!=(1<< BITMAP_OBJ)){
            f32 *pBoundBox = GetBoundBox((B3L_Mesh_t *)(pCurrentObj->pResource0));
            result = BoundBoxTest(pBoundBox, &mat);
        }else{

            //TODO: bitmap boundbox testing method
        }
        
        if ((result >> BOX_IN_SPACE) == 0) { //both of 8 points are not in the clip range 
            pCurrentObj = pCurrentObj->next;
            continue;
        }
        //calculate the distance
        f32 x = mat.m03;
        f32 y = mat.m13;
        f32 z = mat.m23;
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
            RenderTexMesh(pCurrentObj, pRender, &mat, &objMat,renderLevel,result);
            break;
        case (1 << POLYGON_OBJ):
            //RenderPolygon(pCurrentObj, pRender, &mat);
            break;
        case (1 << NOTEX_MESH_OBJ):
            //RenderNoTexMesh(pCurrentObj, pRender, &mat, renderLevel);
            break;
        case (1<< BITMAP_OBJ):
            //RenderBitmap(pCurrentObj, pRender, &mat, renderLevel);
            break;
        }
        //point to the next obj
        pCurrentObj = pCurrentObj->next;
    }

}








