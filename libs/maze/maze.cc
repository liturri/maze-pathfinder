#include "string"
#include "maze.hxx"
#include <iostream>

void MazeGenerator::maze(mapType &map)
{
   // U unvisited, ' ' visited
   for (std::size_t x = 0; x < map.size(); ++x)
   {
      for (std::size_t y = 0; y < map[0].size(); ++y)
      {
         map[x][y] = '*';
      }
   }
   _maze(map, 0, 0);
}

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
void MazeGenerator::_maze(mapType &map, int x, int y)
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
   if (map[x][y] == ' ')
      return;

   // some neightbors are visited in addition to the coming direction, return
   // this is to avoid circles in maze
   if (countVisitedNeighbor(map, x, y) > 1)
      return;

   map[x][y] = ' '; // visited

   // shuffle the visitOrder
   shuffle(visitOrder, 4);

   for (int k = 0; k < 4; ++k)
   {
      int nx = x + direct[visitOrder[k]][0];
      int ny = y + direct[visitOrder[k]][1];
      _maze(map, nx, ny);
   }
}

int MazeGenerator::countVisitedNeighbor(mapType &map, int x, int y)
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
      if (map[nx][ny] == ' ')
         count++; // visited
   }
   return count;
}
void MazeGenerator::shuffle(int a[], int n)
{
   std::uniform_int_distribution<int> randGen(0, n - 1);
   for (int x = 0; x < n; ++x)
   {
      swap(a[x], a[randGen(randomEngine)]);
   }
}
void MazeGenerator::swap(int &a, int &b)
{
   int c = a;
   a = b;
   b = c;
}