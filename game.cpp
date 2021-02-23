#include "game.hpp"
#include <math.h>

#include "b3d.h"
#include "b3d_port.h"
#include "ship2.h"
using namespace blit;

render_t rnd;
B3LObj_t* pShip;
B3LObj_t *pBox;
B3LObj_t* pBox2;
B3LObj_t* pBitmap;

mat3_t  sourceMat3;

mat3_t  invMat;
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

void init() {

    set_screen_mode(ScreenMode::hires_palette);
    set_screen_palette((const Pen *)(pal), 256);





    B3L_RenderInit(&rnd,(fBuff_t *)(screen.data));

    pShip = B3L_CreatTexMeshObj(&rnd, (B3L_Mesh_t *)ship_Mesh_Tex,(B3L_tex_t *) ship2_tex,
                               true, false, 0, false, 0,true);
    pBox = B3L_CreatTexMeshObj_Simple(&rnd, b3d_box, tex);
    pBox2 = B3L_CreatColorMeshObj_Simple(&rnd, b3d_box, box_color);

    B3L_SetObjPosition(pShip, 0.0f, 0.0f, 0.0f);
    B3L_SetObjScale(pShip, 20.0f, 20.0f, 20.0f);

    //pBox->pMother = pShip;
    B3L_SetObjPosition(pBox, 1.0f, 1.0f, 1.0f);
    B3L_SetObjScale(pBox, 50.0f,50.0f, 50.0f);
    pBox2->pMother = pBox;
    B3L_SetObjPosition(pBox2, 1.0f, 1.0f, 1.0f);
    B3L_SetObjScale(pBox2, 1.0f, 1.0f, 1.0f);

    B3L_SetLightType(&rnd,dotLight);
    B3L_SetLightVect(&rnd, 200.0f, 0.0f, 0.0f);

    B3L_CameraMoveTo(&rnd, 0.0f, 6.0f, -6.0f);
    //ROTATE_IN_BODY_X(&(rnd.camera), 0.125f);

    B3L_CameraLookAt(&(rnd.camera), &at);
    rnd.camera.pMother = pShip;

}

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//


void render(uint32_t time) {

    B3L_CameraLookAt(&(rnd.camera), &at);
    B3L_RotateObjInOZ(&(rnd.camera.transform.quaternion), upAngle);
    B3L_RenderScence(&rnd,time);
    
    screen.pen = Pen(0, 0, 0,1);
    screen.text("Basic 3d engine test", minimal_font, Point(5, 4));

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

  }
  if (pressed(DPAD_DOWN)) {

  }
  if (pressed(DPAD_LEFT)) {

      ROTATE_IN_BODY_Y(pShip, -xAngle);
  }
  if (pressed(DPAD_RIGHT)){

      ROTATE_IN_BODY_Y(pShip, xAngle);
  }
  if (pressed(A)) {

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

