#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scene\script\behavior_script.hpp"
#include "scene\sprite\sprite_text.hpp"
#include "system\cpu_monitor.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage::script {

	class StatsScript final : public BehaviorScript {

	public:

		//---------------------------------------------------------------------
		// Class Member Variables
		//---------------------------------------------------------------------

		static constexpr F64 s_resource_fetch_period = 1.0;

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		StatsScript();
		StatsScript(const StatsScript& script) noexcept;
		StatsScript(StatsScript&& script) noexcept;
		virtual ~StatsScript();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		StatsScript& operator=(const StatsScript& script) noexcept;
		StatsScript& operator=(StatsScript&& script) noexcept;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		virtual void Load([[maybe_unused]] Engine& engine) override;
		virtual void Update([[maybe_unused]] Engine& engine) override;

	private:

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		ProxyPtr< rendering::SpriteText > m_text;

		F32 m_accumulated_wall_clock_time;
		U32 m_accumulated_nb_frames;
		U32 m_last_frames_per_second;
		F32 m_last_ms_per_frame;
		F32 m_last_cpu_usage;
		U32 m_last_ram_usage;
	};
}