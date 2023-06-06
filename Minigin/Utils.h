#pragma once
#include "glm/vec3.hpp"

namespace utils
{
	struct Rect
	{
		float x{}, y{};
		float width{}, height{};

		bool operator==(const Rect& rhs)
		{
			if (x == rhs.x && y == rhs.y && width == rhs.width && height == rhs.height) return true;
			else																	    return false;
		}
	};

	inline float GetSqrdMagnitude(const glm::vec3& vector)
	{
		return powf(vector.x, 2.f) + powf(vector.y, 2.f) + powf(vector.z, 2.f);
	}
	inline float GetMagnitude(const glm::vec3& vector)
	{
		return sqrtf(powf(vector.x, 2.f) + powf(vector.y, 2.f) + powf(vector.z, 2.f));
	}
}