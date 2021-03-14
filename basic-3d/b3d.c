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


static void  UpdateCam(render_t* pRender, euler3_t* pCamEuler);
static void  B3L_AddResouceBuffToPool(render_t* pRender, void* pResource);



void B3L_RenderInit(render_t* pRender, fBuff_t* pFrameBuff,u32 objNum, u32 vectBuffSize,u32 particleNum,
                    f32 lv0Distance, f32 lv1Distance, s8 lv1DefaultLight, f32 farPlane,f32 nearPlane,
                    fBuff_t defaultColor, pSkyboxFunc SkyBoxFunc, B3L_tex_t* pSkyBoxTile,B3L_tex_t* pSkyBoxMap,bool ramBuffSkyBoxMap,bool ramBuffSkyBoxTile) {
#if B3L_ARM  == 1
  B3d_FPU_Init();
#endif
  pRender->pBuffResouce = NULL;
  pRender->pFrameBuff = pFrameBuff;
  pRender->pZBuff = zbuff; 

  

  pRender->scene.pObjBuff = (B3LObj_t *)pvPortMalloc(sizeof(B3LObj_t) * objNum, B3L_DATA_OTHER_E, B3L_MEM_HIGH_PRIORITY);
  if (particleNum != 0) {
      pRender->scene.pParticleBuff = (B3L_Particle_t*)pvPortMalloc(sizeof(B3L_Particle_t) * particleNum, B3L_DATA_OTHER_E, B3L_MEM_HIGH_PRIORITY);
  }
  else {
      pRender->scene.pParticleBuff = NULL;
  }
  pRender->pVectBuff = (vect4_t *)pvPortMalloc(sizeof(vect4_t)* vectBuffSize,B3L_DATA_OTHER_E, B3L_MEM_HIGH_PRIORITY);
  pRender->lvl0Distance = lv0Distance;
  pRender->lvl1Distance = lv1Distance;
  pRender->lvl1Light = lv1DefaultLight;
  pRender->farPlane = farPlane;
  pRender->nearPlane = nearPlane;
  B3L_ResetScene(&(pRender->scene),objNum, particleNum);
  B3L_InitCamera(pRender);
  B3L_ResetLight(&(pRender->light));
  B3L_SetSkyBox(pRender, defaultColor, SkyBoxFunc, pSkyBoxTile, pSkyBoxMap, ramBuffSkyBoxMap, ramBuffSkyBoxTile);
  //B3L_SetPerspectiveProject(pRender, DEFAULT_FOCUS_LENGTH);
}


void B3L_RenderDeInit(render_t* pRender) {

    
    pRender->pFrameBuff = NULL;
    pRender->pZBuff = NULL;
    vPortFree(pRender->scene.pObjBuff);//release memory
    pRender->scene.pObjBuff = NULL;
    vPortFree(pRender->pVectBuff);//release memory
    pRender->pVectBuff = NULL;
    if (pRender->scene.pParticleBuff != NULL) {
        vPortFree(pRender->scene.pParticleBuff);
        pRender->scene.pParticleBuff = NULL;
    }
    if (pRender->scene.ramBuffSkyBoxMap == true) {
        vPortFree(pRender->scene.pSkyBoxMap);  
    }
    if (pRender->scene.ramBuffSkyBoxTile == true) {
        vPortFree(pRender->scene.pSkyBoxTile);
    }
    pRender->scene.ramBuffSkyBoxTile == false;
    pRender->scene.ramBuffSkyBoxMap == false;

    pRender->scene.pSkyBoxMap = NULL;
    pRender->scene.pSkyBoxTile = NULL;
    pRender->scene.defaultColor = 0;
    pRender->pSkyboxFunc = NULL;

    pRender->lvl0Distance = 0.0f;
    pRender->lvl1Distance = 0.0f;
    pRender->lvl1Light = 0;
    pRender->farPlane = 1.0f;
    pRender->nearPlane = 100.0f;
    
    prvHeapInit();
    pRender->pBuffResouce = NULL;
    
}

void B3L_RenderScence(render_t* pRender,u32 time) {
    euler3_t camEuler;
    ClearZbuff(pRender->pZBuff, Z_BUFF_LENGTH);
    UpdateCam(pRender,&camEuler);
    //call the skybox render function
    //ClearFbuff(pRender->pFrameBuff, F_BUFF_LENGTH,8);
    pRender->pSkyboxFunc(pRender, &camEuler);
    RenderObjs(pRender,time);
}

void B3L_ResetScene(scene_t* pScene,u32 freeObjNum,u32 freeParticleNum) {
    pScene->freeObjNum = freeObjNum; //reset free obj numbers
    pScene->pActiveObjs = (B3LObj_t*)NULL; //reset active obj list
    B3L_ResetObjList(pScene,freeObjNum);

   
    pScene->freeParticleNum = freeParticleNum;   //reset particle numbers
    if (freeParticleNum != 0) {
        B3L_ResetParticleList(pScene->pParticleBuff, &(pScene->pfreeParticles), freeParticleNum); //reset particle list
    }


}

void B3L_SetSkyBox(render_t* pRender, fBuff_t defaultColor, pSkyboxFunc SkyBoxFunc, B3L_tex_t* pSkyBoxTile,
    B3L_tex_t* pSkyBoxMap, bool ramBuffSkyBoxMap, bool ramBuffSkyBoxTile) {
    
    pRender->scene.defaultColor = defaultColor;
    pRender->pSkyboxFunc = SkyBoxFunc;


    if (ramBuffSkyBoxMap == true) {
        if (pSkyBoxMap != NULL) {
            pRender->scene.pSkyBoxMap = B3L_TexBuffInRam(pRender, pSkyBoxMap, B3L_MEM_HIGH_PRIORITY);
        }

        if ((pRender->scene.pSkyBoxMap) == pSkyBoxMap) {
            ramBuffSkyBoxMap = false;
        }
    }
    else {
        pRender->scene.pSkyBoxMap = pSkyBoxMap;
    }
    if (ramBuffSkyBoxTile == true) {
        if (pSkyBoxTile != NULL) {
            pRender->scene.pSkyBoxTile = B3L_TexBuffInRam(pRender, pSkyBoxTile, B3L_MEM_HIGH_PRIORITY);
        }

        if ((pRender->scene.pSkyBoxTile) == pSkyBoxTile) {
            ramBuffSkyBoxTile = false;
        }
    }
    else {
        pRender->scene.pSkyBoxTile = pSkyBoxTile;

    }

    pRender->scene.ramBuffSkyBoxMap = ramBuffSkyBoxMap;
    pRender->scene.ramBuffSkyBoxTile = ramBuffSkyBoxTile;
}






static void B3L_AddResouceBuffToPool(render_t* pRender, void* pResource) {
    BlockLink_t* nextBlock = pRender->pBuffResouce;
    BlockLink_t* currentBlock = (BlockLink_t*)((u8*)pResource - xHeapStructSize);
    pRender->pBuffResouce = currentBlock;
    currentBlock->pxNextFreeBlock = nextBlock;
    
}

void* B3L_FindResouceInBuff(render_t* pRender, void* pResource, dataType_e dType) {

    BlockLink_t* currentBlock = pRender->pBuffResouce;
    u32 resourceID = ((u32*)pResource)[0];
    u32 dataType;
    u32 ID;
    while (currentBlock != NULL) {
        dataType = currentBlock->dataType;
        ID = (((u32*)((u8*)currentBlock + xHeapStructSize)))[0];
        if ((dataType == dType) && (ID == resourceID)) {
            return (void*)((u8 *)currentBlock + xHeapStructSize);
        }
        currentBlock = currentBlock->pxNextFreeBlock;
    }
    return NULL;

}

void B3L_ChangeResourceReference(void *pResource, s32 num) {
    BlockLink_t* ctlBlock = (BlockLink_t*)((u8*)pResource - xHeapStructSize);
    ctlBlock->refCount += num;

}


void* B3L_MeshBuffInRam(render_t* pRender, B3L_Mesh_t* pMesh, u16 priority) {
    u32 size = B3L_GetMeshResouceSize(pMesh);//calculate the resource size in byte
    void* pResource;
    pResource = B3L_FindResouceInBuff(pRender, pMesh, B3L_DATA_MESH_E);//find if it already buffed in the heap
    if (pResource == NULL) {//not buffered
        pResource = (void*)pvPortMalloc(size, B3L_DATA_MESH_E, priority);//try to alloc memory in heap
        if (pResource != (void*)NULL) { //if get the memory
            B3L_AddResouceBuffToPool(pRender, pResource);//also add the resource in pool for future search
            memcpy((void*)(pResource), (const void*)pMesh, size); //copy the resource
        }
        else {
            pResource = (void*)pMesh; //fail to get the memory, just point to the original resource
        }
    }  
    return pResource;
}


void* B3L_PolygonBuffInRam(render_t* pRender, B3L_Polygon_t* pPoly, u16 priority) {
    u32 size = B3L_GetPolyResouceSize(pPoly);
    void* pResource;
    pResource = B3L_FindResouceInBuff(pRender, pPoly, B3L_DATA_POLYGON_E);
    if (pResource == NULL) {
        pResource = (void*)pvPortMalloc(size, B3L_DATA_POLYGON_E, priority);
        if (pResource != (void*)NULL) {
            B3L_AddResouceBuffToPool(pRender, pResource);
            memcpy((void*)(pResource), (const void*)pPoly, size);
        }
        else {
            pResource = (void*)pPoly;
        }
    }
    return pResource;

}


void* B3L_TexBuffInRam(render_t* pRender, B3L_tex_t* pTexture, u16 priority) {
    u32 size = B3L_GetTexResouceSize(pTexture);
    void* pResource;
    pResource = B3L_FindResouceInBuff(pRender, pTexture, B3L_DATA_TEX_E);
    if (pResource == NULL) {
        pResource = (void*)pvPortMalloc(size, B3L_DATA_TEX_E, priority);

        if (pResource != (void*)NULL) {
            B3L_AddResouceBuffToPool(pRender, pResource);
            memcpy((void*)(pResource), (const void*)pTexture, size);
        }
        else {
            pResource = (void*)pTexture;
        }
    }
    return pResource;
}

void* B3L_ColorBuffInRam(render_t* pRender, B3L_tex_t* pColor, u16 priority) {
    u32 size = ((u16*)pColor)[2] + 6;
    void* pResource;
    pResource = B3L_FindResouceInBuff(pRender, pColor, B3L_DATA_COLOR_E);
    if (pResource == NULL) {
        pResource = (void*)pvPortMalloc(size, B3L_DATA_COLOR_E, priority);
        if (pResource != (void*)NULL) {
            B3L_AddResouceBuffToPool(pRender, pResource);
            memcpy((void*)(pResource), (const void*)pColor, size);
            
        }
        else {
            pResource = (void*)pColor;
        }
    }
    return pResource;
}

void B3L_GarbageCollection(render_t* pRender,u32 time) {
    static u32 oldTime = 0;
    if ((time - oldTime) > B3L_GARBAGE_COLLECTION_PERIOD) {
        oldTime = time;
        BlockLink_t* currentBlock = pRender->pBuffResouce;
        BlockLink_t* prvBlock = NULL;
        BlockLink_t* nxtBlock;
        while (currentBlock != NULL) {
            nxtBlock = currentBlock->pxNextFreeBlock;
           //recycle all the resource with 0 reference and low priority, high priority resource need to be free manually
            if ((currentBlock->refCount<=0)&&(currentBlock->priority == B3L_MEM_LOW_PRIORITY)) {
                //pop current block
                
                if (prvBlock == NULL) {//first one
                    pRender->pBuffResouce = nxtBlock;
                }
                else {
                    prvBlock->pxNextFreeBlock = nxtBlock;
                }
                currentBlock->pxNextFreeBlock = NULL;
                vPortFree((void*)((u8*)currentBlock + xHeapStructSize));
                currentBlock = nxtBlock;
            }
            else {
                prvBlock = currentBlock;
                currentBlock = nxtBlock;
            }
        
        }

    }
    
}

/*
void B3L_NewRenderStart(render_t* pRender, fBuff_t color) {

    ClearFbuff(pRender->pFrameBuff, F_BUFF_LENGTH,color );
    //ClearFrameBuff(pRender->pFrameBuff,0xFF003423,Z_BUFF_LENTH);//need to add shift
    ClearZbuff(pRender->pZBuff, Z_BUFF_LENGTH);//need to add shift

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

void B3L_InitCamera(render_t* pRender) {
    camera_t* pCam = &(pRender->camera);
    pCam->aspectRate = DEFAULT_ASPECT_RATIO;
    pCam->focalLength = DEFAULT_FOCUS_LENGTH;
    B3L_VECT3_SET(pCam->transform.scale, 1.0f, 1.0f, 1.0f);
    B3L_VECT3_SET(pCam->transform.translation, 0.0f, 0.0f, 0.0f);
    B3L_VECT4_SET(pCam->transform.quaternion, 0.0f, 0.0f, 0.0f, 1.0f);
    pCam->pMother = (B3LObj_t*)NULL;
    pCam->state = 0; //default is PERSPECTIVE_PROJECT
    B3L_SET(pCam->state, PERSPECTIVE_PROJECT);
    MakeClipMatrix(pCam->state, pRender->nearPlane, pRender->farPlane, pCam->focalLength, pCam->aspectRate, &(pCam->clipMat));
    //printf("after init clip matrix:\n");
    //B3L_logMat4(pCam->clipMat);

}

void B3L_SetOrthographicProject(render_t* pRender, f32 newZoom) {
    pRender->camera.focalLength = newZoom;
    pRender->camera.state &= PROJECT_MASK;
    B3L_SET(pRender->camera.state, OTHROGRAPHIC_PROJECT);
    B3L_UpdateClipMatrix(pRender);
}

void B3L_SetPerspectiveProject(render_t* pRender,f32 newZoom) {
   // pRender->camera.focalLength = newZoom;
    pRender->camera.state =0;
    B3L_SET(pRender->camera.state, PERSPECTIVE_PROJECT);
    B3L_UpdateClipMatrix(pRender);
}

void B3L_UpdateClipMatrix(render_t* pRender) {
    camera_t* pCam = &(pRender->camera);
    MakeClipMatrix(pCam->state, pRender->nearPlane, pRender->farPlane, pCam->focalLength, pCam->aspectRate, &(pCam->clipMat));
}

void B3L_CamSetFocusLengthByFOV(render_t* pRender, f32 fov) {
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

static void  UpdateCam(render_t* pRender, euler3_t* pCamEuler) {
    camera_t* pCam = &(pRender->camera);
    mat3_t rotateMat;
    mat4_t o2wMat;
    B3L_CreateO2WChainMatrixForCam(pCam, &o2wMat);
    vect3_t camPosition;
    camPosition.x = -o2wMat.m03;
    camPosition.y = -o2wMat.m13;
    camPosition.z = -o2wMat.m23;

    B3L_CreateO2WChainMatrixOnlyRotationForCam(pCam, &rotateMat);
    B3L_MatrixToEuler(&rotateMat, pCamEuler);
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



