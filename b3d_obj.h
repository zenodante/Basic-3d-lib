#ifndef __B3D_OBJ_H__
#define __B3D_OBJ_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "b3d_types.h"
#include "b3d_config.h"
#include "b3d_draw.h"
#include "b3d_common.h"
//#include "b3d_port.h"
#include "b3d_math.h"
//#include "b3d_testObj.h"
#include "b3d_renderObj.h"
//#include "heap_4.h"
//#include "b3d.h"


/*-----------------------------------------------------------------------------
Process obj functions
-----------------------------------------------------------------------------*/
extern f32 B3L_ObjDistance(B3LObj_t* pA, B3LObj_t* pB);

/*-----------------------------------------------------------------------------
Render obj functions
-----------------------------------------------------------------------------*/
extern void B3L_ResetObjList(scene_t* pScene,u32 objNum);
extern u32 B3L_GetFreeObjNum(render_t* pRender);
extern B3LObj_t* B3L_GetFreeObj(render_t* pRender);
extern B3LObj_t* B3L_CreatTexMeshObj(render_t* pRender, B3L_Mesh_t* pMesh, B3L_tex_t* pTexture,
        bool backfaceCulling, bool fix_render_level, u8 render_level,
        bool fix_light_value, u8 light_value, bool Add_To_RenderList, bool Buff_In_Ram, u16 Buff_priority);
extern B3LObj_t* B3L_CreatColorMeshObj(render_t* pRender, B3L_Mesh_t* pMesh, B3L_tex_t* pColor,
        bool backfaceCulling, bool fix_render_level, u8 render_level,
        bool fix_light_value, u8 light_value, bool Add_To_RenderList, bool Buff_In_Ram, u16 Buff_priority);
extern B3LObj_t* B3L_CreatBitmapObj(render_t* pRender, B3L_tex_t* pTexture, u8 tu, u8 tv, u8 bu, u8 bv,
        u8 light_value, bool Add_To_RenderList, bool Buff_In_Ram, u16 Buff_priority);

extern B3LObj_t* B3L_CreatParticleGenObj(render_t* pRender, particleGenerator_t *pGenFunc, bool Add_To_RenderList, bool active);

#define B3L_CreatTexMeshObj_Simple(pRnd,m,t)      B3L_CreatTexMeshObj((pRnd),(B3L_Mesh_t*)(m),(B3L_tex_t*)(t),true,false,0,false,0,true,true,B3L_MEM_LOW_PRIORITY )
#define B3L_CreatColorMeshObj_Simple(pRnd,m,t)      B3L_CreatColorMeshObj((pRnd),(B3L_Mesh_t*)(m),(B3L_tex_t*)(t),true,false,0,false,0,true,true,B3L_MEM_LOW_PRIORITY )
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

#define  SET_OBJ_VISIABLE(pObj)                B3L_SET((pObj)->state,OBJ_VISUALIZABLE)
#define  SET_OBJ_INVISIABLE(pObj)              B3L_CLR((pObj)->state,OBJ_VISUALIZABLE)
#define  ENABLE_OBJ_BACKFACE_CULLING(pObj)     B3L_SET((pObj)->state,OBJ_BACKFACE_CULLING)
#define  DISABLE_OBJ_BACKFACE_CULLING(pObj)    B3L_CLR((pObj)->state,OBJ_BACKFACE_CULLING)
#define  SET_OBJ_FIX_RENDER_LEVEL(pObj,n)      B3L_SET((pObj)->state,OBJ_IGNORE_RENDER_LEVEL);\
                                               ((pObj)->state=((pObj)->state&(~OBJ_RENDER_LEVEL_MASK))|((n)<<OBJ_FIX_RENDER_LEVEL_SHIFT))
    //fix light value only useful to render level 1
    //to make the obj always has the fix light value, you need to first se the obj fix render level to 1
#define  SET_OBJ_FIX_LIGHT_VALUE(pObj,m)       u32 temp=SatToU8(m+16);\
                                               ((pObj)->state=((pObj)->state&(~OBJ_SPECIAL_LIGHT_MASK))|((temp)<<OBJ_SPECIAL_LIGHT_SHIFT))
#define  GET_OBJ_FIX_LIGHT_VALUE(pObj)         (((s8)(((pObj)->state&OBJ_SPECIAL_LIGHT_MASK)>>OBJ_SPECIAL_LIGHT_SHIFT))-16)

#define  SET_PARTICLE_GENERATOR_ACTIVE(pObj)    B3L_SET((pObj)->state,OBJ_PARTICLE_GEN_ACTIVE)
#define  SET_PARTICLE_GENERATOR_INACTIVE(pObj)  B3L_CLR((pObj)->state,OBJ_PARTICLE_GEN_ACTIVE)




#ifdef __cplusplus
}
#endif
#endif

