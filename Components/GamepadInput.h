#pragma once
#include "Component.h"

struct GamepadInput : public Component
{
public:
	GamepadInput(){};
	std::type_index GetType() override;
	void Fill(uint8_t* mem) override;
};

