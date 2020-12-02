#include "EntityManager.h"

#include "Components/Translation.h"

std::map<std::type_index, std::bitset<32>> EntityManager::m_RegisteredComponents;
std::map<std::bitset<32>, chunk, Comparer> EntityManager::m_Archetypes;
uint8_t* const EntityManager::m_Mem = (uint8_t*)malloc(1024 * 1024 * 1024);
const unsigned int chunk_size = 1024;

/*
 receives memory, returns pointer?
 each "component type" must fill the pointer allocated and return a pointer to the next location?
 because we use a array of structures as a table I don't know upfront the size of the archetype but each component does

*/
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

	/*
	 * for each element in the array I need to first order it? i think so to avoid duplicating archetypes because of different
	 * order, then call each component "fill" memory method that receives a pointer and copy from the "potentially" stack and fill the
	 * archetype allocated chunk, then returns a pointer that's an offset? perhaps or have another method that returns the size? i think
	 * the second option its best
	 */

	/*
	I need to test the addvantage of the map vs a vector of chunks
	*/

	if (m_Archetypes.contains(mask))
	{
		// get the offset base on the number of archetypes?
		
		auto mem = m_Archetypes.at(mask);
		


	}
	else
	{
		chunk c;

		c.begin = m_Mem + (chunk_size * (m_Archetypes.size() - 1));
		c.size = 1;
		for (auto && component : components)
		{

		}
		/* 

		this needs its a vector instead of a map i think

		*/

		// insert pointer?

		m_Archetypes.insert({mask, c});
		// auto t = (Translation) offset;
	}
	return;
}