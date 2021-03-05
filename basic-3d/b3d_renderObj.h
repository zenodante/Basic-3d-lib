#ifndef __B3D_RENDER_OBJ_H__
#define __B3D_RENDER_OBJ_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "b3d_types.h"
	extern void RenderObjs(render_t* pRender,u32 time);
	extern u32 B3L_GetMeshResouceSize(B3L_Mesh_t* pMesh);
	extern u32 B3L_GetTexResouceSize(B3L_tex_t* pTexture);
	extern u32 B3L_GetPolyResouceSize(B3L_Polygon_t* pPoly);



#ifdef __cplusplus
}
#endif

#endif
