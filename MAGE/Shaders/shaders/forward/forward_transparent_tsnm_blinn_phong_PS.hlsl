//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#define BRDF_FUNCTION   CookTorranceBRDF
#define BRDF_D_FUNCTION D_BlinnPhong
#define BRDF_F_FUNCTION F_None
#define BRDF_V_FUNCTION V_Implicit
#define DISABLE_VCT
#define ENABLE_TRANSPARENCY
#include "forward\forward.hlsli"