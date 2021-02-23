#include "b3d_config.h"
#include "b3d_types.h"



#if B3L_ARM  == 1


#else

zBuff_t zbuff[Z_BUFF_LENGTH];
vect4_t vectBuff[VECT_BUFF_SIZE]; //10KB

u8    ucHeap[437 * 1024];
#endif

#ifdef B3L_USING_PARTICLE
B3L_Particle_t  particleBuff[B3L_PARTICLE_BUFF_DEPTH];//18KB
#endif
