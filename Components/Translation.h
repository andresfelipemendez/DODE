#pragma once
#include <typeindex>

#include "Component.h"

class Translation : public Component
{
public:
	float x = 0;
	float y = 0;
	float z = 0;
	std::type_index GetType() override;
	void Fill(uint8_t* mem) override;
};
