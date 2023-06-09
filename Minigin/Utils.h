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

	inline bool RectOverlaps(const Rect& firstRect, const Rect& secondRect)
	{
		const bool xOverlaps{ firstRect.x <= secondRect.x + secondRect.width && secondRect.x <= firstRect.x + firstRect.width };
		const bool yOverlaps{ firstRect.y <= secondRect.y + secondRect.height && secondRect.y <= firstRect.y + firstRect.height };

		return (xOverlaps && yOverlaps);
	}

	inline bool IsInsideRect(const glm::vec3& position, const Rect& rect)
	{
		const bool isInsideX{ rect.x <= position.x && position.x <= rect.x + rect.width };
		const bool isInsideY{ rect.y <= position.y && position.y <= rect.y + rect.height };

		return isInsideX && isInsideY;
	}

	inline float GetSqrdMagnitude(const glm::vec3& vector)
	{
		return powf(vector.x, 2.f) + powf(vector.y, 2.f) + powf(vector.z, 2.f);
	}
	inline float GetMagnitude(const glm::vec3& vector)
	{
		return sqrtf(powf(vector.x, 2.f) + powf(vector.y, 2.f) + powf(vector.z, 2.f));
	}

	inline glm::vec3 GetNormalizedVector(const glm::vec3& vector)
	{
		return vector / GetMagnitude(vector);
	}

	inline int RandomInteger(int min, int max)
	{
		return (rand() % (max - min + 1)) + min;
	}
}