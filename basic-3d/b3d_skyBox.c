#include "b3d_types.h"
#include "b3d_common.h"
#include "b3d_math.h"
void SkyBox_FillColor(render_t* pRnd) {
#if fBuff_t == u8
    s32 length = F_BUFF_LENGTH >> 2;
    int32_t i = length & 0x0000000F;
    length = length >> 4;
    u32* Addr = ((u32*)(pRnd->pFrameBuff));
    fBuff_t color = pRnd->scene.defaultColor;
    u32 value = color | (color << 8) | (color << 16) | (color << 24);
    while (length--) {
        *Addr++ = value; *Addr++ = value; *Addr++ = value;
        *Addr++ = value; *Addr++ = value; *Addr++ = value;
        *Addr++ = value; *Addr++ = value; *Addr++ = value;
        *Addr++ = value; *Addr++ = value; *Addr++ = value;
        *Addr++ = value; *Addr++ = value; *Addr++ = value;
        *Addr++ = value;
    }
    switch (i) {
    case 15:*Addr++ = value;
    case 14:*Addr++ = value;
    case 13:*Addr++ = value;
    case 12:*Addr++ = value;
    case 11:*Addr++ = value;
    case 10:*Addr++ = value;
    case 9:*Addr++ = value;
    case 8:*Addr++ = value;
    case 7:*Addr++ = value;
    case 6:*Addr++ = value;
    case 5:*Addr++ = value;
    case 4:*Addr++ = value;
    case 3:*Addr++ = value;
    case 2:*Addr++ = value;
    case 1:*Addr = value;
    case 0:break;
    }



#endif
}