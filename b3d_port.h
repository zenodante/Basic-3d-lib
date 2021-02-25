#ifndef __B3D_PORT_H__
#define __B3D_PORT_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "b3d_types.h"
#include "b3d_config.h"
#include "b3d_common.h"

#if B3L_ARM  == 1
extern char __ltdc_start, __ltdc_end;
extern char __fb_start, __fb_end;

extern zBuff_t*  const zbuff;
//extern vect4_t*  const vectBuff;

static inline uint32_t __get_FPSCR(void);
static inline void __set_FPSCR(uint32_t fpscr);

static inline uint32_t __get_FPSCR(void)
{
  uint32_t result;
  __ASM volatile ("VMRS %0, fpscr" : "=r" (result) );
  return(result);
}

static inline void __set_FPSCR(uint32_t fpscr)
{
  __ASM volatile ("VMSR fpscr, %0" : : "r" (fpscr) : "vfpcc", "memory");
}
#else

extern zBuff_t zbuff[Z_BUFF_LENGTH];
extern vect4_t vectBuff[VECT_BUFF_SIZE]; 

#endif

extern u8 ucHeap[ENGINE_HEAP_SIZE];

#ifdef B3L_USING_PARTICLE
extern B3L_Particle_t  particleBuff[B3L_PARTICLE_BUFF_DEPTH];
#endif
 


#ifdef __cplusplus
}
#endif
#endif
