#pragma once
#include <stack>
#include <vector>
#include "MazeTile.h"

namespace Confus
{
	class DFSBacktracking
	{
	public:
		std::stack<MazeTile, std::vector<MazeTile*>> TileStack;
		std::vector<std::vector<MazeTile>> *Maze;
		std::vector<MazeTile*> Neighbours;
		MazeTile *CurrentTile;
	public:
		DFSBacktracking();
		void getNeighbours(MazeTile& a_Tile);
		std::vector<std::vector<MazeTile>>& generateMaze(std::vector<std::vector<MazeTile>>& a_Maze);
		~DFSBacktracking();
	};
}

