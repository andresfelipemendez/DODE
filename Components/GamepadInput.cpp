#include "GamepadInput.h"

std::type_index GamepadInput::GetType()
{
	return std::type_index(typeid(GamepadInput));
}

void GamepadInput::Fill(uint8_t* mem)
{
}
