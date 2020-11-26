#pragma once
#include <bitset>
#include <map>
#include <typeindex>
#include <vector>

#include "Components/Component.h"

struct chunk
{
	void* begin;
	void* end;
};

struct Comparer {
	bool operator() (const std::bitset<32> &b1, const std::bitset<32> &b2) const {
		return b1.to_ulong() < b2.to_ulong();
	}
};

struct EntityManager
{
	static void AddEntity(const std::vector<Component*>& components);
private:
	static std::map<std::type_index, std::bitset<32>> m_RegisteredComponents;
	static std::map<std::bitset<32>, chunk, Comparer> m_Archetypes;
};
