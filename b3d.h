#ifndef __B3D_H__
#define __B3D_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "b3d_types.h"
#include "b3d_config.h"
#include "b3d_draw.h"
#include "b3d_common.h"
#include "b3d_port.h"
#include "b3d_math.h"
#include "b3d_testObj.h"
#include "b3d_renderObj.h"
#include "heap_4.h"
#include "b3d_obj.h"
/*-----------------------------------------------------------------------------
Render functions
-----------------------------------------------------------------------------*/
extern  void B3L_RenderInit(render_t* pRender, fBuff_t* pFrameBuff, u32 objNum, u32 vectBuffSize,
                            f32 lv0Distance, f32 lv1Distance, s8 lv1DefaultLight, f32 farPlane, f32 nearPlane);
#define B3L_RenderInit_Simple(a,b)      B3L_RenderInit(a,b,OBJ_BUFF_SIZE,VECT_BUFF_SIZE,\
                                                LEVEL_0_DEFAULT_DISTANCE,LEVEL_1_DEFAULT_DISTANCE,\
                                                LEVEL_1_DEFAULT_LIGHT,DEFAULT_FAR_PLANE,DEFAULT_NEAR_PLANE)
extern  void B3L_RenderDeInit(render_t* pRender, bool resetBuff);
extern  void B3L_RenderScence(render_t* pRender, u32 time); //draw work 
extern  void B3L_ResetScene(scene_t* pScene, u32 freeObjNum); //reset all the scene resource
extern  void B3L_NewRenderStart(render_t* pRender, fBuff_t color); //clear buffs
extern  void B3L_Update(render_t* pRender, u32 time); //update particles etc


/*-----------------------------------------------------------------------------
Target rotate control functions
-----------------------------------------------------------------------------*/
extern void B3L_RotateObjInOX(quat4_t* pQuat, f32 angle);
extern void B3L_RotateObjInOY(quat4_t* pQuat, f32 angle);
extern void B3L_RotateObjInOZ(quat4_t* pQuat, f32 angle);
extern void B3L_RotateObjInWX(quat4_t* pQuat, f32 angle);
extern void B3L_RotateObjInWY(quat4_t* pQuat, f32 angle);
extern void B3L_RotateObjInWZ(quat4_t* pQuat, f32 angle);
#define  SET_OBJ_ROTATE_BY_EULER(pObj,pEuler)          B3L_EulerToQuaternion(pEuler,&((pObj)->transform.quaternion);                                                                       
#define  ROTATE_IN_BODY_X(pObj,angle)                  B3L_RotateObjInOX(&((pObj)->transform.quaternion),angle);                                                                     
#define  ROTATE_IN_BODY_Y(pObj,angle)                  B3L_RotateObjInOY(&((pObj)->transform.quaternion),angle);                                                                       
#define  ROTATE_IN_BODY_Z(pObj,angle)                  B3L_RotateObjInOZ(&((pObj)->transform.quaternion),angle);                                                                        
#define  ROTATE_IN_WORLD_X(pObj,angle)                 B3L_RotateObjInWX(&((pObj)->transform.quaternion),angle);                                                                       
#define  ROTATE_IN_WORLD_Y(pObj,angle)                 B3L_RotateObjInWY(&((pObj)->transform.quaternion),angle);                                                                       
#define  ROTATE_IN_WORLD_Z(pObj,angle)                 B3L_RotateObjInWZ(&((pObj)->transform.quaternion),angle);
                                                                        
/*-----------------------------------------------------------------------------
Camera functions
-----------------------------------------------------------------------------*/
// you could use obj rotate macros for camera also
extern void B3L_InitCamera(render_t* pRender);
extern void B3L_SetOrthographicProject(render_t* pRender);
extern void B3L_SetPerspectiveProject(render_t* pRender);//default mode
//call after you reset the aspect ratio, focus length, near/far plane, project mode 
extern void B3L_UpdateClipMatrix(render_t* pRender);
extern void B3L_CamSetFocusLengthByFOV(render_t* pRender, f32 fov);
extern void B3L_CameraMoveTo(render_t* pRender, f32 x,f32 y,f32 z);
extern void B3L_CameraMoveToV(render_t* pRender,vect3_t position);
extern void B3L_CameraLookAt(camera_t* pCam, vect3_t* pAt);

/*-----------------------------------------------------------------------------
Light functions
-----------------------------------------------------------------------------*/
extern void B3L_ResetLight(light_t* pLight);
extern void B3L_SetLightType(render_t* pRender, lightType_e type);
extern void B3L_SetLightVect(render_t* pRender, f32 x, f32 y, f32 z);

/*-----------------------------------------------------------------------------
Particle functions
-----------------------------------------------------------------------------*/

/*
#ifdef B3L_USING_PARTICLE
extern B3L_Particle_t* B3L_GetFreeParticle(scene_t* pScene);
extern u32  B3L_GetFreeParticleNum(render_t* pRender);
extern void B3L_ReturnParticleToPool(B3L_Particle_t* pParticle, scene_t* pScene);
extern void B3L_AddParticleToGenerator(B3L_Particle_t* pParticle, B3LParticleGenObj_t* pGenerator);
extern void B3L_UpdateAllParticlesStatesInGen(render_t* pRender, B3LParticleGenObj_t* pGen,
    u32 deltaTime, vect3_t* pForce);
#define B3L_SET_PARTICLE_POSITION(pP,px,py,pz)   pP->position.x=px;pP->position.y=py;pP->position.z=pz                                                                                                
#define B3L_SET_PARTICLE_DELTA(pP,dx,dy,dz)      pP->delta.x=dx;pP->delta.y=dy;pP->delta.z=dz                                                                                                
#endif  //end of  B3L_USING_PARTICLE
*/
#ifdef __cplusplus
}
#endif
#endif

