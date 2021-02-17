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


/*-----------------------------------------------------------------------------
Render functions
-----------------------------------------------------------------------------*/
extern  void B3L_RenderInit(render_t* pRender, fBuff_t* pFrameBuff);
extern  void B3L_RenderScence(render_t* pRender); //draw work 
extern  void B3L_ResetScene(scene_t* pScene); //reset all the scene resource
extern  void B3L_NewRenderStart(render_t* pRender, fBuff_t color); //clear buffs
extern  void B3L_Update(render_t* pRender, u32 time); //update particles etc

/*-----------------------------------------------------------------------------
Render obj functions
-----------------------------------------------------------------------------*/
extern u32 B3L_GetFreeObjNum(render_t* pRender);
extern B3LObj_t* B3L_GetFreeObj(render_t* pRender);
extern B3LObj_t* B3L_CreatTexMeshObj(render_t* pRender, B3L_Mesh_t* pMesh, B3L_tex_t* pTexture,
                                     bool backfaceCulling, bool fix_render_level, u8 render_level, 
                                     bool fix_light_value, u8 light_value, bool Add_To_RenderList);
extern B3LObj_t* B3L_CreatColorMeshObj(render_t* pRender, B3L_Mesh_t* pMesh, B3L_tex_t* pColor,
                                        bool backfaceCulling, bool fix_render_level, u8 render_level,
                                        bool fix_light_value, u8 light_value, bool Add_To_RenderList);

#define B3L_CreatTexMeshObj_Simple(pRnd,m,t)      B3L_CreatTexMeshObj((pRnd),(B3L_Mesh_t*)(m),(B3L_tex_t*)(t),true,false,0,false,0,true)
#define B3L_CreatColorMeshObj_Simple(pRnd,m,t)      B3L_CreatColorMeshObj((pRnd),(B3L_Mesh_t*)(m),(B3L_tex_t*)(t),true,false,0,false,0,true)
extern void B3L_SetObjPosition(B3LObj_t* pObj, f32 x, f32 y, f32 z);
extern void B3L_SetObjScale(B3LObj_t* pObj, f32 xScale, f32 yScale, f32 zScale);
/*
#ifdef B3L_USING_PARTICLE
extern B3LParticleGenObj_t* B3L_GetFreeParticleGeneratorObj(render_t* pRender);
#endif
*/
extern void B3L_AddObjToRenderList(B3LObj_t* pObj, render_t* pRender);
extern void B3L_PopObjFromRenderList(B3LObj_t* pObj, render_t* pRender);
extern void B3L_ReturnObjToInactiveList(B3LObj_t* pObj, render_t* pRender);

#define  SET_OBJ_VISIABLE(pObj)           B3L_SET((pObj)->state,OBJ_VISUALIZABLE)
#define  SET_OBJ_INVISIABLE(pObj)         B3L_CLR((pObj)->state,OBJ_VISUALIZABLE)
#define  SET_OBJ_BACKFACE_CULLING(pObj)         B3L_SET((pObj)->state,OBJ_BACKFACE_CULLING)
#define  SET_OBJ_NOT_BACKFACE_CULLING(pObj)     B3L_CLR((pObj)->state,OBJ_BACKFACE_CULLING)
#define  SET_OBJ_FIX_RENDER_LEVEL(pObj,n)      B3L_SET((pObj)->state,OBJ_IGNORE_RENDER_LEVEL);\
                                          ((pObj)->state=((pObj)->state&(~OBJ_RENDER_LEVEL_MASK))|((n)<<OBJ_FIX_RENDER_LEVEL_SHIFT))
#define  SET_OBJ_FIX_LIGHT_FACT(pObj,m)        B3L_SET((pObj)->state,OBJ_IGNORE_RENDER_LEVEL);\
                                          B3L_SET((pObj)->state,OBJ_SPECIAL_LIGHT_VALUE);\
                                          ((pObj)->state=((pObj)->state&(~OBJ_RENDER_LEVEL_MASK))|((1)<<OBJ_FIX_RENDER_LEVEL_SHIFT));\
                                          ((pObj)->state=((pObj)->state&(~OBJ_SPECIAL_LIGHT_MASK))|((m)<<OBJ_SPECIAL_LIGHT_SHIFT))
#define  CHANGE_OBJ_FIX_LIGHT_VALUE(pObj,m)    ((pObj)->state=((pObj)->state&(~OBJ_SPECIAL_LIGHT_MASK))|((m)<<OBJ_SPECIAL_LIGHT_SHIFT))

#define  SET_PARTICLE_GENERATOR_ACTIVE(pObj)    B3L_SET((pObj)->state,OBJ_PARTICLE_ACTIVE)
#define  SET_PARTICLE_GENERATOR_INACTIVE(pObj)  B3L_CLR((pObj)->state,OBJ_PARTICLE_ACTIVE)
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
extern void B3L_CameraLookAt(camera_t* pCam, vect3_t* pAt, vect3_t* pUp);
extern void B3L_CamStopTrack(camera_t* pCam);
extern void B3L_CamStartTrack(camera_t* pCam);
extern void B3L_CamInitTrack(camera_t* pCam, B3LObj_t* pObj, f32 camX, f32 camY, f32 camZ, f32 lookAtX, f32 lookAtY, f32 lookAtZ);
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
