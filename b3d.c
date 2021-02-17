#include "b3d.h"



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

static void ResetObjList(scene_t* pScene);
static void GenerateW2CMatrix(camera_t* pCam);
static void  UpdateCam(render_t* pRender);
static void AddObjToTwoWayList(B3LObj_t *pObj, B3LObj_t **pStart);



void B3L_RenderInit(render_t* pRender, fBuff_t* pFrameBuff) {
#if B3L_ARM  == 1
  B3d_FPU_Init();
#endif
  pRender->pFrameBuff = pFrameBuff;
  pRender->pZBuff = zbuff;
  pRender->pVectBuff = vectBuff;
  pRender->lvl0Distance = LEVEL_0_DEFAULT_DISTANCE;
  pRender->lvl1Distance = LEVEL_1_DEFAULT_DISTANCE;
  pRender->lvl1Light = LEVEL_1_DEFAULT_LIGHT;
  pRender->farPlane = DEFAULT_FAR_PLANE;
  pRender->nearPlane = DEFAULT_NEAR_PLANE;
  B3L_ResetScene(&(pRender->scene));
  B3L_InitCamera(pRender);
  B3L_ResetLight(&(pRender->light));
}


void B3L_RenderScence(render_t* pRender) {
    ClearZbuff(pRender->pZBuff, Z_BUFF_LENGTH);
    ClearFbuff(pRender->pFrameBuff, F_BUFF_LENGTH,8);
    //from camera's rotation matrix to create world -> clip space matrix
    //printf("cleared\n");
    UpdateCam(pRender);
    //printf("camera updated\n");
    RenderObjs(pRender);
    //printf("objs drawed\n");
#ifdef B3L_USING_PARTICLE
    RenderParticleObjs(pRender);
#endif

}

void B3L_ResetScene(scene_t* pScene) {
    pScene->freeObjNum = OBJ_BUFF_SIZE; //reset free obj numbers
    pScene->pActiveMeshObjs = (B3LObj_t*)NULL; //reset active obj list
    ResetObjList(pScene);
#ifdef B3L_USING_PARTICLE
    pScene->pActiveParticleGenObjs = (B3LObj_t*)NULL;//reset particle generator obj list 
    pScene->freeParticleNum = B3L_PARTICLE_BUFF_DEPTH;   //reset particle numbers
    ResetParticleList(particleBuff, &(pScene->pfreeParticles), B3L_PARTICLE_BUFF_DEPTH); //reset particle list
    //call reset particle one-way list function
#endif
}

static void ResetObjList(scene_t* pScene) {
    u32 i;
    pScene->pFreeObjs = pScene->objBuff;  //reset all the obj buffer
    pScene->objBuff[0].state = 0x00000000;
    pScene->objBuff[0].privous = pScene->pFreeObjs;
    pScene->objBuff[0].next = &(pScene->objBuff[1]);
    for (i = 1; i < OBJ_BUFF_SIZE; i++) {
        pScene->objBuff[i].state = 0x00000000;
        pScene->objBuff[i].privous = &(pScene->objBuff[i - 1]);
        pScene->objBuff[i].next = &(pScene->objBuff[i + 1]);
    }
    pScene->objBuff[OBJ_BUFF_SIZE - 1].next = (B3LObj_t*)NULL;
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

u32 B3L_GetFreeObjNum(render_t* pRender) {
    return pRender->scene.freeObjNum;
}

B3LObj_t* B3L_GetFreeObj(render_t* pRender) {
    B3LObj_t* returnObj;
    if (pRender->scene.pFreeObjs != (B3LObj_t*)NULL) {
        pRender->scene.freeObjNum -= 1;
        returnObj = pRender->scene.pFreeObjs;
        pRender->scene.pFreeObjs = pRender->scene.pFreeObjs->next;
pRender->scene.pFreeObjs->privous = pRender->scene.pFreeObjs;
//isolate the returned obj 
returnObj->next = (B3LObj_t*)NULL;
returnObj->privous = (B3LObj_t*)NULL;
returnObj->state = 0;
B3L_VECT4_SET(returnObj->transform.quaternion, 0.0f, 0.0f, 0.0f, 1.0f);
B3L_VECT3_SET(returnObj->transform.scale, 1.0f, 1.0f, 1.0f);
B3L_VECT3_SET(returnObj->transform.translation, 0.0f, 0.0f, 0.0f);
//B3L_InitUnitMat3(&(returnObj->mat));
return returnObj;
    }
    else {
    return (B3LObj_t*)NULL;
    }
}

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


static void AddObjToTwoWayList(B3LObj_t* pObj, B3LObj_t** pStart) {

    pObj->next = *pStart;
    *pStart = pObj;
    pObj->privous = pObj;

    if (pObj->next != (B3LObj_t*)NULL) {
        pObj->next->privous = pObj;
    }
}

void B3L_AddObjToRenderList(B3LObj_t* pObj, render_t* pRender) {
    //get the statement
    u32 type = (pObj->state & OBJ_TYPE_MASK);
    //printf("type %d\n",type);
    if ((type == (1 << MESH_OBJ)) || (type == (1 << POLYGON_OBJ)) || (type == (1 << NOTEX_MESH_OBJ))) {
        //printf("add\n");
        AddObjToTwoWayList(pObj, &(pRender->scene.pActiveMeshObjs));
    }
#ifdef B3L_USING_PARTICLE
    if (type == (1 << PARTICLE_GEN_OBJ)) {
        AddObjToTwoWayList(pObj, &(pRender->scene.pActiveParticleGenObjs));
    }
#endif  
}

void B3L_PopObjFromRenderList(B3LObj_t* pObj, render_t* pRender) {
    if (pObj->privous != pObj) {
        if (pObj->next != (B3LObj_t*)NULL) {
            pObj->next->privous = pObj->privous;
        }
        pObj->privous->next = pObj->next;
    }
    else {
        u32 type = (pObj->state & OBJ_TYPE_MASK);
        if ((type == (1 << MESH_OBJ)) || (type == (1 << POLYGON_OBJ)) || (type == (1 << NOTEX_MESH_OBJ))) {
            pRender->scene.pActiveMeshObjs = pObj->next;
            if (pObj->next != (B3LObj_t*)NULL) {
                pObj->next->privous = pRender->scene.pActiveMeshObjs;
            }
        }
#ifdef B3L_USING_PARTICLE
        if (type == (1 << PARTICLE_GEN_OBJ)) {
            pRender->scene.pActiveParticleGenObjs = pObj->next;
            if (pObj->next != (B3LObj_t*)NULL) {
                pObj->next->privous = pRender->scene.pActiveParticleGenObjs;
            }
        }
#endif       
    }
    pObj->next = (B3LObj_t*)NULL;
    pObj->privous = (B3LObj_t*)NULL;
}

void B3L_ReturnObjToInactiveList(B3LObj_t* pObj, render_t* pRender) {
    if (pObj->privous != (B3LObj_t*)NULL) { //it is now in the active list
        B3L_PopObjFromRenderList(pObj, pRender);
    }
    //clean the obj statement
    pObj->state = 0x00000000;
    pRender->scene.freeObjNum += 1;
    AddObjToTwoWayList(pObj, &(pRender->scene.pFreeObjs));
}


B3LObj_t* B3L_CreatTexMeshObj(render_t* pRender, B3L_Mesh_t* pMesh, B3L_tex_t* pTexture,
    bool backfaceCulling, bool fix_render_level,u8 render_level, bool fix_light_value,
    u8 light_value,bool Add_To_RenderList) {
    B3LObj_t* pObj = B3L_GetFreeObj(pRender);
    if (pObj == (B3LObj_t*)NULL){
        return pObj;
    }
    B3L_SET(pObj->state, MESH_OBJ);
    SET_OBJ_VISIABLE(pObj);

    pObj->pResource0 = (void*)pMesh;
    pObj->pResource1 = (void*)pTexture;
    if (backfaceCulling==true){
        SET_OBJ_BACKFACE_CULLING(pObj);
    }
    else {
        SET_OBJ_NOT_BACKFACE_CULLING(pObj);
    }
    if (render_level > 3) {
        render_level = 3;
    }
    if (fix_render_level==true) {
        SET_OBJ_FIX_RENDER_LEVEL(pObj, render_level);
    }
    else {
        //clear render level bit
        B3L_CLR(pObj->state, OBJ_IGNORE_RENDER_LEVEL);
    }
    if (fix_light_value == true) {
        B3L_SET(pObj->state, OBJ_SPECIAL_LIGHT_VALUE);
        CHANGE_OBJ_FIX_LIGHT_VALUE(pObj,light_value);
    }
    else {
        B3L_CLR(pObj->state, OBJ_SPECIAL_LIGHT_VALUE);
    }
    if (Add_To_RenderList == true) {
        B3L_AddObjToRenderList(pObj, pRender);
    }
    return pObj;
}

void B3L_SetObjPosition(B3LObj_t* pObj, f32 x, f32 y, f32 z) {
    pObj->transform.translation.x = x;
    pObj->transform.translation.y = y;
    pObj->transform.translation.z = z;
}

void B3L_SetObjScale(B3LObj_t* pObj, f32 xScale, f32 yScale, f32 zScale) {
    pObj->transform.scale.x = xScale;
    pObj->transform.scale.y = yScale;
    pObj->transform.scale.z = zScale;
}
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
    B3L_InitUnitMat3(&(pCam->mat));
    B3L_VECT4_SET(pCam->transform.quaternion, 0.0f, 0.0f, 0.0f, 1.0f);
    pCam->pTrackObj = (B3LObj_t*)NULL;

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

static void GenerateW2CMatrix(camera_t* pCam) {
    mat3_t* pMat3 = &(pCam->mat);
    mat4_t* pW2CMat = &(pCam->camW2CMat);
    f32 x = -(pCam->transform.translation.x);
    f32 y = -(pCam->transform.translation.y);
    f32 z = -(pCam->transform.translation.z);
    //get the shift by translation
    f32 zero = 0.0f;
    pW2CMat->m33 = 1.0f; pW2CMat->m30 = zero; pW2CMat->m31 = zero; pW2CMat->m32 = zero;
    pW2CMat->m00 = pMat3->m00; pW2CMat->m01 = pMat3->m10; pW2CMat->m02 = pMat3->m20;
    pW2CMat->m10 = pMat3->m01; pW2CMat->m11 = pMat3->m11; pW2CMat->m12 = pMat3->m21;
    pW2CMat->m20 = pMat3->m02; pW2CMat->m21 = pMat3->m12; pW2CMat->m22 = pMat3->m22;
    pW2CMat->m03 = x * pW2CMat->m00 + y * pW2CMat->m01 + z * pW2CMat->m02;
    pW2CMat->m13 = x * pW2CMat->m10 + y * pW2CMat->m11 + z * pW2CMat->m12;
    pW2CMat->m23 = x * pW2CMat->m20 + y * pW2CMat->m21 + z * pW2CMat->m22;
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


void B3L_CameraLookAt(camera_t* pCam, vect3_t* pAt, vect3_t* pUp) {
    B3L_SET(pCam->state, CAM_NEED_MATRIX_UPDATE);
    B3L_CreateLookAtQuaternion(&(pCam->transform.translation),
        pAt, pUp, &(pCam->transform.quaternion));
}

void B3L_CamStopTrack(camera_t* pCam) {
    B3L_CLR(pCam->state, B3L_CAMERA_TRACK_OBJ_MODE);
}

void B3L_CamStartTrack(camera_t* pCam) {
    B3L_SET(pCam->state, B3L_CAMERA_TRACK_OBJ_MODE);
}


_RAM_FUNC void B3L_CamInitTrack(camera_t* pCam, B3LObj_t* pObj, f32 camX, f32 camY, f32 camZ, f32 lookAtX, f32 lookAtY, f32 lookAtZ) {
    //B3L_SET(pCam->state,B3L_CAMERA_TRACK_OBJ_MODE);
    pCam->pTrackObj = pObj;
    pCam->targetPosition.x = camX;
    pCam->targetPosition.y = camY;
    pCam->targetPosition.z = camZ;
    vect3_t from = { camX,camY,camZ };
    vect3_t at = { lookAtX,lookAtY,lookAtZ };
    vect3_t up = { 0.0f,1.0f,0.0f };
    B3L_CreateLookAtQuaternion(&from, &at, &up, &(pCam->targetQuat));
    //printf("target quat:");
    //B3L_logVec4(pCam->targetQuat);
}

_RAM_FUNC static void CamCalNewTrackPosition(camera_t* pCam) {
    B3LObj_t* pObj = pCam->pTrackObj;
    mat3_t mat;
    B3L_QuaternionToMatrix(&(pObj->transform.quaternion), &mat);
    vect3_t result;
    B3L_Vect3MulMat3(&(pCam->targetPosition), &mat, &result);

    B3L_Vect3Add(&result, &(pObj->transform.translation), &(pCam->transform.translation));
}

_RAM_FUNC static void CamCalNewTrackQuaternion(camera_t* pCam) {
    quat4_t targetQuat;
    B3LObj_t* pObj = pCam->pTrackObj;
    /*
    if (B3L_TEST(pObj->state, OBJ_NEED_QUAT_UPDATE)) {
        //update it's matrix and clear the flag
        B3L_MatrixToQuaternion(&(pObj->mat), &(pObj->transform.quaternion));
        B3L_CLR(pObj->state, OBJ_NEED_QUAT_UPDATE);
    }
    */
    B3L_QuatMult(&(pObj->transform.quaternion), &(pCam->targetQuat), &targetQuat);
    //now the targetQuat in world space
    f32 cosHalfAngel = B3L_QuatDot(&targetQuat, &(pCam->transform.quaternion));
    f32 t = (1.0f - B3L_Absf(cosHalfAngel)) * 8.0f;
    B3L_QuaternionInterp(&(pCam->transform.quaternion), &targetQuat, &(pCam->transform.quaternion), t);
    B3L_SET(pCam->state, CAM_NEED_MATRIX_UPDATE);
    //printf("new quat:");
    //B3L_logVec4(pCam->transform.quaternion);
}


static void  UpdateCam(render_t* pRender) {
    camera_t* pCam = &(pRender->camera);
    if (B3L_TEST(pCam->state, B3L_CAMERA_TRACK_OBJ_MODE)) {
        CamCalNewTrackPosition(pCam);
        CamCalNewTrackQuaternion(pCam);
        //printf("current camera position:");
        //B3L_logVec3(pCam->transform.translation);
    }
    if (B3L_TEST(pCam->state, CAM_NEED_MATRIX_UPDATE)) {
        B3L_QuaternionToMatrix(&(pCam->transform.quaternion), &(pCam->mat));
        B3L_CLR(pCam->state, CAM_NEED_MATRIX_UPDATE);
    }
    GenerateW2CMatrix(&(pRender->camera));
    B3L_Mat4XMat4(&(pCam->camW2CMat), &(pCam->clipMat), &(pCam->camW2CMat));
    //printf("clip mat:");
    //B3L_logMat4(pCam->clipMat);
    //printf("W2C mat:");
    //B3L_logMat4(pCam->camW2CMat);
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


