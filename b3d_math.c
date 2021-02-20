#include "b3d_common.h"
#include "b3d_types.h"
#include "b3d_math.h"

#pragma GCC optimize("-O3")

u32 B3L_seed = 0x31415926;

static void B3L_CreateO2WChainMatrix(B3LObj_t* pObj, mat3_t* pRMat, mat4_t* pResult);
/*-----------------------------------------------------------------------------
Triangle functions
-----------------------------------------------------------------------------*/


f32 B3L_sin(f32 in) {
    in = in - (f32)(B3L_RoundingToS(in));//scale to -1~1
    if (in > 0.25f) { in = 0.5f - in; }
    if (in < -0.25f) { in = -0.5f - in; }
    if (in > 0.25f) { in = 0.5f - in; }
    f32 v2 = in * in;
    f32 v3 = in * v2;
    f32 v5 = v3 * v2;
    f32 v7 = v5 * v2;
    return  6.28316394434f * in - 41.3371315018f * v3 + 81.3404239211f * v5 - 70.99090501333f * v7;
}

f32 B3L_cos(f32 in) {
    return B3L_sin(in + 0.25f);
}

f32 B3L_asin(f32 in) {
    in = Clamp_f(in, -1.0f, 1.0f);
    float negate = (f32)(in < 0);

    if (in < 0.0f) {
        in = -1.0f * in;
    }
    float ret = -0.0187293f;
    ret *= in;
    ret += 0.0742610f;
    ret *= in;
    ret -= 0.2121144f;
    ret *= in;
    ret += 1.5707288f;
    ret = 3.14159265358979f * 0.5f - B3L_Sqrtf(1.0f - in) * ret;
    ret = (ret - 2 * negate * ret) * 0.15915494309f;
    return ret;
}

f32  B3L_atan2(f32 y, f32 x) {
    f32 t0, t1, t3, t4;

    t3 = B3L_Absf(x);
    t1 = B3L_Absf(y);
    t0 = B3L_MAX(t3, t1);
    t1 = B3L_MIN(t3, t1);
    t3 = 1.0f / t0;
    t3 = t1 * t3;
    t4 = t3 * t3;
    t0 = -0.013480470f;
    t0 = t0 * t4 + 0.057477314f;
    t0 = t0 * t4 - 0.121239071f;
    t0 = t0 * t4 + 0.195635925f;
    t0 = t0 * t4 - 0.332994597f;
    t0 = t0 * t4 + 0.999995630f;
    t3 = t0 * t3 * 0.159154943f;
    t3 = (B3L_Absf(y) > B3L_Absf(x)) ? 0.25f - t3 : t3;
    t3 = (x < 0) ? 0.5f - t3 : t3;
    t3 = (y < 0) ? -t3 : t3;
    return t3;
}

/*-----------------------------------------------------------------------------
Random functions
-----------------------------------------------------------------------------*/

void B3L_SetSeed(u32 seed) {
    B3L_seed = seed | 1;
}
//using rand xorshift algorithm from george marsaglia's paper
u32 B3L_Random(void) {
    B3L_seed ^= (B3L_seed << 13);
    B3L_seed ^= (B3L_seed >> 17);
    B3L_seed ^= (B3L_seed << 5);
    return B3L_seed;
}

u32  B3L_Rnd(u32 range) {
    return B3L_Random() % range;
}

/*-----------------------------------------------------------------------------
Vector functions
-----------------------------------------------------------------------------*/

vect2_t B3L_Vect2(f32 x, f32 y) {
    vect2_t output = { .x = x,.y = y };
    return output;
}

vect3_t B3L_Vect3(f32 x, f32 y, f32 z) {
    vect3_t output = { .x = x,.y = y,.z = z };
    return output;
}

vect4_t B3L_Vect4(f32 x, f32 y, f32 z, f32 w) {
    vect4_t output = { .x = x,.y = y,.z = z,.w = w };
    return output;
}

f32 B3L_Vec2Length(vect2_t* pV) {
    return B3L_Sqrtf(pV->x * pV->x + pV->y * pV->y);
}

void B3L_Vect2Normalize(vect2_t* pV, vect2_t* pResult) {
    f32 x = pV->x; f32 y = pV->y;
    f32 factor = FastInvertSqrt(x * x + y * y);
    pResult->x = (x * factor);
    pResult->y = (y * factor);
}

f32 B3L_Vect3Length(vect3_t* pV) {
    f32 x = pV->x; f32 y = pV->y; f32 z = pV->z;
    return B3L_Sqrtf(x * x + y * y + z * z);
}

void B3L_Vect3Normalize(vect3_t* pV, vect3_t* pResult) {
    f32 x = pV->x; f32 y = pV->y; f32 z = pV->z;
    //f32 factor = FastInvertSqrt(x*x+y*y+z*z);
    f32 factor = 1.0f / B3L_Sqrtf(x * x + y * y + z * z);
    pResult->x = (x * factor);
    pResult->y = (y * factor);
    pResult->z = (z * factor);
}
void B3L_Vect3Add(vect3_t* pVa, vect3_t* pVb, vect3_t* pResult) {
    pResult->x = pVa->x + pVb->x;
    pResult->y = pVa->y + pVb->y;
    pResult->z = pVa->z + pVb->z;
}

void B3L_Vect3Sub(vect3_t* pVa, vect3_t* pVb, vect3_t* pResult) {
    pResult->x = pVa->x - pVb->x;
    pResult->y = pVa->y - pVb->y;
    pResult->z = pVa->z - pVb->z;
}

f32 B3L_Vect3Dot(vect3_t* pA, vect3_t* pB) {
    return (pA->x * pB->x + pA->y * pB->y + pA->z * pB->z);
}

void B3L_Vect3Cross(vect3_t* pA, vect3_t* pB, vect3_t* pResult) {
    pResult->x = pA->y * pB->z - pA->z * pB->y;
    pResult->y = pA->z * pB->x - pA->x * pB->z;
    pResult->z = pA->x * pB->y - pA->y * pB->x;
}

void B3L_Vect3Interp(vect3_t* pVa, vect3_t* pVb, vect3_t* pResult, f32 t) {
    pResult->x = Interp_f(pVa->x, pVb->x, t);
    pResult->y = Interp_f(pVa->y, pVb->y, t);
    pResult->z = Interp_f(pVa->z, pVb->z, t);
}

void B3L_Vect3Scale(vect3_t* pV, f32 scale, vect3_t* pResult) {
    pResult->x = scale * pV->x;
    pResult->y = scale * pV->y;
    pResult->z = scale * pV->z;
}


/*-----------------------------------------------------------------------------
Matrix functions
-----------------------------------------------------------------------------*/
void B3L_InitUnitMat3(mat3_t* pMat) {
    pMat->m00 = 1.0f; pMat->m01 = 0.0f; pMat->m02 = 0.0f;
    pMat->m10 = 0.0f; pMat->m11 = 1.0f; pMat->m12 = 0.0f;
    pMat->m20 = 0.0f; pMat->m21 = 0.0f; pMat->m22 = 1.0f;
}

void B3L_Mat3XRotate(mat3_t* pMat, f32 angle) {
    f32 cosp = B3L_cos(angle);
    f32 sinp = B3L_sin(angle);
    pMat->m00 = 1.0f; pMat->m01 = 0.0f; pMat->m02 = 0.0f;
    pMat->m10 = 0.0f; pMat->m11 = cosp; pMat->m12 = -sinp;
    pMat->m20 = 0.0f; pMat->m21 = sinp; pMat->m22 = cosp;
}

/*
    Create 3*3 matrix for rotation in y axis
*/
void B3L_Mat3YRotate(mat3_t* pMat, f32 angle) {
    f32 cosh = B3L_cos(angle);
    f32 sinh = B3L_sin(angle);
    pMat->m00 = cosh; pMat->m01 = 0.0f; pMat->m02 = sinh;
    pMat->m10 = 0.0f; pMat->m11 = 1.0f; pMat->m12 = 0.0f;
    pMat->m20 = -sinh; pMat->m21 = 0.0f; pMat->m22 = cosh;
}

/*
    Create 3*3 matrix for rotation in z axis
*/
void B3L_Mat3ZRotate(mat3_t* pMat, f32 angle) {
    f32 cosb = B3L_cos(angle);
    f32 sinb = B3L_sin(angle);
    pMat->m00 = cosb; pMat->m01 = -sinb; pMat->m02 = 0.0f;
    pMat->m10 = sinb; pMat->m11 = cosb; pMat->m12 = 0.0f;
    pMat->m20 = 0.0f; pMat->m21 = 0.0f; pMat->m22 = 1.0f;
}



void B3L_InitUnitMat4(mat4_t* pMat) {
#define M(x,y) (pMat)->m##x##y
    M(0, 0) = 1.0f; M(1, 0) = 0.0f; M(2, 0) = 0.0f; M(3, 0) = 0.0f;
    M(0, 1) = 0.0f; M(1, 1) = 1.0f; M(2, 1) = 0.0f; M(3, 1) = 0.0f;
    M(0, 2) = 0.0f; M(1, 2) = 0.0f; M(2, 2) = 1.0f; M(3, 2) = 0.0f;
    M(0, 3) = 0.0f; M(1, 3) = 0.0f; M(2, 3) = 0.0f; M(3, 3) = 1.0f;
#undef M
}

void B3L_TransposeMat4(mat4_t* pMat) {
    f32 temp;
    temp = ((f32*)pMat)[1];
    ((f32*)pMat)[1] = ((f32*)pMat)[4];
    ((f32*)pMat)[4] = temp;

    temp = ((f32*)pMat)[2];
    ((f32*)pMat)[2] = ((f32*)pMat)[8];
    ((f32*)pMat)[8] = temp;

    temp = ((f32*)pMat)[6];
    ((f32*)pMat)[6] = ((f32*)pMat)[9];
    ((f32*)pMat)[9] = temp;

    temp = ((f32*)pMat)[3];
    ((f32*)pMat)[3] = ((f32*)pMat)[12];
    ((f32*)pMat)[12] = temp;

    temp = ((f32*)pMat)[7];
    ((f32*)pMat)[7] = ((f32*)pMat)[13];
    ((f32*)pMat)[13] = temp;

    temp = ((f32*)pMat)[11];
    ((f32*)pMat)[11] = ((f32*)pMat)[14];
    ((f32*)pMat)[14] = temp;
}

//mat1 * mat2 -> mat3, it is safe to set mat1 same as mat3
void B3L_Mat4XMat4(mat4_t* pMat1, mat4_t* pMat2, mat4_t* pMat3) {
    f32 t0, t1, t2, t3;
    f32 s0, s1, s2, s3;
#define M(x,y) (pMat1)->m##x##y
#define N(x,y) (pMat2)->m##x##y
#define O(x,y) (pMat3)->m##x##y
    s0 = M(0, 0); s1 = M(1, 0); s2 = M(2, 0); s3 = M(3, 0);
    t0 = s0 * N(0, 0) + s1 * N(0, 1) + s2 * N(0, 2) + s3 * N(0, 3);
    t1 = s0 * N(1, 0) + s1 * N(1, 1) + s2 * N(1, 2) + s3 * N(1, 3);
    t2 = s0 * N(2, 0) + s1 * N(2, 1) + s2 * N(2, 2) + s3 * N(2, 3);
    t3 = s0 * N(3, 0) + s1 * N(3, 1) + s2 * N(3, 2) + s3 * N(3, 3);
    O(0, 0) = t0; O(1, 0) = t1; O(2, 0) = t2; O(3, 0) = t3;
    s0 = M(0, 1); s1 = M(1, 1); s2 = M(2, 1); s3 = M(3, 1);
    t0 = s0 * N(0, 0) + s1 * N(0, 1) + s2 * N(0, 2) + s3 * N(0, 3);
    t1 = s0 * N(1, 0) + s1 * N(1, 1) + s2 * N(1, 2) + s3 * N(1, 3);
    t2 = s0 * N(2, 0) + s1 * N(2, 1) + s2 * N(2, 2) + s3 * N(2, 3);
    t3 = s0 * N(3, 0) + s1 * N(3, 1) + s2 * N(3, 2) + s3 * N(3, 3);
    O(0, 1) = t0; O(1, 1) = t1; O(2, 1) = t2; O(3, 1) = t3;
    s0 = M(0, 2); s1 = M(1, 2); s2 = M(2, 2); s3 = M(3, 2);
    t0 = s0 * N(0, 0) + s1 * N(0, 1) + s2 * N(0, 2) + s3 * N(0, 3);
    t1 = s0 * N(1, 0) + s1 * N(1, 1) + s2 * N(1, 2) + s3 * N(1, 3);
    t2 = s0 * N(2, 0) + s1 * N(2, 1) + s2 * N(2, 2) + s3 * N(2, 3);
    t3 = s0 * N(3, 0) + s1 * N(3, 1) + s2 * N(3, 2) + s3 * N(3, 3);
    O(0, 2) = t0; O(1, 2) = t1; O(2, 2) = t2; O(3, 2) = t3;
    s0 = M(0, 3); s1 = M(1, 3); s2 = M(2, 3); s3 = M(3, 3);
    t0 = s0 * N(0, 0) + s1 * N(0, 1) + s2 * N(0, 2) + s3 * N(0, 3);
    t1 = s0 * N(1, 0) + s1 * N(1, 1) + s2 * N(1, 2) + s3 * N(1, 3);
    t2 = s0 * N(2, 0) + s1 * N(2, 1) + s2 * N(2, 2) + s3 * N(2, 3);
    t3 = s0 * N(3, 0) + s1 * N(3, 1) + s2 * N(3, 2) + s3 * N(3, 3);
    O(0, 3) = t0; O(1, 3) = t1; O(2, 3) = t2; O(3, 3) = t3;
#undef M
#undef N
#undef O
}


/*
    matrix 3*3 B = AB
*/
void B3L_Mat3MultMat3ABB(mat3_t* pMatA, mat3_t* pMatB) {
    f32 t0, t1, t2;
    f32 s0, s1, s2;
#define M(x,y) (pMatA)->m##x##y
#define N(x,y) (pMatB)->m##x##y
    s0 = N(0, 0); s1 = N(0, 1); s2 = N(0, 2);
    t0 = s0 * M(0, 0) + s1 * M(1, 0) + s2 * M(2, 0);
    t1 = s0 * M(0, 1) + s1 * M(1, 1) + s2 * M(2, 1);
    t2 = s0 * M(0, 2) + s1 * M(1, 2) + s2 * M(2, 2);
    N(0, 0) = t0; N(0, 1) = t1; N(0, 2) = t2;
    s0 = N(1, 0); s1 = N(1, 1); s2 = N(1, 2);
    t0 = s0 * M(0, 0) + s1 * M(1, 0) + s2 * M(2, 0);
    t1 = s0 * M(0, 1) + s1 * M(1, 1) + s2 * M(2, 1);
    t2 = s0 * M(0, 2) + s1 * M(1, 2) + s2 * M(2, 2);
    N(1, 0) = t0; N(1, 1) = t1; N(1, 2) = t2;
    s0 = N(2, 0); s1 = N(2, 1); s2 = N(2, 2);
    t0 = s0 * M(0, 0) + s1 * M(1, 0) + s2 * M(2, 0);
    t1 = s0 * M(0, 1) + s1 * M(1, 1) + s2 * M(2, 1);
    t2 = s0 * M(0, 2) + s1 * M(1, 2) + s2 * M(2, 2);
    N(2, 0) = t0; N(2, 1) = t1; N(2, 2) = t2;
#undef M
#undef N
}

/*
    matrix 3*3 A = AB
*/
void B3L_Mat3MultMat3ABA(mat3_t* pMatA, mat3_t* pMatB) {
    f32 t0, t1, t2;
    f32 s0, s1, s2;
#define M(x,y) (pMatA)->m##x##y
#define N(x,y) (pMatB)->m##x##y
    s0 = M(0, 0); s1 = M(1, 0); s2 = M(2, 0);
    t0 = s0 * N(0, 0) + s1 * N(0, 1) + s2 * N(0, 2);
    t1 = s0 * N(1, 0) + s1 * N(1, 1) + s2 * N(1, 2);
    t2 = s0 * N(2, 0) + s1 * N(2, 1) + s2 * N(2, 2);
    M(0, 0) = t0; M(1, 0) = t1; M(2, 0) = t2;
    s0 = M(0, 1); s1 = M(1, 1); s2 = M(2, 1);
    t0 = s0 * N(0, 0) + s1 * N(0, 1) + s2 * N(0, 2);
    t1 = s0 * N(1, 0) + s1 * N(1, 1) + s2 * N(1, 2);
    t2 = s0 * N(2, 0) + s1 * N(2, 1) + s2 * N(2, 2);
    M(0, 1) = t0; M(1, 1) = t1; M(2, 1) = t2;
    s0 = M(0, 2); s1 = M(1, 2); s2 = M(2, 2);
    t0 = s0 * N(0, 0) + s1 * N(0, 1) + s2 * N(0, 2);
    t1 = s0 * N(1, 0) + s1 * N(1, 1) + s2 * N(1, 2);
    t2 = s0 * N(2, 0) + s1 * N(2, 1) + s2 * N(2, 2);
    M(0, 2) = t0; M(1, 2) = t1; M(2, 2) = t2;
#undef M
#undef N
}

void B3L_CreateScaleMatrix(f32 scaleX, f32 scaleY, f32 scaleZ, mat4_t* pMat) {
#define M(x,y) (pMat)->m##x##y

    M(0, 0) = scaleX; M(1, 0) = 0.0f;   M(2, 0) = 0.0f;   M(3, 0) = 0.0f;
    M(0, 1) = 0.0f;   M(1, 1) = scaleY; M(2, 1) = 0.0f;   M(3, 1) = 0.0f;
    M(0, 2) = 0.0f;   M(1, 2) = 0.0f;   M(2, 2) = scaleZ; M(3, 2) = 0.0f;
    M(0, 3) = 0.0f;   M(1, 3) = 0.0f;   M(2, 3) = 0.0f;   M(3, 3) = 1.0f;

#undef M
}

void B3L_CreateTranslationMat(f32 offsetX, f32 offsetY, f32 offsetZ, mat4_t* pMat) {
#define M(x,y) (pMat)->m##x##y
    f32 one = 1.0f;
    M(0, 0) = one; M(1, 0) = 0.0f; M(2, 0) = 0.0f; M(3, 0) = 0.0f;
    M(0, 1) = 0.0f; M(1, 1) = one; M(2, 1) = 0.0f; M(3, 1) = 0.0f;
    M(0, 2) = 0.0f; M(1, 2) = 0.0f; M(2, 2) = one; M(3, 2) = 0.0f;
    M(0, 3) = offsetX; M(1, 3) = offsetY; M(2, 3) = offsetZ; M(3, 3) = one;
#undef M
}

/*-------------------------------------------------------------------------------------------------
Create single obj to world matrix
-------------------------------------------------------------------------------------------------*/
void B3L_CreateO2WMat(mat3_t* pRMat, vect3_t* pTranslation, vect3_t* pScale, mat4_t* pResult) {
    f32 sx = pScale->x; f32 sy = pScale->y; f32 sz = pScale->z;
    pResult->m03 = pTranslation->x; pResult->m13 = pTranslation->y; pResult->m23 = pTranslation->z;
    pResult->m00 = pRMat->m00 * sx; pResult->m01 = pRMat->m01 * sy; pResult->m02 = pRMat->m02 * sz;
    pResult->m10 = pRMat->m10 * sx; pResult->m11 = pRMat->m11 * sy; pResult->m12 = pRMat->m12 * sz;
    pResult->m20 = pRMat->m20 * sx; pResult->m21 = pRMat->m21 * sy; pResult->m22 = pRMat->m22 * sz;
    pResult->m30 = 0.0f; pResult->m31 = 0.0f; pResult->m32 = 0.0f; pResult->m33 = 1.0f;
}


/*-------------------------------------------------------------------------------------------------
Traverse all mother objs chain to generate final object to world matrix
-------------------------------------------------------------------------------------------------*/
static void B3L_CreateO2WChainMatrix(B3LObj_t* pObj, mat4_t* pResult) {
    //mat4_t o2wMat;
    mat3_t rMat;
    B3L_QuaternionToMatrix(&(pObj->transform.quaternion), &(rMat));
    vect3_t* pTrans = &(pObj->transform.translation);
    vect3_t* pScale = &(pObj->transform.scale);
    B3L_CreateO2WMat(&rMat, pTrans, pScale, pResult);
    B3LObj_t* pMotherObj = pObj->pMother;
    if (pMotherObj != (B3LObj_t*)NULL) {       
        //has mother obj
        mat4_t motherMat;
        B3L_CreateO2WChainMatrix(pMotherObj,&motherMat);
        //B3L_Mat4XMat4(&o2wMat, &motherMat, pResult);
        B3L_Mat4XMat4(pResult, &motherMat, pResult);
    }


}

/*-------------------------------------------------------------------------------------------------
Generate obj to clip space matrix
-------------------------------------------------------------------------------------------------*/
void B3L_CreateO2CMatrix(B3LObj_t* pObj,mat4_t* pCamMat, mat4_t* pW2OMat,mat4_t* pO2CMat) {
    mat4_t o2wMat;
    //mat3_t rmat;
    //B3L_QuaternionToMatrix(&(pObj->transform.quaternion), &(rmat));
    B3L_CreateO2WChainMatrix(pObj,&o2wMat);
    B3L_Mat4XMat4(&o2wMat, pCamMat, pO2CMat);
    B3L_InvertMat4(&o2wMat, pW2OMat); //now the obj r matrix contants the inverse matrix w2o

}

void B3L_InvertMat4(mat4_t* pMatS, mat4_t* pMatI) {

}

void B3L_InvertMat33(mat3_t* pMatS, mat3_t* pMatI) {
    f32 det;
#define S(x,y) (pMatS)->m##x##y
#define I(x,y) (pMatI)->m##x##y
    I(0, 0) = +S(1, 1) * S(2, 2) - S(2, 1) * S(1, 2);
    I(1, 0) = -S(1, 0) * S(2, 2) + S(2, 0) * S(1, 2);
    I(2, 0) = +S(1, 0) * S(2, 1) - S(2, 0) * S(1, 1);
    I(0, 1) = -S(0, 1) * S(2, 2) + S(2, 1) * S(0, 2);
    I(1, 1) = +S(0, 0) * S(2, 2) - S(2, 0) * S(0, 2);
    I(2, 1) = -S(0, 0) * S(2, 1) + S(2, 0) * S(0, 1);
    I(0, 2) = +S(0, 1) * S(1, 2) - S(1, 1) * S(0, 2);
    I(1, 2) = -S(0, 0) * S(1, 2) + S(1, 0) * S(0, 2);
    I(2, 2) = +S(0, 0) * S(1, 1) - S(1, 0) * S(0, 1);

    det = S(0, 0) * I(0, 0) + S(1, 0) * I(0, 1) + S(2, 0) * I(0, 2);
    det = 1.0f / det;
    I(0, 0) *= det;
    I(1, 0) *= det;
    I(2, 0) *= det;
    I(0, 1) *= det;
    I(1, 1) *= det;
    I(2, 1) *= det;
    I(0, 2) *= det;
    I(1, 2) *= det;
    I(2, 2) *= det;

#undef S(x,y)
#undef I(x,y)
}
void B3L_InvertMat43(mat4_t* pMatS, mat3_t* pMatI) {
    f32 det;
#define S(x,y) (pMatS)->m##x##y
#define I(x,y) (pMatI)->m##x##y
    I(0,0) = + S(1,1) * S(2,2) - S(2,1) * S(1,2);
    I(1,0) = - S(1,0) * S(2,2) + S(2,0) * S(1,2);
    I(2,0) = + S(1,0) * S(2,1) - S(2,0) * S(1,1);
    I(0,1) = - S(0,1) * S(2,2) + S(2,1) * S(0,2);
    I(1,1) = + S(0,0) * S(2,2) - S(2,0) * S(0,2);
    I(2,1) = - S(0,0) * S(2,1) + S(2,0) * S(0,1);
    I(0,2) = + S(0,1) * S(1,2) - S(1,1) * S(0,2);
    I(1,2) = - S(0,0) * S(1,2) + S(1,0) * S(0,2);
    I(2,2) = + S(0,0) * S(1,1) - S(1,0) * S(0,1);

    det = S(0, 0) * I(0, 0) + S(1, 0) * I(0, 1) + S(2, 0) * I(0, 2);
    det = 1.0f / det;
    I(0,0) *= det;
    I(1,0) *= det;
    I(2,0) *= det;
    I(0,1) *= det;
    I(1,1) *= det;
    I(2,1) *= det;
    I(0,2) *= det;
    I(1,2) *= det;
    I(2,2) *= det;

#undef S(x,y)
#undef I(x,y)
}

void B3L_Vect3Xmat4(vect3_t* pV, mat4_t* pMat, vect4_t* pResult) {
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

void B3L_Vect3XMat3(vect3_t* pV, mat3_t* pMat, vect3_t* pResult) {
    f32 x = pV->x; f32 y = pV->y; f32 z = pV->z;
    f32 rx, ry, rz;

#define dotCol(col)\
        (x*(pMat->m##col##0)) +\
        (y*(pMat->m##col##1)) +\
        (z*(pMat->m##col##2)) 

    rx = dotCol(0);
    ry = dotCol(1);
    rz = dotCol(2);

    pResult->x = rx;
    pResult->y = ry;
    pResult->z = rz;
#undef dotCol 
}

void MakeClipMatrix(u32 state, f32 near_plane, f32 far_plane,
    f32 focalLength, f32 aspectRatio, mat4_t* pMat) {
    f32 zero = 0.0f;
    f32 one = 1.0f;
#define M(x,y) (pMat)->m##x##y
    if (B3L_TEST(state, B3L_PROJECT_MODE) == PERSPECTIVE_PROJECT) {
        M(0, 0) = focalLength; M(1, 0) = zero;   M(2, 0) = zero;   M(3, 0) = zero;
        M(0, 1) = zero;   M(1, 1) = focalLength * aspectRatio; M(2, 1) = zero;   M(3, 1) = zero;
        M(0, 2) = zero;   M(1, 2) = zero;   M(2, 2) = far_plane / (far_plane - near_plane); M(3, 2) = one;
        M(0, 3) = zero;   M(1, 3) = zero;   M(2, 3) = -near_plane * far_plane / (far_plane - near_plane);   M(3, 3) = zero;
    }
    else {
        M(0, 0) = focalLength; M(1, 0) = zero;   M(2, 0) = zero;   M(3, 0) = zero;
        M(0, 1) = zero;   M(1, 1) = focalLength * aspectRatio; M(2, 1) = zero;   M(3, 1) = zero;
        M(0, 2) = zero;   M(1, 2) = zero;   M(2, 2) = one / (far_plane - near_plane); M(3, 2) = zero;
        M(0, 3) = zero;   M(1, 3) = zero;   M(2, 3) = near_plane / (far_plane - near_plane);   M(3, 3) = one;
    }

#undef M
}

/*-----------------------------------------------------------------------------
Rotation convert functions
-----------------------------------------------------------------------------*/
void B3L_EulerToMatrix(euler3_t* pEuler, mat3_t* pMat) {
    f32 byX = pEuler->x; f32 byY = pEuler->y; f32 byZ = pEuler->z;
    f32 sx = B3L_sin(byX); f32 sy = B3L_sin(byY); f32 sz = B3L_sin(byZ);
    f32 cx = B3L_cos(byX); f32 cy = B3L_cos(byY); f32 cz = B3L_cos(byZ);
#define M(x,y) (pMat)->m##x##y
    M(0, 0) = (cy * cz) + (sy * sx * sz);
    M(0, 1) = (cz * sy * sx) - (cy * sz);
    M(0, 2) = (cx * sy);
    M(1, 0) = (cx * sz);
    M(1, 1) = (cx * cz);
    M(1, 2) = -1.0f * sx;
    M(2, 0) = (cy * sx * sz) - (cz * sy);
    M(2, 1) = (cy * cz * sx) + (sy * sz);
    M(2, 2) = (cy * cx);
#undef M

}
void B3L_MatrixToEuler(mat3_t* pMat, euler3_t* pEuler) {
    f32 sp = -pMat->m12;
    if (sp <= -1.0f) {
        pEuler->x = -0.25f;
    }
    else if (sp >= 1.0f) {
        pEuler->x = 0.25f;
    }
    else {
        pEuler->x = B3L_asin(sp);
    }
    if (B3L_Absf(sp) > 0.9999f) {
        pEuler->z = 0.0f;
        pEuler->y = B3L_atan2(-pMat->m02, pMat->m00);
    }
    else {
        pEuler->y = B3L_atan2(pMat->m02, pMat->m22);
        pEuler->z = B3L_atan2(pMat->m10, pMat->m11);
    }
}

void B3L_QuaternionToMatrix(quat4_t* pQuat, mat3_t* pMat) {
    f32 x = pQuat->x; f32 y = pQuat->y; f32 z = pQuat->z; f32 w = pQuat->w;
    pMat->m00 = 1.0f - 2.0f * (y * y + z * z);
    pMat->m01 = 2.0f * (x * y - w * z);
    pMat->m02 = 2.0f * (x * z + w * y);
    pMat->m10 = 2.0f * (x * y + w * z);
    pMat->m11 = 1.0f - 2.0f * (x * x + z * z);
    pMat->m12 = 2.0f * (y * z - w * x);
    pMat->m20 = 2.0f * (x * z - w * y);
    pMat->m21 = 2.0f * (y * z + w * x);
    pMat->m22 = 1.0f - 2.0f * (x * x + y * y);
}

void B3L_MatrixToQuaternion(mat3_t* pMat, quat4_t* pQuat) {
    f32 fWSM = pMat->m00 + pMat->m11 + pMat->m22;
    f32 fXSM = pMat->m00 - pMat->m11 - pMat->m22;
    f32 fYSM = pMat->m11 - pMat->m00 - pMat->m22;
    f32 fZSM = pMat->m22 - pMat->m00 - pMat->m11;
    s32 biggestIndex = 0;
    f32 fBSM = fWSM;
    if (fXSM > fBSM) {
        fBSM = fXSM;
        biggestIndex = 1;
    }
    if (fYSM > fBSM) {
        fBSM = fYSM;
        biggestIndex = 2;
    }
    if (fZSM > fBSM) {
        fBSM = fZSM;
        biggestIndex = 3;
    }
    f32 biggestVal = B3L_Sqrtf(fBSM + 1.0f) * 0.5f;
    f32 mult = 0.25f / biggestVal;
    switch (biggestIndex) {
    case 0:
        pQuat->w = biggestVal;
        pQuat->x = (pMat->m21 - pMat->m12) * mult;
        pQuat->y = (pMat->m02 - pMat->m20) * mult;
        pQuat->z = (pMat->m10 - pMat->m01) * mult;
        break;
    case 1:
        pQuat->x = biggestVal;
        pQuat->w = (pMat->m21 - pMat->m12) * mult;
        pQuat->y = (pMat->m10 - pMat->m01) * mult;
        pQuat->z = (pMat->m02 + pMat->m20) * mult;
        break;
    case 2:
        pQuat->y = biggestVal;
        pQuat->w = (pMat->m02 - pMat->m20) * mult;
        pQuat->x = (pMat->m10 - pMat->m01) * mult;
        pQuat->z = (pMat->m21 + pMat->m12) * mult;
        break;
    case 3:
        pQuat->z = biggestVal;
        pQuat->w = (pMat->m10 - pMat->m01) * mult;
        pQuat->x = (pMat->m02 - pMat->m20) * mult;
        pQuat->y = (pMat->m21 + pMat->m12) * mult;
        break;
    }
}

void B3L_EulerToQuaternion(euler3_t* pEuler, quat4_t* pQuat) {
    f32 cp = B3L_cos(pEuler->x * 0.5f); f32 ch = B3L_cos(pEuler->y * 0.5f);
    f32 cb = B3L_cos(pEuler->z * 0.5f); f32 sp = B3L_sin(pEuler->x * 0.5f);
    f32 sh = B3L_sin(pEuler->y * 0.5f); f32 sb = B3L_sin(pEuler->z * 0.5f);
    pQuat->w = ch * cp * cb + sh * sp * sb;
    pQuat->x = ch * sp * cb + sh * cp * sb;
    pQuat->y = sh * cp * cb - ch * sp * sb;
    pQuat->z = ch * cp * sb - sh * sp * cb;
}


void B3L_QuaternionToEuler(quat4_t* pQuat, euler3_t* pEuler) {
    f32 x = pQuat->x; f32 y = pQuat->y; f32 z = pQuat->z; f32 w = pQuat->w;
    f32 sp = -2.0f * (y * z - w * x);
    if (B3L_Absf(sp) > 0.9999f) {
        pEuler->x = 0.25f * sp;
        pEuler->y = B3L_atan2(-x * z + w * y, 0.5f - y * y - z * z);
        pEuler->z = 0.0f;
    }
    else {
        pEuler->x = B3L_asin(sp);
        pEuler->y = B3L_atan2(x * z + w * y, 0.5f - x * x - y * y);
        pEuler->z = B3L_atan2(x * y + w * z, 0.5f - x * x - z * z);
    }
}


/*-----------------------------------------------------------------------------
Quaternion functions
-----------------------------------------------------------------------------*/

void B3L_QuatMult(quat4_t* pL, quat4_t* pR, quat4_t* pResult) {
    f32 x1 = pL->x; f32 y1 = pL->y; f32 z1 = pL->z; f32 w1 = pL->w;
    f32 x2 = pR->x; f32 y2 = pR->y; f32 z2 = pR->z; f32 w2 = pR->w;
    pResult->x = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
    pResult->y = w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2;
    pResult->z = w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2;
    pResult->w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
}

f32  B3L_QuatDot(quat4_t* pL, quat4_t* pR) {
    return pL->w * pR->w + pL->x * pR->x + pL->y * pR->y + pL->z * pR->z;
}

void B3L_CreateQuaternionByAxisAngle(vect3_t* pAxis, f32 angle, quat4_t* pResult) {
    f32 halfAngle = 0.5f * angle;
    f32 sinh = B3L_sin(halfAngle);
    f32 cosh = B3L_cos(halfAngle);
    pResult->x = pAxis->x * sinh;
    pResult->y = pAxis->y * sinh;
    pResult->z = pAxis->z * sinh;
    pResult->w = cosh;
}

void B3L_FromToRotation(vect3_t* pFrom, vect3_t* pTo, quat4_t* pResult) {
    vect3_t fN;
    vect3_t tN;
    B3L_Vect3Normalize(pFrom, &fN);
    B3L_Vect3Normalize(pTo, &tN);
    vect3_t normal;
    B3L_Vect3Cross(pFrom, pTo, &normal);
    f32 cosA = B3L_Vect3Dot(&fN, &tN);
    f32 cosh = B3L_Sqrtf(0.5f + cosA * 0.5f);
    f32 sinh = B3L_Sqrtf(0.5f - cosA * 0.5f);
    pResult->x = normal.x * sinh;
    pResult->y = normal.y * sinh;
    pResult->z = normal.z * sinh;
    pResult->w = cosh;
}


//create a quaternion that rotates vector a to vector b
void B3L_LookRotation(vect3_t* pA, vect3_t* pB, vect3_t* pUp, quat4_t* pResult) {
    vect3_t aN;
    vect3_t bN;
    vect3_t uN;
    B3L_Vect3Normalize(pA, &aN);
    B3L_Vect3Normalize(pB, &bN);
    B3L_Vect3Normalize(pUp, &uN);
    f32 dot = B3L_Vect3Dot(&aN, &bN);

    if (dot < -0.9999f) {
        //printf("less than -0.99f\n");
        B3L_CreateQuaternionByAxisAngle(&uN, 0.5f, pResult);
        return;
    }
    else if (dot > 0.9999f) {
        //printf("> than 0.99f\n");
        SET_IDENTITY_P_QUAT(pResult);
        return;
    }

    f32 rotAngle = 0.25f - B3L_asin(dot);
    vect3_t ortAxis;
    B3L_Vect3Cross(&aN, &bN, &ortAxis);
    B3L_Vect3Normalize(&ortAxis, &ortAxis);
    B3L_CreateQuaternionByAxisAngle(&ortAxis, rotAngle, pResult);
}

void B3L_CreateLookAtQuaternion(vect3_t* pFrom, vect3_t* pAt, vect3_t* pUp, quat4_t* pResult) {
    vect3_t toObj;
    B3L_Vect3Sub(pAt, pFrom, &toObj);
    vect3_t zAxis = { 0.0f,0.0f,1.0f };
    B3L_LookRotation(&zAxis, &toObj, pUp, pResult);
}

void B3L_QuatCreateXRotate(quat4_t* pQ, f32 angle) {
    f32 halfAngle = 0.5f * angle;
    f32 cosh = B3L_cos(halfAngle); f32 sinh = B3L_sin(halfAngle);
    pQ->w = cosh; pQ->x = sinh; pQ->y = 0.0f; pQ->z = 0.0f;
}

void B3L_QuatCreateYRotate(quat4_t* pQ, f32 angle) {
    f32 halfAngle = 0.5f * angle;
    f32 cosh = B3L_cos(halfAngle); f32 sinh = B3L_sin(halfAngle);
    pQ->w = cosh; pQ->x = 0.0f; pQ->y = sinh; pQ->z = 0.0f;
}

void B3L_QuatCreateZRotate(quat4_t* pQ, f32 angle) {
    f32 halfAngle = 0.5f * angle;
    f32 cosh = B3L_cos(halfAngle); f32 sinh = B3L_sin(halfAngle);
    pQ->w = cosh; pQ->x = 0.0f; pQ->y = 0.0f; pQ->z = sinh;
}

void B3L_QuaternionInterp(quat4_t* pFrom, quat4_t* pTo, quat4_t* pResult, f32 t) {
    f32 w0 = pFrom->w; f32 x0 = pFrom->x; f32 y0 = pFrom->y; f32 z0 = pFrom->z;
    f32 w1 = pTo->w; f32 x1 = pTo->x; f32 y1 = pTo->y; f32 z1 = pTo->z;
    f32 cosOmega = w0 * w1 + x0 * x1 + y0 * y1 + z0 * z1;
    if (cosOmega < 0.0f) {
        w1 = -w1; x1 = -x1; y1 = -y1; z1 = -z1;
        cosOmega = -cosOmega;
    }
    f32 k0, k1;
    if (cosOmega > 0.9999f) {
        k0 = 1.0f - t;
        k1 = t;
    }
    else {
        f32 sinOmega = B3L_Sqrtf(1.0f - cosOmega * cosOmega);
        f32 omega = B3L_atan2(sinOmega, cosOmega);
        f32 oneOverSinOmega = 1.0f / sinOmega;
        k0 = B3L_sin((1.0f - t) * omega) * oneOverSinOmega;
        k1 = B3L_sin(t * omega) * oneOverSinOmega;
    }
    pResult->w = w0 * k0 + w1 * k1;
    pResult->x = x0 * k0 + x1 * k1;
    pResult->y = y0 * k0 + y1 * k1;
    pResult->z = z0 * k0 + z1 * k1;
}


