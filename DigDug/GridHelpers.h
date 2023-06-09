#pragma once
#include "GridComponent.h"

#include <deque>

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

	inline std::deque<grid::Cell*> CalculatePath(const glm::vec3& currentPos, const glm::vec3& desiredPos, grid::GridComponent* pGrid)
	{
		// Define struct
		// -------------
		struct CellRecord
		{
			grid::Cell* pCell{ nullptr };
			grid::Cell* pPreviousCell{ nullptr };
			float costSoFar{ 0.f }; // accumulated g-costs of all the connections leading up to this one
			float estimatedTotalCost{ 0.f }; // f-cost (= costSoFar + h-cost)

			bool operator==(const CellRecord& other) const
			{
				return pCell == other.pCell
					&& pPreviousCell == other.pPreviousCell
					&& costSoFar == other.costSoFar
					&& estimatedTotalCost == other.estimatedTotalCost;
			};

			bool operator<(const CellRecord& other) const
			{
				return estimatedTotalCost < other.estimatedTotalCost;
			};
		};

		// Define lambda
		// -------------
		auto GetHeuristicCost = [](grid::Cell* pStartCell, grid::Cell* pEndCell)
		{
			const glm::vec2 toDestination{ pEndCell->centerPosition - pStartCell->centerPosition };
			return std::max(abs(toDestination.x), abs(toDestination.y));
		};

		// Get cells
		// ---------
		grid::Cell* pStartCell{ pGrid->GetCell(currentPos) };
		grid::Cell* pPlayerCell{ pGrid->GetCell(desiredPos) };

		// StartVariables
		// --------------
		std::deque<grid::Cell*> path;
		std::vector<CellRecord> openList;
		std::vector<CellRecord> closedList;
		CellRecord currentRecord;

		// StartRecord
		CellRecord startRecord;
		startRecord.pCell = pStartCell;
		startRecord.pPreviousCell = nullptr;
		startRecord.costSoFar = 0;
		startRecord.estimatedTotalCost = GetHeuristicCost(pStartCell, pPlayerCell);

		openList.push_back(startRecord);

		bool goToNextConnection{ false };
		CellRecord existingRecord;
		existingRecord.pCell = nullptr;

		bool hasSameNode{};
		bool hasSameConnection{};

		// Start the loop
		// --------------
		while (openList.empty() == false)
		{
			// Get lowestCost node from openList
			currentRecord = *std::min(openList.begin(), openList.end());

			// Check if lowestCost node is endNode
			if (currentRecord.pCell == pPlayerCell)
			{
				break;
			}

			// Get all connections
			for (auto& pConnectedCell : currentRecord.pCell->pConnectedCells)
			{
				//---------------//
				//--First Check--//
				//---------------//

				// Check if node is already on closedList
				for (auto& nodeRecord : closedList)
				{
					if (pConnectedCell == nodeRecord.pCell)
					{
						// If connection is cheaper, replace with old connection
						if (currentRecord.estimatedTotalCost < nodeRecord.estimatedTotalCost)
						{
							existingRecord = nodeRecord;
						}
						// Else just leave it
						else
						{
							goToNextConnection = true;
						}

						break;
					}
				}

				// Remove from closedList if necesarry
				if (existingRecord.pCell)
				{
					for (size_t idx{}; idx < closedList.size(); ++idx)
					{
						hasSameNode = closedList[idx].pCell == currentRecord.pCell;
						hasSameConnection = closedList[idx].pPreviousCell == currentRecord.pPreviousCell;

						if (hasSameNode && hasSameConnection)
						{
							closedList.erase(closedList.begin() + idx);
							break;
						}
					}

					// Reset variables
					existingRecord = {};
					existingRecord.pCell = nullptr;
				}

				// Go next if needed
				if (goToNextConnection)
				{
					goToNextConnection = false;
					continue;
				}


				//----------------//
				//--Second Check--//
				//----------------//

				// Check if node is already on openList
				for (auto& nodeRecord : openList)
				{
					if (pConnectedCell == nodeRecord.pCell)
					{
						// If connection is cheaper, replace with old connection
						if (currentRecord.estimatedTotalCost < nodeRecord.estimatedTotalCost)
						{
							existingRecord = nodeRecord;
						}
						// Else just leave it
						else
						{
							goToNextConnection = true;
						}

						break;
					}
				}

				// Remove from openList if necesarry
				if (existingRecord.pCell)
				{
					for (size_t idx{}; idx < openList.size(); ++idx)
					{
						hasSameNode = openList[idx].pCell == currentRecord.pCell;
						hasSameConnection = openList[idx].pPreviousCell == currentRecord.pPreviousCell;

						if (hasSameNode && hasSameConnection)
						{
							openList.erase(openList.begin() + idx);
							break;
						}
					}

					// Reset variables
					existingRecord = {};
					existingRecord.pCell = nullptr;
				}

				// Go next if needed
				if (goToNextConnection)
				{
					goToNextConnection = false;
					continue;
				}


				//-------------------//
				//--Add to openList--//
				//-------------------//

				CellRecord newRecord;
				newRecord.pCell = pConnectedCell;
				newRecord.pPreviousCell = currentRecord.pCell;
				newRecord.costSoFar = currentRecord.costSoFar + 1;
				newRecord.estimatedTotalCost = GetHeuristicCost(newRecord.pCell, pPlayerCell) + newRecord.costSoFar;

				openList.push_back(newRecord);
			}

			// Remove from openList and add to closedList
			for (size_t idx{}; idx < openList.size(); ++idx)
			{
				hasSameNode = openList[idx].pCell == currentRecord.pCell;
				hasSameConnection = openList[idx].pPreviousCell == currentRecord.pPreviousCell;

				if (hasSameNode && hasSameConnection)
				{
					openList.erase(openList.begin() + idx);
					break;
				}
			}
			closedList.push_back(currentRecord);
		}


		//--------------------//
		//--Reconstruct Path--//
		//--------------------//

		// Do until currentNode is startNode
		while (currentRecord.pCell != pStartCell)
		{
			// Add node to path
			path.push_back(currentRecord.pCell);

			for (auto& records : closedList)
			{
				// TrackBack
				if (records.pCell == currentRecord.pPreviousCell)
				{
					currentRecord = records;
					break;
				}
			}
		}

		path.push_back(pStartCell);
		std::reverse(path.begin(), path.end());
		return path;
	}
}