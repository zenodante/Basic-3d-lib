#ifndef __B3D_CONFIG_H__
#define __B3D_CONFIG_H__

#ifdef TARGET_32BLIT_HW 
#define B3L_ARM       1
#else
#define B3L_ARM       0
#endif

#define ENGINE_HEAP_SIZE        ((u32)(1024*437))

#define B3L_FIX_BITS            16
//vect buff size limited the max vectors in single obj
#define VECT_BUFF_SIZE          1024
//obj buff size limited the max objs in a scene 
#define OBJ_BUFF_SIZE           64
//#define LIGHT_BUFF_SIZE         1024


#define Z_LIMIT_NUM            (0xffff)

//the whole frame buff size
#define WHOLE_FRAME_BUFF_WIDTH  320
#define WHOLE_FRAME_BUFF_HEIGHT 240
//the render window size
#define RENDER_RESOLUTION_X     320
#define RENDER_RESOLUTION_Y     240
#define RENDER_X_SHIFT          WHOLE_FRAME_BUFF_WIDTH
#define F_BUFF_LENGTH           (WHOLE_FRAME_BUFF_WIDTH*WHOLE_FRAME_BUFF_HEIGHT)
#define Z_BUFF_LENGTH           (RENDER_RESOLUTION_X * RENDER_RESOLUTION_Y)
//half resolution in floating point value
#define HALF_RESOLUTION_X       159.5f
#define HALF_RESOLUTION_Y       119.5f
//The default aspect ratio value, you could change it at camera parm
#define DEFAULT_ASPECT_RATIO    ((4.0f)/(3.0f))
//1.0f == 90 degree fov,smaller is larger fov
#define DEFAULT_FOCUS_LENGTH    (1.0f)


#define USING_LIGHT
//#define USING_COLOR_LEVEL
#define  B3L_DO_NEAR_PLANE_CLIP     1

//maybe all here parameter could be dynamic set in render struct to balance render load?
#define DEFAULT_FAR_PLANE            500.0f
#define DEFAULT_NEAR_PLANE            10.0f
#define LEVEL_0_DEFAULT_DISTANCE     400.0f
#define LEVEL_1_DEFAULT_DISTANCE     800.0f
#define LEVEL_1_DEFAULT_LIGHT          (-2)
//level 0, calculate light, texture
//level 1, calculate texture
//level 2,??

//if you want to use any particle effects
//#define B3L_USING_PARTICLE   
//max particle numbers in a scene, different particle generator share the same buff pool for the particle
//36 byte per particle for ARM32 system, it is ~18KB for 512 particle
#ifdef B3L_USING_PARTICLE
#define B3L_PARTICLE_BUFF_DEPTH    512
#endif

#define _RAM_FUNC   __attribute__((long_call,section(".itcm")))

// do the near plane clip, otherwise it will skip these triangles, set the 0 could 
//speedup the rendering


#ifdef USING_LIGHT
#ifndef USING_COLOR_LEVEL
#define USING_COLOR_LEVEL
#endif
#endif

#endif
