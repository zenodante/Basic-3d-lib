#ifndef __B3D_TYPES_H__
#define __B3D_TYPES_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
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
  typedef uint8_t B3L_tex_t;

#define TEXTURE_HEAD_SHIFT        6
#define MESH_HEAD_SHIFT           12

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

  /*
  typedef struct {
    f32*      pVect;
    u8*       pLine;
  }B3L_Polygon_t;
  */
  typedef u32 B3L_Polygon_t;

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
    struct B3L_PARTICLE* next; //4
    struct B3L_PARTICLE* prev; //4
    uint32_t            state; //4
    int32_t             life;  //4
    vect3_t             position;  //12
    vect3_t             delta; //12
  }B3L_Particle_t;//40byte

  /*
  B3LObj_t state
     31     2423     1615      87
     ------------------------------------
  31|$$$$$$$$|******RQ|*ONMLKJI|***EDCBA|0
     ------------------------------------
    A-- mesh obj with texture
    B-- polygon obj
    C-- mesh obj without texture
    D-- particle generator obj
    E-- Bitmap obj
    I-- obj visualization
    J-- Particle generator active state
    K-- Back face culling state
    L-- fix render level switch
    M-- special light value
    N-- Ram buffered resource 0
    O-- Ram buffered resource 1
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
#define OBJ_PARTICLE_GEN_ACTIVE             (9)
#define OBJ_BACKFACE_CULLING               (10)
#define OBJ_IGNORE_RENDER_LEVEL            (11)
#define OBJ_SPECIAL_LIGHT_VALUE            (12)
#define OBJ_RAM_BUFF_RESOURCE_0            (13)
#define OBJ_RAM_BUFF_RESOURCE_1            (14)
//render stage information
#define OBJ_RENDER_LEVEL_MASK        0x00030000
#define OBJ_FIX_RENDER_LEVEL_SHIFT   (16)

#define OBJ_SPECIAL_LIGHT_MASK       0xFF000000
#define OBJ_SPECIAL_LIGHT_SHIFT      (24)
#define PARTICLE_NUM_MASK            0xFFF00000
#define PARTICLE_NUM_SHIFT           (20)

//all different obj types's size is <= sizeof(B3LObj_t)
  typedef struct B3LOBJ {
    struct B3LOBJ*        privous;//4
    struct B3LOBJ*        next;//4
    struct B3LOBJ*        pMother;//4
    u32                   state;//4
    transform3D_t         transform;//40
    void*                 pResource0;//4
    void*                 pResource1;//4  
  }B3LObj_t;
  //mesh texture obj      resource0: mesh, resource1: texture
  //mesh color obj        resource0: mesh, resource1: colors
  //bitmap obj            resource0: texture resource1: top left u, topleft v, bottom right u, bottom right v
  //polygon obj           resource0: line struct  resource1: colors
  //particle generator    resource0: active particles entry resource1: pointer to particle generator controlling block str
 




  typedef struct {
    //B3LObj_t            objBuff[OBJ_BUFF_SIZE];
    B3LObj_t*           pObjBuff;
    u32                 freeObjNum;
    B3LObj_t*           pFreeObjs;
    B3LObj_t*           pActiveObjs;
    B3L_Particle_t*     pParticleBuff;
    u32                 freeParticleNum;
    B3L_Particle_t*     pfreeParticles;
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

          
  typedef struct {
    B3LObj_t*           pMother;
    u32                 state;
    transform3D_t       transform;
    mat4_t              clipMat;
    mat4_t              camW2CMat;
    f32                 aspectRate;
    f32                 focalLength;
  }camera_t;

  typedef enum {
      B3L_DATA_OTHER_E = 0,
      B3L_DATA_MESH_E = 1,
      B3L_DATA_TEX_E = 2,
      B3L_DATA_COLOR_E = 3,
      B3L_DATA_POLYGON_E = 4,
}dataType_e;

  typedef struct A_BLOCK_LINK
  {
      struct A_BLOCK_LINK* pxNextFreeBlock;	/*<< The next free block in the list. */
      size_t xBlockSize;						/*<< The size of the free block. */
      u32    dataType;
      u16    priority;
      s16    refCount;
  } BlockLink_t;

  typedef struct {
    fBuff_t*            pFrameBuff;
    zBuff_t*            pZBuff;
    vect4_t*            pVectBuff;
    BlockLink_t*        pBuffResouce;
    camera_t            camera;
    light_t             light;
    scene_t             scene;
    f32                 lvl0Distance;
    f32                 lvl1Distance;
    f32                 farPlane;
    f32                 nearPlane;
    s8                  lvl1Light;
  }render_t;


  typedef struct {
      void (*pParticleGenFunc)(render_t*, B3LObj_t*, mat4_t*, u32);
      void (*pParticleUpdateFunc)(render_t*, B3LObj_t*, mat4_t*, u32);
  }particleGenerator_t;

#define B3L_MEM_HIGH_PRIORITY  0xFFFF
#define B3L_MEM_LOW_PRIORITY   0x0000

  /* Define the linked list structure.  This is used to link free blocks in order
of their memory address. */

#ifdef __cplusplus
}
#endif

#endif
