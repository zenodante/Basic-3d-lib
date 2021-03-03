#ifndef __B3D_PARTICLE_H__
#define __B3D_PARTICLE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "b3d_types.h"

extern B3L_Particle_t* B3L_GetFreeParticle(render_t* pRender);
extern u32  B3L_GetFreeParticleNum(render_t* pRender);
extern void B3L_ReturnParticleToPool(B3L_Particle_t* pParticle, render_t* pRender);
extern void B3L_PopParticleFromGenerator( B3LObj_t* pGen, B3L_Particle_t* pParticle);
extern void B3L_AddParticleToGenerator(B3LObj_t* pGen, B3L_Particle_t* pParticle);
extern void B3L_ResetParticleList(B3L_Particle_t* pBuff, B3L_Particle_t** ppFreeparticle,u32 particleNum);




#ifdef __cplusplus
}
#endif
#endif
