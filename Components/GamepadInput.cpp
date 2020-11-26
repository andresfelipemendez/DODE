#include "GamepadInput.h"

std::type_index GamepadInput::GetType()
{
	return std::type_index(typeid(GamepadInput));
}
