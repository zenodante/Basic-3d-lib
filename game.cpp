#include "game.hpp"
#include <math.h>

#include "b3d.h"
#include "b3d_port.h"
#include "ship2.h"
using namespace blit;

render_t rnd;
B3LObj_t *pBox;
f32 fov = 1.0f;
//extern  zBuff_t zbuff[38400];
f32 px0 = 0.0f; f32  py0 = 0.0f; f32  px1 = 70.0f; f32  py1 = 0.0f; f32  px2 = 0.0f; f32  py2 = 70.0f;
f32 px3 = 70.0f; f32  py3 = 0.0f; f32  px4 = 70.0f; f32  py4 = 70.0f; f32  px5 = 0.0f; f32  py5 = 70.0f;
////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init() {
    set_screen_mode(ScreenMode::hires_palette);
    set_screen_palette((const Pen *)(pal), 256);
    B3L_RenderInit(&rnd,(fBuff_t *)(screen.data));
    u32 num = B3L_GetFreeObjNum(&rnd);
    //printf("%d\n",num);
    pBox = B3L_GetFreeObj(&rnd);
    //num = B3L_GetFreeObjNum(&rnd);
    //printf("%d\n",num);
    B3L_SET(pBox->state,MESH_OBJ); 
    B3L_VECT3_SET(pBox->transform.translation,0.0f,0.0f,0.0f);
    B3L_VECT3_SET(pBox->transform.scale,30.0f,30.0f,30.0f);
    pBox->pResource0= (void *)ship_Mesh_Tex;
    pBox->pResource1=(void *)ship2_tex;
    //pBox->pResource0= (void *)b3d_box;
    //pBox->pResource1=(void *)tex;
    SET_OBJ_VISIABLE(pBox);
    SET_OBJ_BACKFACE_CULLING(pBox);
    B3L_AddObjToRenderList(pBox, &rnd);

    B3L_RotateObjInOY(&(pBox->transform.quaternion), 0.1f);
    //rnd.light.lightVect.y = 100.0f;
    B3L_SetLightType(&rnd,dotLight);
    B3L_VECT3_SET(rnd.light.lightVect, 200.0f, 0.0f, 0.0f);
    rnd.camera.transform.translation.x = 0.0f;
    rnd.camera.transform.translation.y =  0.0f;
    rnd.camera.transform.translation.z =  -200.0f;
    //printf("init done\n");
    //B3L_RenderScence(&rnd);
    ClearFbuff(screen.data, F_BUFF_LENGTH, 8);
    float i=0.0f;
    float y;
    int ix,iy;
    for(;i<1.0f;i+=0.01f){
      y = B3L_sin(i);
      iy = B3L_RoundingToS(100.0f - y*20.0f);
      ix= B3L_RoundingToS(100.0f + i*20.0f);
      screen.data[iy*320+ix]=0;

    }
}
f32 currentLevel = 0;
///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
vect3_t at = { 0.0f,0.0f,0.0f };
vect3_t up = { 0.0f,1.0f,0.0f };
float i = 0.0f;
void render(uint32_t time) {
    //rnd.camera.transform.translation.z = B3L_sin(i) * 200.0f;

    //rnd.camera.transform.translation.x = B3L_cos(i) * 200.0f;
    B3L_CameraLookAt(&rnd.camera, &at, &up);
    B3L_RenderScence(&rnd);
    //B3L_CamSetFocusLengthByFOV(&rnd, fov);
    
    if (i > 1.0f) {
        i = 0.0f;
    }
    //i += 0.002f;
    
/*
    B3L_CameraLookAt(&rnd.camera, &at, &up);
    rnd.camera.focalLength = fov;
    B3L_UpdateClipMatrix(&rnd);
    B3L_RenderScence(&rnd);
    DrawSpaceBitmap(140.0f, 100.0f, 170.0f, 160.0f, 0.98f,
      0.0f, 0.0f, 63.0f, 63.0f,
      rnd.pFrameBuff, rnd.pZBuff, (texture_t *)tex, 0);
      */
   /*
  ClearZbuff(zbuff, Z_BUFF_LENGTH);
  ClearFbuff(screen.data, F_BUFF_LENGTH, 8);

  DrawTriangleTexture(164.84f,95.37f, 0.0f, 63.0f, 0.98f,
                      186.599f, 100.119f, 0.0f, 0.0f, 0.98f,
                      186.599f, 138.881f, 63.0f, 0.0f, 0.98f,screen.data,zbuff, (texture_t *)tex, 0);
                   
  DrawTriangleTexture(0.0f,0.0f, 0.0f, 63.0f, 0.98f,
                      100.0f, 0.0f, 0.0f, 0.0f, 0.98f,
                      0.0f, 138.881f, 63.0f, 0.0f, 0.98f,screen.data,zbuff, (texture_t *)tex, 0);
   */                    
  //DrawTriangleTexture(px3,py3, 63.0f, 0.0f, 0.1f, px4, py4, 63.0f, 63.0f, 0.1f, px5, py5, 0.0f, 63.0f, 0.1f, screen.data, zbuff, (texture_t*)tex, (s8)currentLevel);

  //DrawTriangleTexture(px0, py0, 0.0f, 0.0f, 0.01f, px1, py1, 4.0f, 0.0f, 0.01f, px2, py2, 0.0f, 4.0f, 0.01f, screen.data, zbuff, (texture_t*)tex, (s8)currentLevel);
  //DrawTriangleTexture(px3, py3, 4.0f, 0.0f, 0.01f, px4, py4, 4.0f, 4.0f, 0.01f, px5, py5, 0.0f, 4.0f, 0.01f, screen.data, zbuff, (texture_t*)tex, (s8)currentLevel);

  //DrawTriangleTexture(px0, py0, 0.0f, 0.0f, 0.01f, px1, py1, 63.0f, 0.0f, 0.10f, px2, py2, 0.0f, 63.0f, 10.0f, screen.data, zbuff, (texture_t*)tex, (s8)currentLevel);
  //DrawTriangleColor(px3, py3,  10.0f, px4, py4,  0.01f, px5, py5,  0.1f, screen.data, zbuff, 32, (s8)currentLevel);
  /*
  DrawSpaceBitmap(10.0f, 10.0f, px4, py4, 0.001f,
    0.0f, 0.0f, 4.0f, 4.0f,
    screen.data, zbuff, (texture_t*)tex, (s8)currentLevel);
  */
}

///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void update(uint32_t time) {
  
  if (pressed(DPAD_UP)) {
    //rnd.camera.transform.translation.y+=0.5f;
    rnd.light.lightVect.y +=5.0f;
   /* 
   
    px1 += 0.5f; px3 += 0.5f;
    py2 += 0.5f; py4 += 0.5f;
    py5 += 0.5f; px4 += 0.5f;
   
    py0 += 0.5f;
    py1 += 0.5f;
    py2 += 0.5f;
 */

  }
  if (pressed(DPAD_DOWN)) {
    //rnd.camera.transform.translation.y-=0.5f;
      rnd.light.lightVect.y -= 5.0f;
/*
    px1 -= 0.5f; px3 -= 0.5f;
    py2 -= 0.5f; py4 -= 0.5f;
    py5 -= 0.5f; px4 -= 0.5f;

    py0 -= 0.5f;
    py1 -= 0.5f;
    py2 -= 0.5f;
*/
  }
  if (pressed(DPAD_LEFT)) {
    //currentLevel = (currentLevel - 0.1f);
      rnd.camera.transform.translation.z -= 1.5f;
  }
  if (pressed(DPAD_RIGHT)) {
    //currentLevel = (currentLevel + 0.1f);
      rnd.camera.transform.translation.z += 1.5f;
  }
  if (pressed(A)) {
      rnd.camera.transform.translation.z += 1.0f;
  }
  if (pressed(B)) {
      rnd.camera.transform.translation.z -= 1.0f;
  }
  if (pressed(X)) {
      fov += 0.01f;
  }
  if (pressed(Y)) {
      fov -= 0.01f;
  }
}

