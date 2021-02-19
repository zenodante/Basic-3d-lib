#include "game.hpp"
#include <math.h>

#include "b3d.h"
#include "b3d_port.h"
#include "ship2.h"
using namespace blit;

render_t rnd;
B3LObj_t *pBox;
B3LObj_t* pBox2;
B3LObj_t* pBitmap;
////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init() {
    vect3_t a, b;
    a.x = 10.5f;
    a.y = 270.0f;
    a.z = 0.0f;
    b.x = 10.5f;
    b.y = -120.0f;
    b.z = 0.0f;
    //ClipLineInScreen(&a, &b);
    printf("a: %.3f,%.3f,%.3f\n", a.x, a.y, a.z);
    printf("b: %.3f,%.3f,%.3f\n", b.x, b.y, b.z);



    set_screen_mode(ScreenMode::hires_palette);
    set_screen_palette((const Pen *)(pal), 256);
    
    B3L_RenderInit(&rnd,(fBuff_t *)(screen.data));

    //pBox = B3L_CreatTexMeshObj(&rnd, (B3L_Mesh_t *)ship_Mesh_Tex,(B3L_tex_t *) ship2_tex,
    //                           true, false, 0, false, 16,true);
    pBox = B3L_CreatTexMeshObj_Simple(&rnd, b3d_box, tex);
    pBox2 = B3L_CreatColorMeshObj_Simple(&rnd, b3d_box, box_color);
    pBitmap = B3L_CreatBitmapObj(&rnd, (B3L_tex_t*)tex, 0, 0, 63, 63, 0, true);
    B3L_SetObjPosition(pBitmap, 0.0f, 0.0f, 0.0f);
    B3L_SetObjScale(pBitmap, 50.0f, 50.0f, 50.0f);

    pBox->pMother = pBitmap;
    B3L_SetObjPosition(pBox, 1.0f, 1.0f, 1.0f);
    B3L_SetObjScale(pBox, 1.0f,1.0f, 1.0f);
    pBox2->pMother = pBox;
    B3L_SetObjPosition(pBox2, 1.0f, 1.0f, 1.0f);
    B3L_SetObjScale(pBox2, 1.0f, 1.0f, 1.0f);
    /*
    B3L_SetObjPosition(pBox2, 100.0f, 30.0f, 40.0f);
    B3L_SetObjScale(pBox2, 100.0f, 100.0f, 100.0f);
*/
    //B3L_RotateObjInOY(&(pBox->transform.quaternion), 0.1f);
    //B3L_RotateObjInOY(&(pBox2->transform.quaternion), -0.1f);
    //B3L_RotateObjInOX(&(pBox2->transform.quaternion), -0.1f);


    B3L_SetLightType(&rnd,dotLight);
    B3L_SetLightVect(&rnd, 200.0f, 0.0f, 0.0f);

    B3L_CameraMoveTo(&rnd, 0.0f, 100.0f, -173.0f);
    ROTATE_IN_BODY_X(&(rnd.camera), 0.083333333f);

}
u8 currentLevel = 16;
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
f32 camHeight=0;
void render(uint32_t time) {
    //rnd.camera.transform.translation.z = B3L_sin(i) * 200.0f;

    //rnd.camera.transform.translation.x = B3L_cos(i) * 200.0f;
    //rnd.camera.transform.translation.y = camHeight;
    //CHANGE_OBJ_FIX_LIGHT_VALUE(pBox, currentLevel);
    //B3L_CameraLookAt(&rnd.camera, &at, &up);
    //f32 angle = B3L_atan2(camHeight, 200.0f);

    //B3L_RotateObjInOX(&(rnd.camera.transform.quaternion), angle);

    //rnd.camera.transform.quaternion
    B3L_RenderScence(&rnd);
    i += 0.01f;
    if (i > 1.0f) {
        i = i - 1.0f;
    }
    screen.pen = Pen(0, 0, 0,12);
    screen.text("Text Rendering", minimal_font, Point(5, 4));
  /*   
    DrawTriangleColor(0.0f, 0.0f, 0.1f,
        20.0f, 30.0f, 0.1f,
        90.0f, 120.0f, 0.1f,
        (fBuff_t*)(screen.data), zbuff, 32, 0);
 
  DrawSpaceBitmap(10.0f, 10.0f, px4, py4, 0.001f,
    0.0f, 0.0f, 4.0f, 4.0f,
    screen.data, zbuff, (B3L_tex_t*)tex, (s8)currentLevel);
  */
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
    //camHeight+=0.5f;
    //rnd.light.lightVect.y +=5.0f;
      //pBox->transform.translation.y += 1.0f;
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
      //camHeight -=0.5f;
      //rnd.light.lightVect.y -= 5.0f;
      //pBox->transform.translation.y -= 1.0f;
      
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
      //rnd.camera.transform.translation.z -= 1.5f;
      //pBitmap->transform.translation.x -= 1.0f;
      ROTATE_IN_BODY_X(pBox, -xAngle);
  }
  if (pressed(DPAD_RIGHT)){
    //currentLevel = (currentLevel + 0.1f);
      //rnd.camera.transform.translation.z += 1.5f;
      //pBitmap->transform.translation.x += 1.0f;
      ROTATE_IN_BODY_X(pBox, xAngle);
  }
  if (pressed(A)) {
      rnd.camera.transform.translation.x += 1.0f;
  }
  if (pressed(B)) {
      rnd.camera.transform.translation.x -= 1.0f;
  }
  if (pressed(X)) {
      currentLevel += 1;
  }
  if (pressed(Y)) {
      currentLevel -= 1;
  }
}

