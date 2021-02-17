#ifndef __B3D_MATH_H__
#define __B3D_MATH_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "b3d_types.h"

    extern u32 B3L_seed;

__STATIC_FORCEINLINE void Vect3Xmat4(vect3_t *pV, mat4_t *pMat, vect4_t *pResult);
__STATIC_FORCEINLINE void  Vect3Xmat4ToScreen4(vect3_t* pV, mat4_t* pMat, vect4_t* pResult);
__STATIC_FORCEINLINE void Vect3Xmat4WithTestToScreen4(vect3_t *pV, mat4_t *pMat, screen4_t *pResult);
__STATIC_FORCEINLINE void Vect3Xmat4WithTest_f(vect3_t *pV, mat4_t *pMat, screen3f_t *pResult);
__STATIC_FORCEINLINE bool Vect4BoundTest(vect4_t *pV);

/*-----------------------------------------------------------------------------
Triangle functions
-----------------------------------------------------------------------------*/
extern f32  B3L_sin(f32 in);
extern f32  B3L_cos(f32 in);
extern f32  B3L_asin(f32 in);
extern f32  B3L_atan2(f32 y,f32 x);
/*-----------------------------------------------------------------------------
Random functions
-----------------------------------------------------------------------------*/
extern  void B3L_SetSeed(u32 seed);
extern  u32  B3L_Random(void); 
extern  u32  B3L_Rnd(u32 range);
/*-----------------------------------------------------------------------------
Vector functions
-----------------------------------------------------------------------------*/
extern vect2_t B3L_Vect2(f32 x,f32 y);
extern vect3_t B3L_Vect3(f32 x,f32 y,f32 z);
extern vect4_t B3L_Vect4(f32 x,f32 y,f32 z,f32 w);
#define B3L_VECT2_SET(v,vx,vy)         v.x=vx;v.y=vy
#define B3L_VECT3_SET(v,vx,vy,vz)      v.x=vx;v.y=vy;v.z=vz
#define B3L_VECT4_SET(v,vx,vy,vz,vw)   v.x=vx;v.y=vy;v.z=vz;v.w=vw
extern f32  B3L_Vec2Length(vect2_t *pV);
extern void B3L_Vect2Normalize(vect2_t *pV, vect2_t *pResult);
extern f32  B3L_Vect3Length(vect3_t *pV);
extern void B3L_Vect3Normalize(vect3_t *pV,vect3_t *pResult);
extern void B3L_Vect3Add(vect3_t *pVa,vect3_t *pVb,vect3_t *pResult);
extern void B3L_Vect3Sub(vect3_t *pVa,vect3_t *pVb,vect3_t *pResult);
extern f32  B3L_Vect3Dot(vect3_t *pA, vect3_t *pB);
extern void B3L_Vect3Cross(vect3_t *pA, vect3_t *pB, vect3_t *pResult);
extern void B3L_Vect3Interp(vect3_t *pVa,vect3_t *pVb,vect3_t *pResult,f32 t);
extern void B3L_Vect3Scale(vect3_t *pV,f32 scale,vect3_t *pResult);
/*-----------------------------------------------------------------------------
Matrix functions
-----------------------------------------------------------------------------*/
extern void B3L_InitUnitMat3(mat3_t *pMat);
extern void B3L_Mat3XRotate(mat3_t *pMat,f32 angle);
extern void B3L_Mat3YRotate(mat3_t *pMat,f32 angle);
extern void B3L_Mat3ZRotate(mat3_t *pMat,f32 angle);
extern void B3L_CreateO2WMat(mat3_t *pRMat, vect3_t *pTranslation, vect3_t *pScale, mat4_t *pResult);
extern void B3L_InitMat4One(mat4_t *pMat);
extern void B3L_TransposeMat4(mat4_t *pMat);
extern void B3L_Mat4XMat4(mat4_t *pMat1,mat4_t *pMat2, mat4_t *pMat3);
extern void B3L_Mat3MultMat3ABB(mat3_t *pMatA,mat3_t *pMatB);
extern void B3L_Mat3MultMat3ABA(mat3_t *pMatA,mat3_t *pMatB);
extern void B3L_MakeScaleMatrix(f32 scaleX,f32 scaleY,f32 scaleZ,mat4_t *pMat);
extern void B3L_MakeTranslationMat(f32 offsetX,f32 offsetY,f32 offsetZ,mat4_t *pMat);
extern void B3L_MakeO2CMatrix(mat3_t *pRMat,vect3_t *pScale,vect3_t *pTrans,mat4_t *pCamMat, mat4_t *pResult);
extern void B3L_Vect3MulMat3(vect3_t *pV, mat3_t *pMat, vect3_t *pResult);
extern void B3L_Point3MulMat4(vect3_t *pV, mat4_t *pMat, vect3_t *pResult);
extern void MakeClipMatrix(u32 state,f32 near_plane,f32 far_plane,f32 focalLength, f32 aspectRatio,mat4_t *pMat);
/*-----------------------------------------------------------------------------
Rotation convert functions
-----------------------------------------------------------------------------*/
extern void B3L_EulerToMatrix(euler3_t *pEuler,mat3_t *pMat);
extern void B3L_MatrixToEuler(mat3_t *pMat, euler3_t *pEuler);
extern void B3L_QuaternionToMatrix(quat4_t *pQuat, mat3_t *pMat);
extern void B3L_MatrixToQuaternion(mat3_t *pMat, quat4_t *pQuat);
extern void B3L_EulerToQuaternion(euler3_t *pEuler,quat4_t *pQuat);
extern void B3L_QuaternionToEuler(quat4_t *pQuat,euler3_t *pEuler);
/*-----------------------------------------------------------------------------
Quaternion functions
-----------------------------------------------------------------------------*/
#define SET_IDENTITY_P_QUAT(a)       (a)->x=0.0f;(a)->y=0.0f;(a)->z=0.0f;(a)->w=1.0f
extern void B3L_QuatMult(quat4_t *pL,quat4_t *pR, quat4_t *pResult);
extern f32  B3L_QuatDot(quat4_t *pL,quat4_t *pR);
extern void B3L_CreateQuaternionByAxisAngle(vect3_t *pAxis, f32 angle, quat4_t *pResult);
extern void B3L_FromToRotation(vect3_t *pFrom, vect3_t *pTo,quat4_t *pResult);
extern void B3L_LookRotation(vect3_t *pA, vect3_t *pB, vect3_t *pUp, quat4_t *pResult);
extern void B3L_CreateLookAtQuaternion(vect3_t *pFrom, vect3_t *pAt, vect3_t *pUp, quat4_t *pResult);
extern void B3L_QuatCreateXRotate(quat4_t *pQ,f32 angle);
extern void B3L_QuatCreateYRotate(quat4_t *pQ,f32 angle);
extern void B3L_QuatCreateZRotate(quat4_t *pQ,f32 angle);
extern void B3L_QuaternionInterp(quat4_t *pFrom,quat4_t *pTo,quat4_t *pResult, f32 t);



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



__STATIC_FORCEINLINE void  Vect3Xmat4WithTestToScreen4(vect3_t* pV, mat4_t* pMat, screen4_t* pResult) {
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

    if (rz < 0.0f) {//if the near plane clip, then don't do the calculation, set bit and return directly
        B3L_SET(testResult, B3L_NEAR_PLANE_CLIP);
        pResult->test = testResult;
        return;
    }
    else {
        if ((rx <= rw) && (rx >= -rw) && (ry <= rw) && (ry >= -rw) && (rz <= rw)) {
            B3L_SET(testResult, B3L_IN_SPACE);

        }
    }

    f32 factor = 1.0f / (rw);//rw won't be zero due we returned already rz<0 (rz>0, rw must >0)
    s32 intX = (int32_t)(HALF_RESOLUTION_X + rx * factor * HALF_RESOLUTION_X);
    s32 intY = (int32_t)(HALF_RESOLUTION_Y - ry * factor * HALF_RESOLUTION_Y);
    rz = rz * factor;
    pResult->test = testResult;
    pResult->x = intX;
    pResult->y = intY;
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
#if B3L_DO_NEAR_PLANE_CLIP == 1
        //to calculate a correct backface culling, not always correct!
       // pResult->x = HALF_RESOLUTION_X + HALF_RESOLUTION_X * rx;
       // pResult->y = HALF_RESOLUTION_Y - HALF_RESOLUTION_Y * ry;
#endif
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

#ifdef __cplusplus
}
#endif

#endif