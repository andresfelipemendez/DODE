#pragma once
#include <typeindex>
#include <cstdint>

class Component
{
public:
	virtual std::type_index GetType() = 0;
	virtual void Fill(uint8_t* mem) = 0;
};
