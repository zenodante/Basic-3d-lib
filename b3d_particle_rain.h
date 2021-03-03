#ifndef __B3D_PARTICLE_RAIN_H__
#define __B3D_PARTICLE_RAIN_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "b3d_types.h"

	extern f32 rainPerMs;
	extern B3LObj_t* RainGeneratorInit(render_t* pRender, bool active);
	extern void StopRain(B3LObj_t* pObj);




#ifdef __cplusplus
}
#endif
#endif
