#include "b3d.h"
#include <string.h>



static void AddObjToTwoWayList(B3LObj_t* pObj, B3LObj_t** pStart);

static void AddObjToTwoWayList(B3LObj_t* pObj, B3LObj_t** pStart) {

    pObj->next = *pStart;
    *pStart = pObj;
    pObj->privous = pObj;

    if (pObj->next != (B3LObj_t*)NULL) {
        pObj->next->privous = pObj;
    }
}


/*
Public functions
*/





void B3L_ResetObjList(scene_t* pScene,u32 objNum) {
    u32 i;
    pScene->pFreeObjs = pScene->pObjBuff;  //reset all the obj buffer
    pScene->pObjBuff[0].state = 0x00000000;
    pScene->pObjBuff[0].privous = pScene->pFreeObjs;
    pScene->pObjBuff[0].next = &(pScene->pObjBuff[1]);
    for (i = 1; i < objNum; i++) {
        pScene->pObjBuff[i].state = 0x00000000;
        pScene->pObjBuff[i].privous = &(pScene->pObjBuff[i - 1]);
        pScene->pObjBuff[i].next = &(pScene->pObjBuff[i + 1]);
    }
    pScene->pObjBuff[objNum - 1].next = (B3LObj_t*)NULL;
}




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

void B3L_AddObjToRenderList(B3LObj_t* pObj, render_t* pRender) {
    //get the statement
    u32 type = (pObj->state & OBJ_TYPE_MASK);
    //printf("type %d\n",type);
    if ((type == (1 << MESH_OBJ)) || (type == (1 << POLYGON_OBJ)) || (type == (1 << NOTEX_MESH_OBJ)) || (type == (1 << BITMAP_OBJ))|| (type == (1 << PARTICLE_GEN_OBJ))) {
        //printf("add\n");
        AddObjToTwoWayList(pObj, &(pRender->scene.pActiveObjs));
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
            pRender->scene.pActiveObjs = pObj->next;
            if (pObj->next != (B3LObj_t*)NULL) {
                pObj->next->privous = pRender->scene.pActiveObjs;
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
    

    if (B3L_TEST(pObj->state, OBJ_RAM_BUFF_RESOURCE_0)) {
        B3L_ChangeResourceReference(pObj->pResource0, -1);//decrease the reference count
    }    
    if (B3L_TEST(pObj->state, OBJ_RAM_BUFF_RESOURCE_1)) {
        B3L_ChangeResourceReference(pObj->pResource1, -1);//decrease the reference count
        
    }
    if (B3L_TEST((pObj->state), PARTICLE_GEN_OBJ)) {
        //empty the particle below to the generator
        u32 num = (pObj->state) >> PARTICLE_NUM_SHIFT;
        s32 i;
        for (i = 0; i < num; i++) {
            B3L_Particle_t* temp = (B3L_Particle_t*)(pObj->pResource0);
            B3L_PopParticleFromGenerator(pObj, temp);
            B3L_ReturnParticleToPool(temp, pRender);
        }
    }
    pObj->next = NULL;
    pObj->pMother = NULL;
    pObj->privous = NULL;
    pObj->pResource0 = NULL;
    pObj->pResource1 = NULL;
    pObj->state = 0x00000000;
    pRender->scene.freeObjNum += 1;
    AddObjToTwoWayList(pObj, &(pRender->scene.pFreeObjs));
}




B3LObj_t* B3L_CreatTexMeshObj(render_t* pRender, B3L_Mesh_t* pMesh, B3L_tex_t* pTexture,
    bool backfaceCulling, bool fix_render_level, u8 render_level, bool fix_light_value,
    u8 light_value, bool Add_To_RenderList, bool Buff_In_Ram, u16 Buff_priority) {
    B3LObj_t* pObj = B3L_GetFreeObj(pRender);
    if (pObj == (B3LObj_t*)NULL) {
        return pObj;
    }
    pObj->pMother = (B3LObj_t*)NULL;
    B3L_SET(pObj->state, MESH_OBJ);
    SET_OBJ_VISIABLE(pObj);
    if (Buff_In_Ram) {
        pObj->pResource0 = (void*)B3L_MeshBuffInRam(pRender, pMesh,Buff_priority);
        pObj->pResource1 = (void*)B3L_TexBuffInRam(pRender, pTexture, Buff_priority);
        if (pObj->pResource0 != pMesh) {//success buffered
            B3L_SET(pObj->state, OBJ_RAM_BUFF_RESOURCE_0);
            B3L_ChangeResourceReference(pObj->pResource0, 1);//add 1 to reference, if the resource is low priority 
                                                             //and reference == 0, it would be free by garbage collection
        }
        if (pObj->pResource1 != pTexture) {//success buffered
            B3L_SET(pObj->state, OBJ_RAM_BUFF_RESOURCE_1);
            B3L_ChangeResourceReference(pObj->pResource1, 1);
        }
    }
    else {//link to the obj in flash
        pObj->pResource0 = (void*)pMesh;
        pObj->pResource1 = (void*)pTexture;
    }

    if (backfaceCulling == true) {
        ENABLE_OBJ_BACKFACE_CULLING(pObj);
    }
    else {
        DISABLE_OBJ_BACKFACE_CULLING(pObj);
    }
    if (render_level > 3) {
        render_level = 3;
    }
    if (fix_render_level == true) {
        SET_OBJ_FIX_RENDER_LEVEL(pObj, render_level);
    }
    else {
        //clear render level bit
        B3L_CLR(pObj->state, OBJ_IGNORE_RENDER_LEVEL);
    }
    if (fix_light_value == true) {
        B3L_SET(pObj->state, OBJ_SPECIAL_LIGHT_VALUE);
        SET_OBJ_FIX_LIGHT_VALUE(pObj, light_value);
    }
    else {
        B3L_CLR(pObj->state, OBJ_SPECIAL_LIGHT_VALUE);
    }
    if (Add_To_RenderList == true) {
        B3L_AddObjToRenderList(pObj, pRender);
    }
    return pObj;
}
B3LObj_t* B3L_CreatColorMeshObj(render_t* pRender, B3L_Mesh_t* pMesh, B3L_tex_t* pColor,
    bool backfaceCulling, bool fix_render_level, u8 render_level,
    bool fix_light_value, u8 light_value, bool Add_To_RenderList,
    bool Buff_In_Ram, u16 Buff_priority) {
    B3LObj_t* pObj = B3L_GetFreeObj(pRender);
    if (pObj == (B3LObj_t*)NULL) {
        return pObj;
    }
    pObj->pMother = (B3LObj_t*)NULL;
    B3L_SET(pObj->state, NOTEX_MESH_OBJ);
    SET_OBJ_VISIABLE(pObj);
    if (Buff_In_Ram) {
        pObj->pResource0 = (void*)B3L_MeshBuffInRam(pRender, pMesh, Buff_priority);
        pObj->pResource1 = (void*)B3L_ColorBuffInRam(pRender, pColor,  Buff_priority);
        if (pObj->pResource0 != pMesh) {//success buffered
            B3L_SET(pObj->state, OBJ_RAM_BUFF_RESOURCE_0);
            B3L_ChangeResourceReference(pObj->pResource0, 1);
        }
        if (pObj->pResource1 != pColor) {//success buffered
            B3L_SET(pObj->state, OBJ_RAM_BUFF_RESOURCE_1);
            B3L_ChangeResourceReference(pObj->pResource1, 1);
        }
    }
    else {
        pObj->pResource0 = (void*)pMesh;
        pObj->pResource1 = (void*)pColor;

    }

    if (backfaceCulling == true) {
        ENABLE_OBJ_BACKFACE_CULLING(pObj);
    }
    else {
        DISABLE_OBJ_BACKFACE_CULLING(pObj);
    }
    if (render_level > 3) {
        render_level = 3;
    }
    if (fix_render_level == true) {
        SET_OBJ_FIX_RENDER_LEVEL(pObj, render_level);
    }
    else {
        //clear render level bit
        B3L_CLR(pObj->state, OBJ_IGNORE_RENDER_LEVEL);
    }
    if (fix_light_value == true) {
        B3L_SET(pObj->state, OBJ_SPECIAL_LIGHT_VALUE);
        SET_OBJ_FIX_LIGHT_VALUE(pObj, light_value);
    }
    else {
        B3L_CLR(pObj->state, OBJ_SPECIAL_LIGHT_VALUE);
    }
    if (Add_To_RenderList == true) {
        B3L_AddObjToRenderList(pObj, pRender);
    }
    return pObj;
}

B3LObj_t* B3L_CreatBitmapObj(render_t* pRender, B3L_tex_t* pTexture, u8 tu, u8 tv, u8 bu, u8 bv,
    u8 light_value, bool Add_To_RenderList, bool Buff_In_Ram, u16 Buff_priority) {

    B3LObj_t* pObj = B3L_GetFreeObj(pRender);
    if (pObj == (B3LObj_t*)NULL) {
        return pObj;
    }
    pObj->pMother = (B3LObj_t*)NULL;
    B3L_SET(pObj->state, BITMAP_OBJ);
    SET_OBJ_VISIABLE(pObj);
    if (Buff_In_Ram) {
        pObj->pResource0 = (void*)B3L_TexBuffInRam(pRender,pTexture,Buff_priority);
        if (pObj->pResource0 != pTexture) { //success buffered
            B3L_SET(pObj->state, OBJ_RAM_BUFF_RESOURCE_0);
            B3L_ChangeResourceReference(pObj->pResource0, 1);
        }
    }
    else {//link to the obj in flash
        pObj->pResource0 = (void*)pTexture;
    }
    u32 resource1 = tu | (tv << 8) | (bu << 16) | (bv << 24);
    pObj->pResource1 = (void*)resource1;
    SET_OBJ_FIX_LIGHT_VALUE(pObj, light_value);

    if (Add_To_RenderList == true) {
        B3L_AddObjToRenderList(pObj, pRender);
    }
    return pObj;
}

B3LObj_t* B3L_CreatParticleGenObj(render_t* pRender, particleGenerator_t* pGenFunc, bool Add_To_RenderList, bool active) {
    B3LObj_t* pObj = B3L_GetFreeObj(pRender);
    if (pObj == (B3LObj_t*)NULL) {
        return pObj;
    }
    pObj->pMother = (B3LObj_t*)NULL;
    B3L_SET(pObj->state, PARTICLE_GEN_OBJ);
    SET_OBJ_VISIABLE(pObj);
    pObj->pResource0 = NULL;
    pObj->pResource1 = (void*)pGenFunc;
    if (active == true) {
        B3L_SET(pObj->state, OBJ_PARTICLE_GEN_ACTIVE);
    }
    if (Add_To_RenderList == true) {
        B3L_AddObjToRenderList(pObj, pRender);
    }
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


//TODO
f32 B3L_ObjDistance(B3LObj_t* pA, B3LObj_t* pB) {


}