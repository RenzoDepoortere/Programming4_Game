#pragma once
#include "GridComponent.h"

namespace grid
{
	enum CellRelativeDirection
	{
		Up, Down, Left, Right, NR_DIRECTIONS
	};
	inline CellRelativeDirection RelativeDirection(Cell* pFrom, Cell* pTo)
	{
		// Get bools
		const bool lowerRow{ pTo->rowCol.x < pFrom->rowCol.x };
		const bool lowerCol{ pTo->rowCol.y < pFrom->rowCol.y };
		const bool notSameRow{ pTo->rowCol.x != pFrom->rowCol.x };

		// If not same row
		if (notSameRow)
		{
			// Row is lower
			if (lowerRow)	return Up;
			// Row is higher
			else			return Down;
		}
		// If not same col
		else
		{
			// Col is lower
			if (lowerCol)	return Left;
			// Col is higher
			else			return Right;
		}
	}

	inline bool IsInDirtCell(const glm::vec3& position, grid::GridComponent* pGrid)
	{
		grid::Cell* pCell{ pGrid->GetCell(position) };
		if (pCell == nullptr) return false;

		if (pCell->textureID != 0) return true;
		else					   return false;
	}
}