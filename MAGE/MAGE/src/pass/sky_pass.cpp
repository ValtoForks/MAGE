//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scene\scene_renderer.hpp"
#include "rendering\rendering_state_manager.hpp"
#include "resource\resource_factory.hpp"
#include "logging\error.hpp"

// Include HLSL bindings.
#include "..\..\shaders\hlsl.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	SkyPass *SkyPass::Get() {
		Assert(SceneRenderer::Get());

		return SceneRenderer::Get()->GetSkyPass();
	}

	SkyPass::SkyPass()
		: m_device_context(Pipeline::GetImmediateDeviceContext()),
		m_sky_vs(CreateFarFullscreenTriangleVS()), 
		m_sky_ps(CreateSkyPS()){}

	SkyPass::SkyPass(SkyPass &&render_pass) = default;

	SkyPass::~SkyPass() = default;

	void XM_CALLCONV SkyPass::BindTransformData(
		FXMMATRIX view_to_world,
		CXMMATRIX projection_to_view) {

		SkyBuffer buffer;
		buffer.m_projection_to_view = XMMatrixTranspose(projection_to_view);
		buffer.m_view_to_world      = XMMatrixTranspose(view_to_world);

		// Update the transform buffer.
		m_transform_buffer.UpdateData(m_device_context, buffer);
		// Bind the transform buffer.
		m_transform_buffer.Bind< Pipeline::PS >(
			m_device_context, SLOT_CBUFFER_PER_FRAME);
	}

	void SkyPass::BindFixedState() const noexcept {
		// VS: Bind the vertex shader.
		m_sky_vs->BindShader(m_device_context);
		// HS: Bind the hull shader.
		Pipeline::HS::BindShader(m_device_context, nullptr);
		// DS: Bind the domain shader.
		Pipeline::DS::BindShader(m_device_context, nullptr);
		// GS: Bind the geometry shader.
		Pipeline::GS::BindShader(m_device_context, nullptr);
		// RS: Bind the rasterization state.
		RenderingStateManager::Get()->BindCullCounterClockwiseRasterizerState(m_device_context);
		// PS: Bind the pixel shader.
		m_sky_ps->BindShader(m_device_context);
		// OM: Bind the depth stencil state.
		RenderingStateManager::Get()->BindLessEqualDepthReadWriteDepthStencilState(m_device_context);
		// OM: Bind the blend state.
		RenderingStateManager::Get()->BindOpaqueBlendState(m_device_context);
	}

	void SkyPass::Render(
		const PassBuffer *scene,
		FXMMATRIX view_to_world,
		CXMMATRIX projection_to_view) {
		
		Assert(scene);

		// Bind the transform data.
		BindTransformData(view_to_world, projection_to_view);
		// Bind the sky SRV.
		Pipeline::PS::BindSRV(m_device_context, 
			SLOT_SRV_TEXTURE,
			scene->GetSky()->GetSRV());
		
		// Bind the primitive topology.
		Pipeline::IA::BindPrimitiveTopology(m_device_context,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Draw the fullscreen triangle.
		Pipeline::Draw(m_device_context, 3u, 0u);
	}
}