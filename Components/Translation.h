#pragma once
#include <typeindex>

#include "Component.h"

class Translation : public Component
{
public:
	std::type_index GetType() override;
};
