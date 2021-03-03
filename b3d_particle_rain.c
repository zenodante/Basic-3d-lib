#include "b3d_types.h"
#include "b3d_common.h"
#include "b3d_math.h"
#include "b3d_draw.h"
#include "b3d_obj.h"
#include "b3d_particle.h"
static const particleGenerator_t rainGenFunc;
static void RainGenFunc(render_t* pRender, B3LObj_t* pObj, mat4_t* O2WMat, u32 passedTime);
static void RainUpdateFunc(render_t* pRender, B3LObj_t* pObj, mat4_t* O2WMat, u32 passedTime);

f32 rainPerMs = 0.3f;
f32 lowestY = 0.0f;
f32 rainYSpeed = 0.10f;
u8  rainColor = 15;
__STATIC_FORCEINLINE void  Vect3Xmat4ToScreen4(vect3_t* pV, mat4_t* pMat, vect4_t* pResult);
__STATIC_FORCEINLINE void  Vect3Xmat4ToScreen4(vect3_t* pV, mat4_t* pMat, vect4_t* pResult) {
	f32 x = pV->x; f32 y = pV->y; f32 z = pV->z;
	f32 rx, ry, rz, rw;
	//u32 testResult = 0;
#define dotCol(col)\
        ((x*(pMat->m##col##0)) +\
        (y*(pMat->m##col##1)) +\
        (z*(pMat->m##col##2)) +\
        (pMat->m##col##3))

	rx = dotCol(0);
	ry = dotCol(1);
	rz = dotCol(2);
	rw = dotCol(3);


	f32 factor = 1.0f / (rw);//prevent div zero error
	f32 screenX = (HALF_RESOLUTION_X + rx * factor * HALF_RESOLUTION_X);
	f32 screenY = (HALF_RESOLUTION_Y - ry * factor * HALF_RESOLUTION_Y);
	rz = rz * factor;
	pResult->w = rw;
	pResult->x = screenX;
	pResult->y = screenY;
	pResult->z = rz;
#undef dotCol

}

static void RainGenFunc(render_t* pRender, B3LObj_t* pObj, mat4_t* O2WMat, u32 passedTime) {
	s32 num = B3L_RoundingToS(passedTime * rainPerMs);
	if (num <= 0) {
		return;
	}
	for (; num > 0; num--) {
		B3L_Particle_t* pPart= B3L_GetFreeParticle(pRender);
		if (pPart == NULL) {
			return;
		}
		vect3_t vect;
		vect.x = B3L_FloatRandom() - 0.5f;
		vect.y = B3L_FloatRandom() - 0.5f;
		vect.z = B3L_FloatRandom() - 0.5f;
		vect4_t result;
		B3L_Vect3Xmat4(&vect, O2WMat,&result);
		pPart->position.x = result.x;
		pPart->position.y = result.y;
		pPart->position.z = result.z;
		B3L_AddParticleToGenerator(pObj, pPart);
	}


}
static void RainUpdateFunc(render_t* pRender, B3LObj_t* pObj, mat4_t* O2WMat, u32 passedTime) {
	s32 particleNum = (pObj->state) >> PARTICLE_NUM_SHIFT;
	B3L_Particle_t* currentParticle = (B3L_Particle_t*)(pObj->pResource0);
	f32 downY = rainYSpeed * passedTime;
	mat4_t* pW2CMat = &(pRender->camera.camW2CMat);
	fBuff_t* pFrameBuff = pRender->pFrameBuff;
	zBuff_t* pZbuff = pRender->pZBuff;
	while (currentParticle != NULL) {
		//move y down
		currentParticle->position.y -= downY;
		if (currentParticle->position.y < lowestY) {
			B3L_Particle_t* temp = currentParticle;
			currentParticle = currentParticle->next;
			B3L_PopParticleFromGenerator(pObj, temp);
			B3L_ReturnParticleToPool(temp, pRender);
		}
		else {
			//draw the particle
			vect4_t result,result2;
			vect3_t rainEnd;
			rainEnd.x = currentParticle->position.x;
			rainEnd.y = currentParticle->position.y - 2.0f;
			rainEnd.z = currentParticle->position.z;
			Vect3Xmat4ToScreen4(&(currentParticle->position), pW2CMat, &result);
			Vect3Xmat4ToScreen4(&(rainEnd), pW2CMat, &result2);
			if (result.z > 0.0f) {
				DrawSpaceLine(result.x, result.y, result.z, result2.x, result2.y, result2.z, rainColor, pFrameBuff, pZbuff, 0);
			}
			currentParticle = currentParticle->next;
		}
	}
}

static const particleGenerator_t rainGenFunc = { RainGenFunc,RainUpdateFunc };


B3LObj_t* RainGeneratorInit(render_t* pRender,bool active) {

	B3LObj_t* pObj = B3L_CreatParticleGenObj(pRender, (particleGenerator_t* )(&rainGenFunc), true, active);

	return pObj;
}

void StopRain(B3LObj_t* pObj) {
	B3L_CLR(pObj->state, OBJ_PARTICLE_GEN_ACTIVE);
}