#include "b3d_config.h"
#include "b3d_types.h"
//#include "b3d_port.h"


#if B3L_ARM  == 1

extern char __ltdc_start, __ltdc_end;
extern char __fb_start, __fb_end;

zBuff_t* const zbuff = (((zBuff_t *)&__fb_start)+38400);
__attribute__((section(".main_ram")))  u8 ucHeap[ENGINE_HEAP_SIZE];
#else

zBuff_t zbuff[Z_BUFF_LENGTH];
u8 ucHeap[ENGINE_HEAP_SIZE];
#endif

#ifdef B3L_USING_PARTICLE
B3L_Particle_t  particleBuff[B3L_PARTICLE_BUFF_DEPTH];//18KB
#endif
