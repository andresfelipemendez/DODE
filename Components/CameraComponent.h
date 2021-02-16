#pragma once

#include "Component.h"

class CameraComponent : public Component
{
	std::type_index GetType() override;
	void Fill(uint8_t* mem) override;
};