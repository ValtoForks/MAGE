#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "shader\shader.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	class LambertianVertexShader : public VertexShader {

	public:

		LambertianVertexShader(const RenderingDevice &device);
		virtual ~LambertianVertexShader() = default;

		virtual void Render(ComPtr< ID3D11DeviceContext2 > device_context, const Material &material, const World &world, const TransformBuffer &transform_buffer) const override;

	private:

		LambertianVertexShader(const LambertianVertexShader &vertex_shader) = delete;
		LambertianVertexShader &operator=(const LambertianVertexShader &vertex_shader) = delete;

		ComPtr< ID3D11Buffer > m_cb_transform;
	};

	class LambertianPixelShader : public PixelShader {

	public:

		LambertianPixelShader(const RenderingDevice &device);
		virtual ~LambertianPixelShader() = default;

		virtual void Render(ComPtr< ID3D11DeviceContext2 > device_context, const Material &material, const World &world) const override;

	private:

		LambertianPixelShader(const LambertianPixelShader &pixel_shader) = delete;
		LambertianPixelShader &operator=(const LambertianPixelShader &pixel_shader) = delete;

		ComPtr< ID3D11Buffer > m_cb_material;
		ComPtr< ID3D11SamplerState >m_sampler;
	};

	inline CombinedShader CreateLambertianShader(const RenderingDevice &device) {
		SharedPtr< VertexShader > vs(new LambertianVertexShader(device));
		SharedPtr< PixelShader > ps(new LambertianPixelShader(device));
		return CombinedShader(vs, ps);
	}
}