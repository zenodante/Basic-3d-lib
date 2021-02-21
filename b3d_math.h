#ifndef __B3D_MATH_H__
#define __B3D_MATH_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "b3d_types.h"

    extern u32 B3L_seed;




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
extern void B3L_InitUnitMat4(mat4_t *pMat);
extern void B3L_TransposeMat4(mat4_t *pMat);
extern void B3L_Mat3MultMat3ABB(mat3_t *pMatA,mat3_t *pMatB);
extern void B3L_Mat3MultMat3ABA(mat3_t *pMatA,mat3_t *pMatB);
extern void B3L_Mat4XMat4(mat4_t *pMat1,mat4_t *pMat2, mat4_t *pMat3);
extern void B3L_Mat3XRotate(mat3_t *pMat,f32 angle);
extern void B3L_Mat3YRotate(mat3_t *pMat,f32 angle);
extern void B3L_Mat3ZRotate(mat3_t *pMat,f32 angle);
extern void B3L_CreateScaleMatrix(f32 scaleX,f32 scaleY,f32 scaleZ,mat4_t *pMat);
extern void B3L_CreateTranslationMat(f32 offsetX,f32 offsetY,f32 offsetZ,mat4_t *pMat);
extern void B3L_CreateO2WMat(mat3_t *pRMat, vect3_t *pTranslation, vect3_t *pScale, mat4_t *pResult);
extern void  B3L_Create_O2W_O2C_Matrix(B3LObj_t* pObj, mat4_t* pCamMat, mat4_t* pO2WMat, mat4_t* pO2CMat);
extern void MakeClipMatrix(u32 state,f32 near_plane,f32 far_plane,f32 focalLength, f32 aspectRatio,mat4_t *pMat);
extern void B3L_Vect3XMat3(vect3_t *pV, mat3_t *pMat, vect3_t *pResult);
extern void B3L_Vect3Xmat3inMat4Format(vect3_t* pV, mat4_t* pMat, vect4_t* pResult);
extern void B3L_Vect3Xmat4(vect3_t* pV, mat4_t* pMat, vect4_t* pResult);
extern void B3L_InvertMat3(mat3_t* pMatS, mat3_t* pMatI);
extern void B3L_InvertMat4(mat4_t* pMatS, mat4_t* pMatI);
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




#ifdef __cplusplus
}
#endif

#endif