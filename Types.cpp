#include "Types.h"

#include <cmath>

void vec2::normalize()
{
	magnitude = static_cast<float>(sqrt(
		static_cast<double>(x) * static_cast<double>(x) +
		static_cast<double>(y) * static_cast<double>(y)));
	x = x / magnitude;
	y = y / magnitude;
}
