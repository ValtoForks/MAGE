#pragma once

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <DXGI1_3.h>

#pragma endregion

//-----------------------------------------------------------------------------
// Linker Directives
//-----------------------------------------------------------------------------
#pragma region

#pragma comment (lib, "dxgi.lib")

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 The supported driver types.
	 */
	const D3D_DRIVER_TYPE g_driver_types[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	/**
	 The supported feature levels.
	 */
	const D3D_FEATURE_LEVEL g_feature_levels[] = {
		D3D_FEATURE_LEVEL_11_1
	};

	/**
	 The allowed pixel formats.
	 */
	const DXGI_FORMAT g_pixel_formats[] = {
		DXGI_FORMAT_B5G5R5A1_UNORM,	   // A four-component,  16-bit unsigned-normalized-integer format that supports 5  bits for each color channel and 1-bit alpha.
		DXGI_FORMAT_B5G6R5_UNORM,	   // A three-component, 16-bit unsigned-normalized-integer format that supports 5  bits for blue, 6 bits for green, and 5 bits for red.
		DXGI_FORMAT_B8G8R8X8_UNORM,	   // A four-component,  32-bit unsigned-normalized-integer format that supports 8  bits for each color channel and 8 bits unused.
		DXGI_FORMAT_B8G8R8A8_UNORM,    // A four-component,  32-bit unsigned-normalized-integer format that supports 8  bits for each color channel and 8-bit alpha.
		DXGI_FORMAT_R10G10B10A2_UNORM, // A four-component,  32-bit unsigned-normalized-integer format that supports 10 bits for each color and 2 bits for alpha.
	};

	/**
	 Returns the number of bits per pixel of the given format.

	 @return		The number of bits per pixel of the given format.
	 */
	size_t BitsPerPixel(DXGI_FORMAT format);
}

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "rendering/window.hpp"
#include "rendering/device_enumeration.hpp"
#include "rendering/renderer.hpp"
#include "rendering/font.hpp"

#pragma endregion