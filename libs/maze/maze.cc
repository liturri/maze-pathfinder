#include "string"
#include "maze.hxx"
#include <iostream>

void MazeGenerator::showMaze(mapType &map)
{
   std::cout << "+" << std::string(map.size(), '-') << "+" << std::endl;
   for (size_t locY = 0; locY < map[0].size(); locY++)
   {
      std::cout << '|';
      for (size_t locX = 0; locX < map.size(); locX++)
      {
         std::cout << map[locX][locY];
      }
      std::cout << "|\n";
   }
   std::cout << "+" << std::string(map.size(), '-') << "+" << std::endl;
}

// Use DFS
void MazeGenDepthFirsSearch::Generate(mapType &map, int x, int y)
{
   int direct[][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
   int visitOrder[] = {0, 1, 2, 3};
   // out of boundary
   if (x < 0 || y < 0 || x >= (long)map.size() || y >= (long)map[0].size())
      return;
#ifdef DEBUG
   std::cout << "(" << x << ", " << y << ")" << endl;
#endif
   // visited, go back to the coming direction, return
   if (map[x][y] == MazeGenerator::charEmpty)
      return;

   // some neightbors are visited in addition to the coming direction, return
   // this is to avoid circles in maze
   if (countVisitedNeighbor(map, x, y) > 1)
      return;

   map[x][y] = MazeGenerator::charEmpty; // visited

   // shuffle the visitOrder
   shuffle(visitOrder, 4);

   for (int k = 0; k < 4; ++k)
   {
      int nx = x + direct[visitOrder[k]][0];
      int ny = y + direct[visitOrder[k]][1];
      Generate(map, nx, ny);
   }
}

int MazeGenDepthFirsSearch::countVisitedNeighbor(mapType &map, int x, int y)
{
   int direct[][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
   int count = 0;
   for (int k = 0; k < 4; ++k)
   {
      int nx = x + direct[k][0];
      int ny = y + direct[k][1];
      // out of boundary
      if (nx < 0 || ny < 0 || nx >= (long)map.size() || ny >= (long)map[0].size())
         continue;
      if (map[nx][ny] == MazeGenerator::charEmpty)
         count++; // visited
   }
   return count;
}

void MazeGenDepthFirsSearch::shuffle(int a[], int n)
{
   std::uniform_int_distribution<int> randGen(0, n - 1);
   for (int x = 0; x < n; ++x)
   {
      std::swap(a[x], a[randGen(randomEngine)]);
   }
}

void MazeGenRecursive::Generate(mapType &map, int x, int y)
{
   // Starting at the given index, recursively visits every direction in a
   // randomized order.
   // Set my current location to be an empty passage.
   map[x][y] = MazeGenerator::charEmpty;
   // Create an local array containing the 4 directions and shuffle their order.
   int dirs[4];
   dirs[0] = NORTH;
   dirs[1] = EAST;
   dirs[2] = SOUTH;
   dirs[3] = WEST;
   for (int i = 0; i < 4; ++i)
   {
      std::uniform_int_distribution<int> randGen(0, 3);
      int r = randGen(randomEngine);
      int temp = dirs[r];
      dirs[r] = dirs[i];
      dirs[i] = temp;
   }
   // Loop through every direction and attempt to Visit that direction.
   for (int i = 0; i < 4; ++i)
   {
      // dx,dy are offsets from current location. Set them based
      // on the next direction I wish to try.
      int dx = 0, dy = 0;
      switch (dirs[i])
      {
      case NORTH:
         dy = -1;
         break;
      case SOUTH:
         dy = 1;
         break;
      case EAST:
         dx = 1;
         break;
      case WEST:
         dx = -1;
         break;
      }
      // Find the (x,y) coordinates of the grid cell 2 spots
      // away in the given direction.
      int x2 = x + (dx << 1);
      int y2 = y + (dy << 1);
      // Returns "true" if x and y are both in-bounds.

      if ((x2 >= 0 && x2 <= (long)map.size()) && (y2 >= 0 && y2 <= (long)map[0].size()))
      {
         if (map[x2][y2] == MazeGenerator::charWall)
         {
            // (x2,y2) has not been visited yet... knock down the
            // wall between my current position and that position
            map[x2 - dx][y2 - dy] = MazeGenerator::charEmpty;
            // Recursively MazeRecursive (x2,y2)
            Generate(map, x2, y2);
         }
      }
   }
}

void MazeGenBase::CleanMaze(mapType &map)
{
   for (std::size_t x = 0; x < map.size(); ++x)
   {
      for (std::size_t y = 0; y < map[0].size(); ++y)
      {
         map[x][y] = MazeGenerator::charWall;
      }
   }
};
