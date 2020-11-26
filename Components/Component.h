#pragma once
#include <typeindex>

class Component
{
public:
	virtual std::type_index GetType() = 0;
};
