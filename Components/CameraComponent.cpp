#include "CameraComponent.h"

std::type_index CameraComponent::GetType() {
	return std::type_index(typeid(CameraComponent));	
};

void CameraComponent::Fill(uint8_t* mem) {};