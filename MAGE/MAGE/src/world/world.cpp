#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "world\world.hpp"
#include "model\model.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	void World::Render(ComPtr< ID3D11DeviceContext2 > device_context, const TransformBuffer &transform_buffer) {
		for (set< SharedPtr< Model > >::iterator it = m_models.begin(); it != m_models.end(); ++it) {
			(*it)->Render(device_context, *this, transform_buffer);
		}
	}

	//-------------------------------------------------------------------------
	// Models
	//-------------------------------------------------------------------------

	SharedPtr< Model > World::GetModel(const string &name) const {
		for (set< SharedPtr< Model > >::const_iterator it = m_models.cbegin(); it != m_models.cend(); ++it) {
			if ((*it)->GetName() == name) {
				return (*it);
			}
		}
		return nullptr;
	}
	bool World::HasModel(const SharedPtr< Model > model) const {
		for (set< SharedPtr< Model > >::const_iterator it = m_models.cbegin(); it != m_models.cend(); ++it) {
			if ((*it) == model) {
				return true;
			}
		}
		return false;
	}
	void World::RemoveModel(const string &name) {
		set< SharedPtr< Model > >::iterator it = m_models.begin();
		while (it != m_models.end()) {
			if ((*it)->GetName() == name) {
				it = m_models.erase(it);
				break;
			}
			else {
				++it;
			}
		}
	}
	void World::RemoveModel(SharedPtr< Model > model) {
		set< SharedPtr< Model > >::iterator it = m_models.begin();
		while (it != m_models.end()) {
			if ((*it) == model) {
				it = m_models.erase(it);
				break;
			}
			else {
				++it;
			}
		}
	}

	//-------------------------------------------------------------------------
	// Lights
	//-------------------------------------------------------------------------

	SharedPtr< PointLight > World::GetLight(const string &name) const {
		for (set< SharedPtr< PointLight > >::const_iterator it = m_lights.cbegin(); it != m_lights.cend(); ++it) {
			if ((*it)->GetName() == name) {
				return (*it);
			}
		}
		return nullptr;
	}
	bool World::HasLight(const SharedPtr< PointLight > light) const {
		for (set< SharedPtr< PointLight > >::const_iterator it = m_lights.cbegin(); it != m_lights.cend(); ++it) {
			if ((*it) == light) {
				return true;
			}
		}
		return false;
	}
	void World::RemoveLight(const string &name) {
		set< SharedPtr< PointLight > >::iterator it = m_lights.begin();
		while (it != m_lights.end()) {
			if ((*it)->GetName() == name) {
				it = m_lights.erase(it);
				break;
			}
			else {
				++it;
			}
		}
	}
	void World::RemoveLight(SharedPtr< PointLight > light) {
		set< SharedPtr< PointLight > >::iterator it = m_lights.begin();
		while (it != m_lights.end()) {
			if ((*it) == light) {
				it = m_lights.erase(it);
				break;
			}
			else {
				++it;
			}
		}
	}
}