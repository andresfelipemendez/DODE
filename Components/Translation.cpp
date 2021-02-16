#include "Translation.h"

std::type_index Translation::GetType()
{
	return std::type_index(typeid(Translation));
}

void Translation::Fill(uint8_t* mem)
{
	// type punning, I think this could be solved with 
	// https://www.youtube.com/watch?v=_qzMpk-22cc
	Translation* t = reinterpret_cast<Translation*>(mem);
	t->_x = _x;
	t->_y = _y;
	t->_z = _z;
	int i = 0;	
}
