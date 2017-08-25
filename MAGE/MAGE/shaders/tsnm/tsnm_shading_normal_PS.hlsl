#include "basic.fx"

float4 PS(PSInputPositionNormalTexture input) : SV_Target {
	const float3 n0     = normalize(input.n_view);
	const float3 n_view = TangentSpaceNormalMapping_PerturbNormal(input.p_view, n0, input.tex);
	return float4(InverseBiasX2(n_view), 1.0f);
}