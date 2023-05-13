#pragma once

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
}