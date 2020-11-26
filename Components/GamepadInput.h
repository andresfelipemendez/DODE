#pragma once
#include "Component.h"

struct GamepadInput : public Component
{
public:
	GamepadInput();
	std::type_index GetType() override;
};

