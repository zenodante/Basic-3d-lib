#include "game.hpp"
#include <math.h>

#include "b3d.h"

#include "ship2.h"
#include "manlowpoly.h"

#include "sprite.h"
using namespace blit;

render_t rnd;
B3LObj_t* pShip;
B3LObj_t *pBox;
B3LObj_t* pBox2;
B3LObj_t* pBitmap;
B3LObj_t* pMan;
B3LObj_t* pRain;
euler3_t angle;
#define B3L_logMat3(m)\
  printf("Matrix3:\n  %.3f %.3f %.3f \n  %.3f %.3f %.3f \n  %.3f %.3f %.3f \n"\
   ,(m).m00,(m).m10,(m).m20,\
    (m).m01,(m).m11,(m).m21,\
    (m).m02,(m).m12,(m).m22)
f32 upAngle = 0.0f;
////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
vect3_t at = { 0.0f,0.0f,0.0f };
//f32 result;
void init() {

    set_screen_mode(ScreenMode::hires_palette);
    set_screen_palette((const Pen *)(pal), 256);





    B3L_RenderInit_Simple(&rnd,(fBuff_t *)(screen.data));
    //B3L_SetPerspectiveProject(&rnd, DEFAULT_FOCUS_LENGTH);
    //B3L_Set_Default_Orthographic_Project(&rnd);
    B3L_SetOrthographicProject(&rnd, (1.0f / HALF_RESOLUTION_X));

 
    pRain = RainGeneratorInit(&rnd, true);
    B3L_SetObjPosition(pRain, 0.0f, 100.0f, 0.0f);
    B3L_SetObjScale(pRain, 100.0f, 10.0f, 100.0f);
    //pShip = B3L_CreatTexMeshObj(&rnd, (B3L_Mesh_t *)ship_Mesh_Tex,(B3L_tex_t *) ship2_tex,
    //                           true, false, 0, false, 0,true,true, B3L_MEM_LOW_PRIORITY);
    //pBox = B3L_CreatTexMeshObj_Simple(&rnd, b3d_box, tex);
    //B3L_SetObjPosition(pBox, 0.0f, 0.0f, 0.0f);
   // B3L_SetObjScale(pBox, 10.0f,10.0f, 10.0f);
    //pMan = B3L_CreatTexMeshObj(&rnd, (B3L_Mesh_t*)manobj_Mesh_Tex, (B3L_tex_t*)body_tex,
    //                               true, false,0, false, 0,true,true, B3L_MEM_LOW_PRIORITY);
    pBitmap = B3L_CreatBitmapObj(&rnd, (B3L_tex_t*)sprite_tex, 80, 48,95, 71,
        0, true, true, B3L_MEM_LOW_PRIORITY);

   B3L_SetObjPosition(pBitmap, 0.0f, 12.0f, 0.0f);
   B3L_SetObjScale(pBitmap, 16.0f, 24.0f, 1.0f);

    //B3L_SetObjPosition(pMan, 0.0f, 0.0f, 0.0f);
    //B3L_SetObjScale(pMan, 10.0f, 10.0f, 10.0f);
    //pBox2 = B3L_CreatColorMeshObj_Simple(&rnd, b3d_box, box_color);

    //B3L_SetObjPosition(pShip, 0.0f, 0.0f, 0.0f);
    //B3L_SetObjScale(pShip, 10.0f, 10.0f, 10.0f);

    //pBox->pMother = pShip;

   // pBox2->pMother = pBox;
   // B3L_SetObjPosition(pBox2, 1.0f, 1.0f, 1.0f);
   // B3L_SetObjScale(pBox2, 1.0f, 1.0f, 1.0f);

    B3L_SetLightType(&rnd,dotLight);
   B3L_SetLightVect(&rnd, 200.0f, 0.0f, 0.0f);

    B3L_CameraMoveTo(&rnd, 0.0f,0.0f, -150.0f);
    //ROTATE_IN_BODY_X(&(rnd.camera), 0.125f);

    //B3L_CameraLookAt(&(rnd.camera), &at);
    //rnd.camera.pMother = pMan;
    //B3L_TexBuffInRam(&rnd,(B3L_tex_t*) tex, B3L_MEM_LOW_PRIORITY);
    //result = VcvtFixPointToF32_FixQ31(0x00999999);

}

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
bool project;
char stringBuff[10];
void render(uint32_t time) {
    uint32_t ms_begin = now();
    /*
    if (project == false) {
        B3L_Ser_Default_Perspective_Project(&rnd);
    }
    else {
        B3L_Set_Default_Orthographic_Project(&rnd);
    }
    */
    //B3L_CameraLookAt(&(rnd.camera), &at);
    //B3L_RotateObjInOZ(&(rnd.camera.transform.quaternion), upAngle);
    B3L_RenderScence(&rnd,time);
    
    B3L_GarbageCollection(&rnd,time);
    uint32_t ms_end = now();
    screen.pen = Pen(0, 0, 0,1);
    sprintf(stringBuff, "%d", ms_end-ms_begin);
    screen.text(stringBuff, minimal_font, Point(5, 4));
    
}

///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
f32 xAngle = 0.01f;

void update(uint32_t time) {
  
  if (pressed(DPAD_UP)) {
      rnd.camera.transform.translation.z -= 1.0f;
  }
  if (pressed(DPAD_DOWN)) {
      rnd.camera.transform.translation.z += 1.0f;
  }
  if (pressed(DPAD_LEFT)) {

     // ROTATE_IN_BODY_Y(pBox, -xAngle);
  }
  if (pressed(DPAD_RIGHT)){

      //ROTATE_IN_BODY_Y(pBox, xAngle);
  }
  if (pressed(A)) {
      project = true;
  }
  else {
      project = false;
  }
  if (pressed(B)) {

  }
  if (pressed(X)) {
      upAngle += 0.01f;
  }
  if (pressed(Y)) {
      upAngle -= 0.01f;
  }
}

