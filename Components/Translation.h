#pragma once
#include <typeindex>

#include "Component.h"

class Translation : public Component
{
public:
	
	Translation(float x, float y, float z):
		_x(x), _y(y), _z(z) {}
	std::type_index GetType() override;
	void Fill(uint8_t* mem) override;
private:
	float _x = 0;
	float _y = 0;
	float _z = 0;
};
