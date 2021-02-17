#ifndef __B3D_TYPES_H__
#define __B3D_TYPES_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include "b3d_config.h"
  /*Type defines---------------------------------------------------------------*/
  typedef float    f32;
  typedef int32_t  s32;
  typedef uint32_t u32;
  typedef uint16_t u16;
  typedef int16_t  s16;
  typedef uint8_t  u8;
  typedef int8_t   s8;
  typedef uint32_t q32;

  typedef uint8_t fBuff_t;
  typedef u16   zBuff_t;
  typedef uint8_t texture_t;


  typedef struct {
    f32                 x;
    f32                 y;
  }vect2_t;

  typedef struct {
    f32                 x;
    f32                 y;
    f32                 z;
  }vect3_t;

  /*
   31     2423     1615      87
   ------------------------------------
31|********|********|********|**xxBxxA|0
  ------------------------------------
A near plane clip state
B in clip space state

xx bits have been used for tri test
*/
//screen3_t is for 2d screen drawing step, it has same length as vect4_t
#define B3L_IN_SPACE             (3u)
#define B3L_NEAR_PLANE_CLIP      (0u)

  typedef struct {
    int32_t             x;
    int32_t             y;
    f32                 z;
    u32                 test;
  }screen4_t;

  typedef struct {
    f32                 x;
    f32                 y;
    f32                 z;
    u32                 test;
  }screen3f_t;

  typedef struct {
    f32                 x;
    f32                 y;
    f32                 z;
    f32                 w;
  }vect4_t;

  typedef struct {
    f32          x;
    f32          y;
    f32          z;
    f32          u;
    f32          v;
  }B3L_clip_t;

  typedef vect3_t euler3_t;
  typedef vect4_t quat4_t;

  //in column first order, mxy -- x is column num, y is the row number
  typedef struct {
    //row0  row1    row2    row3    
    f32 m00; f32 m01; f32 m02; f32 m03;//column 0
    f32 m10; f32 m11; f32 m12; f32 m13;//column 1
    f32 m20; f32 m21; f32 m22; f32 m23;//column 2
    f32 m30; f32 m31; f32 m32; f32 m33;//column 3
  }mat4_t;

  typedef struct {
    //row0  row1    row2    row3    
    f32 m00; f32 m01; f32 m02;//column 0
    f32 m10; f32 m11; f32 m12;//column 1
    f32 m20; f32 m21; f32 m22;//column 2
  }mat3_t;


  typedef struct {
    quat4_t       quaternion;
    vect3_t       scale;
    vect3_t       translation;
  }transform3D_t;


  typedef struct {
    f32*      pVect;
    u8*       pLine;
  }B3L_Polygon_t;


  /*
  2byte : vect number
  2byte : tri number
  24byte :boundbox
  12* vect number   vect;
  12* tri number    normal;
  6* tri number    triIndx;
  1* tri number    colors;
  */
  typedef u32 B3L_Mesh_NoTex_t;
  /*
  2byte : vect number
  2byte : tri number
  24byte :boundbox
  12* vect number   vect;     start + 4 + 24
  6* tri number    triIndx;   start + 4 + 24 + vectNum*12
  6* tri number    uv;        start + 4 + 24 + vectNum*12 + triNum*6
  12* tri number    normal;   start + 4 + 24 + vectNum*12 + triNum*6 + triNum*6
  */
  typedef u32 B3L_Mesh_t;



  //36 byte for single particle
  typedef struct B3L_PARTICLE {
    struct B3L_PARTICLE* next;
    uint32_t            state;
    int32_t             life;
    vect3_t             position;
    vect3_t             delta;
  }B3L_Particle_t;

  /*
  B3LObj_t state
     31     2423     1615      87
     ------------------------------------
  31|$$$$$$$$|******RQ|PONML*JI|***EDCBA|0
    ------------------------------------
    A-- mesh obj with texture
    B-- polygon obj
    C-- mesh obj without texture
    D-- particle generator obj
    E-- Bitmap obj
    I-- obj visualization
    J-- Back face culling state

    L-- fix render level switch
    M-- Particle generator acitve state
    //N-- need update euler angle
    //O-- need update matrix
    P-- special light value
    QR-- fix render level number
    $-- the 8 bit special light value defined by P + 16

  */
#define OBJ_TYPE_MASK                0x000000FF
#define MESH_OBJ                            (0)
#define POLYGON_OBJ                         (1)
#define NOTEX_MESH_OBJ                      (2)
#define PARTICLE_GEN_OBJ                    (3)
#define BITMAP_OBJ                          (4)
  //obj visualizable control
#define OBJ_VISUALIZABLE                    (8)
#define OBJ_BACKFACE_CULLING                (9)

#define OBJ_IGNORE_RENDER_LEVEL            (11)
#define OBJ_PARTICLE_ACTIVE                (12)
#define OBJ_NEED_QUAT_UPDATE               (13)
#define OBJ_NEED_MATRIX_UPDATE             (14)
#define OBJ_SPECIAL_LIGHT_VALUE            (15)
//render stage information
#define OBJ_RENDER_LEVEL_MASK        0x00030000
#define OBJ_FIX_RENDER_LEVEL_SHIFT   (16)

#define OBJ_SPECIAL_LIGHT_MASK       0xFF000000
#define OBJ_SPECIAL_LIGHT_SHIFT      (24)

//all different obj types's size is <= sizeof(B3LObj_t)
  typedef struct B3LOBJ {
    struct B3LOBJ*      privous;//4
    struct B3LOBJ*      next;//4
    u32                 state;//4
    transform3D_t       transform;//40
    //f32                 boundBox[6];//24
    void*                 pResource0;//4
    void*                 pResource1;//4  
  }B3LObj_t;//88bytes


  typedef struct {
    B3LObj_t            objBuff[OBJ_BUFF_SIZE];
    u32                 freeObjNum;
    B3LObj_t*           pFreeObjs;
    B3LObj_t*           pActiveMeshObjs;
#ifdef B3L_USING_PARTICLE
    B3LObj_t*     pActiveParticleGenObjs;
    u32           freeParticleNum;
    B3L_Particle_t* pfreeParticles;
#endif
  }scene_t;

  //state
  typedef enum {
    parallelLight = 0,
    dotLight = 1,
  }lightType_e;
  /*
light_t state
   31     2423     1615      87
   ------------------------------------
31|        |        |        |       A|0
  ------------------------------------
A-- 1, point light
    0, parallel light
*/
#define LIGHT_TYPE_BIT         (0)
#define POINT_LIGHT         (1u<<LIGHT_TYPE_BIT)
#define PARALLEL_LIGHT      (0u<<LIGHT_TYPE_BIT)
  typedef struct {
    u32                 state;
    vect3_t             lightVect;
    f32                 factor_0;
    f32                 factor_1;
  }light_t;


  /*
camera_t state
   31     2423     1615      87
   ------------------------------------
31|********|********|*ON*****|******BA|0
  ------------------------------------

A-- camera track obj mode
B-- Project mode
N-- need update euler angle
O-- need update matrix
*/
#define  B3L_CAMERA_TRACK_OBJ_MODE           (0)
#define  B3L_PROJECT_MODE                    (1)
#define  OTHROGRAPHIC_PROJECT                (1u<<B3L_PROJECT_MODE)
#define  PERSPECTIVE_PROJECT                 (0u<<B3L_PROJECT_MODE)
//bit 24
#define  CAM_NEED_QUAT_UPDATE                OBJ_NEED_QUAT_UPDATE            
//bit 25
#define  CAM_NEED_MATRIX_UPDATE              OBJ_NEED_MATRIX_UPDATE
  typedef struct {
    u32                 state;
    f32                 aspectRate;
    f32                 focalLength;
    mat3_t              mat;
    transform3D_t       transform;
    mat4_t              clipMat;
    mat4_t              camW2CMat;
    vect3_t             targetPosition;
    quat4_t             targetQuat;
    B3LObj_t*           pTrackObj;
  }camera_t;

  typedef struct {
    fBuff_t* pFrameBuff;
    zBuff_t* pZBuff;
    vect4_t* pVectBuff;
    camera_t            camera;
    light_t             light;
    scene_t             scene;
    f32                 lvl0Distance;
    f32                 lvl1Distance;
    f32                 farPlane;
    f32                 nearPlane;
    s8                  lvl1Light;
  }render_t;


  typedef struct PARTICLEGENOBJ {
    B3LObj_t* privous;//8  4
    B3LObj_t* next;//8   4
    u32                 state;//4   4
    mat3_t              mat;//36    36
    transform3D_t       transform;//40    40
    u32                 lastTime;//4   4
    u32                 particleNum;//4   4
    B3L_Particle_t* pParticleActive;//8   4   
    void      (*DrawFunc)(B3L_Particle_t*, screen4_t*, fBuff_t*, zBuff_t*);//8   4
    void      (*PtlUpdFunc)(u32, struct PARTICLEGENOBJ*, mat3_t*, vect3_t*, render_t*); //8    4  
    //time, self, obj->world matrix,free particle num pointer,free particle pool  
  }B3LParticleGenObj_t; //15 not common on ARM32,22 not common on WIN64

  typedef void (*B3L_DrawFunc_t)(B3L_Particle_t*, screen4_t*, fBuff_t*, zBuff_t*);


#ifdef __cplusplus
}
#endif

#endif
