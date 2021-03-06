# Basic-3d-lib2
#### This is a 3d engine for stm32h750 based 32blit game system. It would be quite easy to port to any arm cortex m4/m7 mcu with enough ram. 
Several math functions were copy from nvidia cg fast math lib. invert sqrtf is copy from id soft quake.

Left-hand coordinate was used in this lib  
    ^ y  
    |    ^ z  
    |  /  
    |/  
    +--------> x  
      
Screen coordinate is x y[0,0] from top left corner. The real render area is defined by
the RENDER_RESOLUTION_X/Y, the whole frameBuff is defined by WHOLE_FRAME_BUFF_WIDTH/HEIGHT, 
the position shift of the render window is controlled by the address given to the init function.  
0,0 ---------> x  
   |  
   |  
   |  
 y v  
Always using row vector, so v * mat -> new v  

Rotations used quaternion internally in transform struct. You could also set the rotation by euler angles in ZXY order, Positive rotation about an axis follow left-hand method.  
- [x] Light and color shade
- [x] Texture mapping triangle drawing
- [x] 2d Sprite drawing in 3d space
- [x] Polygon drawing
- [x] 3d particles, common per defined particle (rain)
- [x] fast flat color triangle drawing
- [x] Near plane clip option
- [x] LUT 8bit framebuff and 16bit uint zbuff
- [x] Customerized heap management (modified from FreeRTOS heap4.c)
- [x] Heap resource buffering and garbage collection based on reference count

#### How to use the lib
You could use the b3d_config.h to change the default values for the engine.  
b3d_port.h/c contains the heap and z buffer setting which depends on the hardware.  

Call B3L_RenderInit_Simple function to init the render system. It would allocate RAM for buffers and default parameters for the engine.   

struct render  
         |--Scene  
         |--Camera  
         |--Light  
         |--pointer to the framebuffer  
         |--pointer to the z buffer  
         |--pointer to the vector buffer  
         ... and other render parameters  

Scene contains all the resources and objs. Only objs in the active list would be draw to the screen.  
Camera has two type: PERSPECTIVE_PROJECT and OTHROGRAPHIC_PROJECT, you could set the zoom parameter (focuslength) and aspect ratio for the camera  
