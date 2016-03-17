#include "DFSBacktracking.h"
#include "Maze.h"

namespace Confus
{

	DFSBacktracking::DFSBacktracking()
	{
	}

	std::vector<std::vector<MazeTile>>& DFSBacktracking::generateMaze(std::vector<std::vector<MazeTile>>& a_Maze)
	{
		//setup globals & variables
		this->Maze = &a_Maze;
		CurrentTile = &(*Maze)[0][0];

		//Startcell must be set to visited, add to visitedcount
		CurrentTile->Raised = false;
		CurrentTile->Wall.hide();

		do
		{
			getNeighbours(*CurrentTile);
			if (Neighbours.size() != 0)
			{
				TileStack.push(CurrentTile);
				MazeTile *tile = Neighbours[rand() % Neighbours.size()];
				int xmovement = (tile->XPos - CurrentTile->XPos) / 2;
				int ymovement = (tile->YPos - CurrentTile->YPos) / 2;


				if (xmovement != 0)
				{
					MazeTile &inBetweenTile = (*Maze)[CurrentTile->XPos + xmovement][CurrentTile->YPos];
					inBetweenTile.Raised = false;
					inBetweenTile.Wall.hide();
				}
				else if (ymovement != 0)
				{
					MazeTile &inBetweenTile = (*Maze)[CurrentTile->XPos][CurrentTile->YPos + ymovement];
					inBetweenTile.Raised = false;
					inBetweenTile.Wall.hide();
				}

				CurrentTile = tile;

				CurrentTile->Raised = false;
				CurrentTile->Wall.hide();
				//Game stuff
			}
			else if (TileStack.size() != 0)
			{
				CurrentTile = TileStack.top();
				TileStack.pop();
			}

		} while (TileStack.size() != 0);



		return *Maze;
	}


	void DFSBacktracking::getNeighbours(MazeTile& a_Tile)
	{
		Neighbours.clear();
		size_t x = static_cast<size_t>(a_Tile.XPos);
		size_t y = static_cast<size_t>(a_Tile.YPos);


		//check if the neighbour is not out of bounds, and is not visited, else add to neighbours
		if (x > 1 && (*Maze)[x - 2][y].Raised)
		{
			Neighbours.push_back(&(*Maze)[x - 2][y]);
		}
		if (x < (*Maze).size() - 2 && (*Maze)[x + 2][y].Raised)
		{
			Neighbours.push_back(&(*Maze)[x + 2][y]);
		}
		if (y > 1 && (*Maze)[x][y - 2].Raised)
		{
			Neighbours.push_back(&(*Maze)[x][y - 2]);
		}
		if (y < (*Maze)[0].size() - 2 && (*Maze)[x][y + 2].Raised)
		{
			Neighbours.push_back(&(*Maze)[x][y + 2]);
		}
	}


	DFSBacktracking::~DFSBacktracking()
	{
	}
}
