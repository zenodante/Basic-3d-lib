
#include "b3d_types.h"
#include "b3d_common.h"

B3L_Particle_t* B3L_GetFreeParticle(render_t* pRender) {
    if (pRender->scene.freeParticleNum == 0) {
        return (B3L_Particle_t*)NULL;
    }
    pRender->scene.freeParticleNum--;

    B3L_Particle_t* popParticle = pRender->scene.pfreeParticles;
    pRender->scene.pfreeParticles = pRender->scene.pfreeParticles->next;
    if (pRender->scene.pfreeParticles != NULL) {
        pRender->scene.pfreeParticles->prev = pRender->scene.pfreeParticles;
    }
    popParticle->next = NULL;
    popParticle->prev = NULL;
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
        if (temp != NULL) {
            temp->prev = pParticle;
        }
        pParticle->prev = pParticle; //as first one, prev point to self
        pRender->scene.freeParticleNum += 1;
    } 
}

void B3L_PopParticleFromGenerator(B3LObj_t* pGen, B3L_Particle_t* pParticle) {
    B3L_Particle_t** ppActive = (B3L_Particle_t**)(&(pGen->pResource0));
    if (*ppActive == pParticle) {
        //pop the first one
        *ppActive = pParticle->next;
        if (pParticle->next != NULL) {
            (*ppActive)->prev = (*ppActive);
        }
    }
    else {
        pParticle->prev->next = pParticle->next;
        if (pParticle->next != NULL) {
            pParticle->next->prev = pParticle->prev;
        }
    }
    pParticle->next = NULL;
    pParticle->prev = NULL;
    (pGen->state) -= (1 << PARTICLE_NUM_SHIFT);
}

void B3L_AddParticleToGenerator(B3LObj_t* pGen, B3L_Particle_t* pParticle) {
    B3L_Particle_t* pTemp = (B3L_Particle_t*)(pGen->pResource0);
    pGen->pResource0 = (void*)pParticle;
    pParticle->prev = pParticle;
    pParticle->next = pTemp;
    if (pTemp != NULL) {
        pTemp->prev = pParticle;
    }
    (pGen->state) += (1 << PARTICLE_NUM_SHIFT);
}

void B3L_ResetParticleList(B3L_Particle_t* pBuff, B3L_Particle_t** ppFreeparticle, u32 particleNum) {
    u32 i;
    *ppFreeparticle = pBuff;
    pBuff[0].state = 0x00000000;
    pBuff[0].next = &(pBuff[1]);
    pBuff[0].prev = &(pBuff[0]);//point to self
    for (i = 1; i < (particleNum - 1); i++) {
        pBuff[i].state = i;
        pBuff[i].next = &(pBuff[i + 1]);
        pBuff[i].prev = &(pBuff[i - 1]);
    }
    pBuff[(particleNum - 1)].state = 0x00000000;
    pBuff[(particleNum - 1)].next = (B3L_Particle_t*)NULL;
    pBuff[(particleNum - 1)].prev = &(pBuff[(particleNum - 2)]);
}