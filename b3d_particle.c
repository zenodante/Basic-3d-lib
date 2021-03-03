
#include "b3d_types.h"
#include "b3d_common.h"

B3L_Particle_t* B3L_GetFreeParticle(render_t* pRender) {
    if (pRender->scene.freeParticleNum == 0) {
        return (B3L_Particle_t*)NULL;
    }
    pRender->scene.freeParticleNum--;
    B3L_Particle_t* popParticle = pRender->scene.pfreeParticles;
    pRender->scene.pfreeParticles = pRender->scene.pfreeParticles->next;
    return popParticle;
}
u32  B3L_GetFreeParticleNum(render_t* pRender) {
    return pRender->scene.freeParticleNum;
}
void B3L_ReturnParticleToPool(B3L_Particle_t* pParticle, render_t* pRender) {
    if (pParticle != NULL) {
        B3L_Particle_t* temp = pRender->scene.pfreeParticles;
        pParticle->state = 0x00000000;
        pRender->scene.pfreeParticles = pParticle;
        pParticle->next = temp;
        pRender->scene.freeParticleNum += 1;
    } 
}

void B3L_PopParticleFromGenerator(B3LObj_t* pGen, B3L_Particle_t* pParticle) {

}

void B3L_AddParticleToGenerator(B3LObj_t* pGen, B3L_Particle_t* pParticle) {
    B3L_Particle_t* pTemp = (B3L_Particle_t*)(pGen->pResource0);
    pGen->pResource0 = (void*)pParticle;
    pParticle->next = pTemp;
    ((particleGenerator_t*)(pGen->pResource1))->particleNum++;
}

void B3L_ResetParticleList(B3L_Particle_t* pBuff, B3L_Particle_t** ppFreeparticle, u32 particleNum) {
    u32 i;
    *ppFreeparticle = pBuff;
    for (i = 0; i < (particleNum - 1); i++) {
        pBuff[i].state = 0x00000000;
        pBuff[i].next = &(pBuff[i + 1]);
    }
    pBuff[(particleNum - 1)].state = 0x00000000;
    pBuff[(particleNum - 1)].next = (B3L_Particle_t*)NULL;

}