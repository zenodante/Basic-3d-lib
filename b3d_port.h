#ifndef __B3D_PORT_H__
#define __B3D_PORT_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "b3d_types.h"
#include "b3d_config.h"

#if B3L_ARM  == 1
extern char __ltdc_start, __ltdc_end;
extern char __fb_start, __fb_end;

#define zbuff     (((zBuff_t *)&__fb_start)+38400)
#define vectBuff  ((vect4_t *)&__ltdc_start)
//#define ucHeap    ((u8 *)(0x24000000))
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
extern u8    ucHeap[437 * 1024];
#endif


#ifdef B3L_USING_PARTICLE
extern B3L_Particle_t  particleBuff[B3L_PARTICLE_BUFF_DEPTH];
#endif



#ifdef __cplusplus
}
#endif
#endif
