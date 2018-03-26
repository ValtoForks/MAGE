//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scene\node.hpp"
#include "scene\camera\camera.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage::rendering {

	//-------------------------------------------------------------------------
	// VoxelizationSettings
	//-------------------------------------------------------------------------
	#pragma region

	Point3 VoxelizationSettings::s_voxel_grid_center;

	U32 VoxelizationSettings::s_voxel_grid_resolution = 128u;

	F32 VoxelizationSettings::s_voxel_size = 0.08f;

	#pragma endregion

	//-------------------------------------------------------------------------
	// Camera
	//-------------------------------------------------------------------------
	#pragma region

	Camera::Camera(ID3D11Device& device) 
		: Component(), 
		m_buffer(device),
		m_clipping_planes(0.01f, 100.0f), 
		m_lens(),
		m_viewport(),
		m_settings() {}

	Camera::Camera(Camera&& camera) noexcept = default;

	Camera::~Camera() = default;

	Camera& Camera::operator=(Camera&& camera) noexcept = default;

	void Camera::UpdateBuffer(ID3D11DeviceContext& device_context, 
							  AntiAliasing aa) const {
		Assert(HasOwner());

		CameraBuffer buffer;

		// Transformations
		{
			const auto& transform            = GetOwner()->GetTransform();
			const auto  world_to_camera      = transform.GetWorldToObjectMatrix();
			const auto  camera_to_world      = transform.GetObjectToWorldMatrix();
			const auto  camera_to_projection = GetCameraToProjectionMatrix();
			const auto  projection_to_camera = GetProjectionToCameraMatrix();
			
			buffer.m_world_to_camera         = XMMatrixTranspose(world_to_camera);
			buffer.m_camera_to_projection    = XMMatrixTranspose(camera_to_projection);
			buffer.m_projection_to_camera    = XMMatrixTranspose(projection_to_camera);
			buffer.m_camera_to_world         = XMMatrixTranspose(camera_to_world);
		}

		// Viewport
		{
			buffer.m_viewport_top_left   = m_viewport.GetTopLeft();
			auto viewport_resolution     = m_viewport.GetSize();
			buffer.m_viewport_inv_resolution_minus1 
				= F32x2(1.0f / (viewport_resolution.m_x - 1u), 
						1.0f / (viewport_resolution.m_y - 1u));
			buffer.m_viewport_resolution = std::move(viewport_resolution);
		}
		
		// SS Viewport
		{
			const auto ss_viewport          = Viewport(m_viewport, aa);
			buffer.m_ss_viewport_top_left   = ss_viewport.GetTopLeft();
			auto ss_viewport_resolution     = ss_viewport.GetSize();
			buffer.m_ss_viewport_inv_resolution_minus1 
				= F32x2(1.0f / (ss_viewport_resolution.m_x - 1u), 
						1.0f / (ss_viewport_resolution.m_y - 1u));
			buffer.m_ss_viewport_resolution = std::move(ss_viewport_resolution);
		}
		
		// Fog and Sky
		{
			const auto& fog           = m_settings.GetFog();
			buffer.m_fog_color        = RGB(fog.GetBaseColor());
			buffer.m_fog_density      = fog.GetDensity();

			const auto& sky           = m_settings.GetSky();
			buffer.m_sky_dome_scale_z = sky.GetScaleZ();
		}

		// Voxelization and Voxel Cone Tracing
		{
			buffer.m_voxel_grid_center 
				= VoxelizationSettings::GetVoxelGridCenter();
			buffer.m_voxel_grid_resolution  
				= VoxelizationSettings::GetVoxelGridResolution();
			buffer.m_voxel_grid_inv_resolution 
				= 1.0f / buffer.m_voxel_grid_resolution;
			buffer.m_voxel_size 
				= VoxelizationSettings::GetVoxelSize();
			buffer.m_voxel_inv_size 
				= 1.0f / buffer.m_voxel_size;
			buffer.m_voxel_texture_max_mip_level 
				= VoxelizationSettings::GetMaxVoxelTextureMipLevel();
			
			const auto& vct               = m_settings.GetVoxelizationSettings();
			buffer.m_nb_cones             = vct.GetNumberOfCones();
			buffer.m_cone_step_multiplier = vct.GetConeStepMultiplier();
			buffer.m_max_cone_distance    = vct.GetMaxConeDistance();
		}

		// Post-processing
		{
			buffer.m_lens_radius    = m_lens.GetLensRadius();
			buffer.m_focal_length   = m_lens.GetFocalLength();
			buffer.m_max_coc_radius = m_lens.GetMaximumCoCRadius();
		}
		
		buffer.m_inv_gamma = 1.0f / m_settings.GetGamma();

		// Update the camera buffer.
		m_buffer.UpdateData(device_context, buffer);
	}

	#pragma endregion
}