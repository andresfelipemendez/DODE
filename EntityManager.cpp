#include "EntityManager.h"

#include "Components/Translation.h"

std::map<std::type_index, std::bitset<32>> EntityManager::m_RegisteredComponents;
std::map<std::bitset<32>, chunk, Comparer> EntityManager::m_Archetypes;

void EntityManager::AddEntity(const std::vector<Component*>& components)
{
	std::bitset<32> mask;
	for (auto && component : components)
	{
		if (m_RegisteredComponents.contains(component->GetType()))
		{
			mask |= m_RegisteredComponents.at(component->GetType());
		}
		else
		{
			auto bitval = static_cast<std::bitset<32>>(1 << m_RegisteredComponents.size());
			m_RegisteredComponents.insert({component->GetType(), {bitval}});
			mask |= bitval;
		}	
	}

	if (m_Archetypes.contains(mask))
	{
		auto mem = m_Archetypes.at(mask);
		void* end = mem.end;
	}
	else
	{
		m_Archetypes.insert({mask, {}});
	}
	return;
}