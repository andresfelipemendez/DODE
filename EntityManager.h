#pragma once
#include <bitset>
#include <map>
#include <typeindex>
#include <vector>

#include "Components/Component.h"

/* 
	I should typedef uint8_t to somethink like raw_memory or something similar, I wonder if there is a semantic way to
	express this
*/

struct chunk
{
	uint8_t* begin;
	uint16_t size;
};

struct Comparer
{
	bool operator()(const std::bitset<32> &b1, const std::bitset<32> &b2) const
	{
		return b1.to_ulong() < b2.to_ulong();
	}
};

struct EntityManager
{
	static void AddEntity(const std::vector<Component*>& components);
private:
	static std::map<std::type_index, std::bitset<32>> m_RegisteredComponents;
	static std::map<std::bitset<32>, chunk, Comparer> m_Archetypes;

	static uint8_t* m_Mem;
	static const unsigned int chunk_size = 1024;
};
