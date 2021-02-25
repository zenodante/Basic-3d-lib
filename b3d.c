#include "b3d.h"

#include <string.h>

#if B3L_ARM  == 1


static void B3d_FPU_Init(void);

static void B3d_FPU_Init() {
  //set round towards Plus Infinity (RP) mode
  u32 temp = __get_FPSCR();
  temp &= 0xFF3FFFFF; //reset R mode to 00( round to nearest)
  //temp |= 0x00400000; 
  __set_FPSCR(temp);
}
#endif


static void  UpdateCam(render_t* pRender);




void B3L_RenderInit(render_t* pRender, fBuff_t* pFrameBuff,u32 objNum, u32 vectBuffSize,
                    f32 lv0Distance, f32 lv1Distance, s8 lv1DefaultLight, f32 farPlane,f32 nearPlane) {
#if B3L_ARM  == 1
  B3d_FPU_Init();
#endif
  pRender->pFrameBuff = pFrameBuff;
  pRender->pZBuff = zbuff; 
  pRender->scene.pObjBuff = (B3LObj_t *)pvPortMalloc(sizeof(B3LObj_t) * objNum, B3L_DATA_OTHER_E, B3L_MEM_HIGH_PRIORITY);
  pRender->pVectBuff = (vect4_t *)pvPortMalloc(sizeof(vect4_t)* vectBuffSize,B3L_DATA_OTHER_E, B3L_MEM_HIGH_PRIORITY);
  pRender->lvl0Distance = lv0Distance;
  pRender->lvl1Distance = lv1Distance;
  pRender->lvl1Light = lv1DefaultLight;
  pRender->farPlane = farPlane;
  pRender->nearPlane = nearPlane;
  B3L_ResetScene(&(pRender->scene),objNum);
  B3L_InitCamera(pRender);
  B3L_ResetLight(&(pRender->light));
}


void B3L_RenderDeInit(render_t* pRender,bool resetBuff) {
    pRender->pFrameBuff = NULL;
    pRender->pZBuff = NULL;
    vPortFree(pRender->scene.pObjBuff);//release memory
    pRender->scene.pObjBuff = NULL;
    vPortFree(pRender->pVectBuff);//release memory
    pRender->pVectBuff = NULL;
    pRender->lvl0Distance = 0.0f;
    pRender->lvl1Distance = 0.0f;
    pRender->lvl1Light = 0;
    pRender->farPlane = 1.0f;
    pRender->nearPlane = 100.0f;
    if (resetBuff) {
        prvHeapInit();
        pRender->pBuffResouce = NULL;
    }
}

void B3L_RenderScence(render_t* pRender,u32 time) {
    ClearZbuff(pRender->pZBuff, Z_BUFF_LENGTH);
    ClearFbuff(pRender->pFrameBuff, F_BUFF_LENGTH,8);
    UpdateCam(pRender);
    RenderObjs(pRender,time);
}

void B3L_ResetScene(scene_t* pScene,u32 freeObjNum) {
    pScene->freeObjNum = freeObjNum; //reset free obj numbers
    pScene->pActiveObjs = (B3LObj_t*)NULL; //reset active obj list
    B3L_ResetObjList(pScene,freeObjNum);
#ifdef B3L_USING_PARTICLE
    pScene->pActiveParticleGenObjs = (B3LObj_t*)NULL;//reset particle generator obj list 
    pScene->freeParticleNum = B3L_PARTICLE_BUFF_DEPTH;   //reset particle numbers
    ResetParticleList(particleBuff, &(pScene->pfreeParticles), B3L_PARTICLE_BUFF_DEPTH); //reset particle list
    //call reset particle one-way list function
#endif
}



void B3L_NewRenderStart(render_t* pRender, fBuff_t color) {

    ClearFbuff(pRender->pFrameBuff, F_BUFF_LENGTH,color );
    //ClearFrameBuff(pRender->pFrameBuff,0xFF003423,Z_BUFF_LENTH);//need to add shift
    ClearZbuff(pRender->pZBuff, Z_BUFF_LENGTH);//need to add shift

}


void B3L_Update(render_t* pRender, u32 time) {
    static u32 oldTime = 0;
    if (oldTime == 0) {//first time run
        oldTime = time;
        return;
    }
    u32 deltaTime = time - oldTime;
    
#ifdef B3L_USING_PARTICLE
    UpdateParticleObjs(pRender, time);
#endif
    oldTime = time;//update oldTime only it is run the update codes

    //TODO: Add particle update and other hook here
}

/*-----------------------------------------------------------------------------
Render obj functions
-----------------------------------------------------------------------------*/


/*
B3LParticleGenObj_t* B3L_GetFreeParticleGeneratorObj(render_t* pRender) {
    B3LObj_t* pObj = B3L_GetFreeObj(pRender);
    if (pObj != (B3LObj_t*)NULL) {
        B3L_SET(pObj->state, PARTICLE_GEN_OBJ);
        B3L_SET(pObj->state, OBJ_VISUALIZABLE);
        B3L_SET(pObj->state, OBJ_PARTICLE_ACTIVE);
        ((B3LParticleGenObj_t*)pObj)->particleNum = 0;
        ((B3LParticleGenObj_t*)pObj)->pParticleActive = (B3L_Particle_t*)NULL;
        ((B3LParticleGenObj_t*)pObj)->lastTime = 0;
    }
    return (B3LParticleGenObj_t*)pObj;
}
*/







/*-----------------------------------------------------------------------------
Target rotate control functions
-----------------------------------------------------------------------------*/
/*
    Rotate obj matrix in obj space by x axis
*/
void B3L_RotateObjInOX(quat4_t* pQuat, f32 angle) {
    quat4_t rqat;
    B3L_QuatCreateXRotate(&rqat, angle);
    B3L_QuatMult(pQuat, &rqat, pQuat);
}

/*
    Rotate obj matrix in obj space by y axis
*/
void B3L_RotateObjInOY(quat4_t* pQuat, f32 angle) {
    quat4_t rqat;
    B3L_QuatCreateYRotate(&rqat, angle);
    B3L_QuatMult(pQuat, &rqat, pQuat);
}

/*
    Rotate obj matrix in obj space by z axis
*/
void B3L_RotateObjInOZ(quat4_t* pQuat, f32 angle) {
    quat4_t rqat;
    B3L_QuatCreateZRotate(&rqat, angle);
    B3L_QuatMult(pQuat, &rqat, pQuat);
}

/*
    Rotate obj matrix in world space by x axis
*/
void B3L_RotateObjInWX(quat4_t* pQuat, f32 angle) {
    quat4_t rqat;
    B3L_QuatCreateXRotate(&rqat, angle);
    B3L_QuatMult(&rqat, pQuat, pQuat);
}

/*
    Rotate obj matrix in world space by y axis
*/
void B3L_RotateObjInWY(quat4_t* pQuat, f32 angle) {
    quat4_t rqat;
    B3L_QuatCreateYRotate(&rqat, angle);
    B3L_QuatMult(&rqat, pQuat, pQuat);
}

/*
    Rotate obj matrix in world space by z axis ?? may should use matrix type ABA
*/
void B3L_RotateObjInWZ(quat4_t* pQuat, f32 angle) {
    quat4_t rqat;
    B3L_QuatCreateZRotate(&rqat, angle);
    B3L_QuatMult(&rqat, pQuat, pQuat);

}

/*-----------------------------------------------------------------------------
Camera functions
-----------------------------------------------------------------------------*/

_RAM_FUNC void B3L_InitCamera(render_t* pRender) {
    camera_t* pCam = &(pRender->camera);
    pCam->aspectRate = DEFAULT_ASPECT_RATIO;
    pCam->focalLength = DEFAULT_FOCUS_LENGTH;
    B3L_VECT3_SET(pCam->transform.scale, 1.0f, 1.0f, 1.0f);
    B3L_VECT3_SET(pCam->transform.translation, 0.0f, 0.0f, 0.0f);
    B3L_VECT4_SET(pCam->transform.quaternion, 0.0f, 0.0f, 0.0f, 1.0f);
    pCam->pMother = (B3LObj_t*)NULL;

    MakeClipMatrix(pCam->state, pRender->nearPlane, pRender->farPlane, pCam->focalLength, pCam->aspectRate, &(pCam->clipMat));
    //printf("after init clip matrix:\n");
    //B3L_logMat4(pCam->clipMat);
    pCam->state = 0; //default is PERSPECTIVE_PROJECT
}

_RAM_FUNC void B3L_SetOrthographicProject(render_t* pRender) {
    B3L_SET(pRender->camera.state, B3L_PROJECT_MODE);
    B3L_UpdateClipMatrix(pRender);
}

_RAM_FUNC void B3L_SetPerspectiveProject(render_t* pRender) {
    B3L_CLR(pRender->camera.state, B3L_PROJECT_MODE);
    B3L_UpdateClipMatrix(pRender);
}

_RAM_FUNC void B3L_UpdateClipMatrix(render_t* pRender) {
    camera_t* pCam = &(pRender->camera);
    MakeClipMatrix(pCam->state, pRender->nearPlane, pRender->farPlane, pCam->focalLength, pCam->aspectRate, &(pCam->clipMat));
}

_RAM_FUNC void B3L_CamSetFocusLengthByFOV(render_t* pRender, f32 fov) {
    f32 halfFOV = 0.5f * fov;
    f32 sinh = B3L_sin(halfFOV);
    f32 cosh = B3L_cos(halfFOV);
    pRender->camera.focalLength = cosh / sinh;
    B3L_UpdateClipMatrix(pRender);
}


void B3L_CameraMoveTo(render_t* pRender, f32 x, f32 y, f32 z) {
    camera_t *pCam = &(pRender->camera);
    pCam->transform.translation.x = x;
    pCam->transform.translation.y = y;
    pCam->transform.translation.z = z;
}

void B3L_CameraMoveToV(render_t* pRender, vect3_t position) {
    camera_t *pCam = &(pRender->camera);
    pCam->transform.translation.x = position.x;
    pCam->transform.translation.y = position.y;
    pCam->transform.translation.z = position.z;
}


void B3L_CameraLookAt(camera_t* pCam, vect3_t* pAt) {
    vect3_t up = { 0.0f,1.0f,0.0f };
    B3L_CreateLookAtQuaternion(&(pCam->transform.translation),
        pAt, &up, &(pCam->transform.quaternion));
}

static void  UpdateCam(render_t* pRender) {
    camera_t* pCam = &(pRender->camera);
    mat3_t rotateMat;
    mat4_t o2wMat;
    B3L_CreateO2WChainMatrixForCam(pCam, &o2wMat);
    vect3_t camPosition;
    camPosition.x = -o2wMat.m03;
    camPosition.y = -o2wMat.m13;
    camPosition.z = -o2wMat.m23;

    B3L_CreateO2WChainMatrixOnlyRotationForCam(pCam, &rotateMat);
    B3L_InvertMat3(&rotateMat, &rotateMat);
    B3L_GenerateW2CMat(&(o2wMat), &rotateMat, &camPosition);
    B3L_Mat4XMat4(&(o2wMat), &(pCam->clipMat), &(pCam->camW2CMat));



}


/*-----------------------------------------------------------------------------
Light functions
-----------------------------------------------------------------------------*/

void B3L_ResetLight(light_t* pLight) {
    B3L_CLR(pLight->state, LIGHT_TYPE_BIT); //parallel light
    B3L_VECT3_SET(pLight->lightVect, 0.0f, 1.0f, 0.0f);
    pLight->factor_0 = -0.2f; 
    pLight->factor_1 = 10.0f;

}

void B3L_SetLightType(render_t* pRender, lightType_e type) {
    if (parallelLight == type) {
        B3L_CLR(pRender->light.state, LIGHT_TYPE_BIT);
    }
    if (dotLight == type) {
        B3L_SET(pRender->light.state, LIGHT_TYPE_BIT);
    }
}

void B3L_SetLightVect(render_t* pRender, f32 x, f32 y, f32 z) {
    if (B3L_TEST(pRender->light.state, LIGHT_TYPE_BIT) == PARALLEL_LIGHT) {//parallel light
        //normalized the vector
        f32 invSqrt = FastInvertSqrt(x * x + y * y + z * z);
        x = x * invSqrt;
        y = y * invSqrt;
        z = z * invSqrt;
    }
    B3L_VECT3_SET(pRender->light.lightVect, x, y, z);
}



