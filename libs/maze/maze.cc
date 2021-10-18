#include "string"
#include "maze.hxx"
#include <iostream>

void Solution::maze(mapType &map)
{
   // U unvisited, ' ' visited
   for (std::size_t i = 0; i < map.size(); ++i)
   {
      for (std::size_t j = 0; j < map[0].size(); ++j)
      {
         map[i][j] = '*';
      }
   }
   _maze(map, 0, 0);
}

void Solution::showMaze(mapType &map)
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
void Solution::_maze(mapType &map, int i, int j)
{
   int direct[][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
   int visitOrder[] = {0, 1, 2, 3};
   // out of boundary
   if (i < 0 || j < 0 || i >= (long)map.size() || j >= (long)map[0].size())
      return;
#ifdef DEBUG
   std::cout << "(" << i << ", " << j << ")" << endl;
#endif
   // visited, go back to the coming direction, return
   if (map[i][j] == ' ')
      return;

   // some neightbors are visited in addition to the coming direction, return
   // this is to avoid circles in maze
   if (countVisitedNeighbor(map, i, j) > 1)
      return;

   map[i][j] = ' '; // visited

   // shuffle the visitOrder
   shuffle(visitOrder, 4);

   for (int k = 0; k < 4; ++k)
   {
      int ni = i + direct[visitOrder[k]][0];
      int nj = j + direct[visitOrder[k]][1];
      _maze(map, ni, nj);
   }
}

int Solution::countVisitedNeighbor(mapType &map, int i, int j)
{
   int direct[][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
   int count = 0;
   for (int k = 0; k < 4; ++k)
   {
      int ni = i + direct[k][0];
      int nj = j + direct[k][1];
      // out of boundary
      if (ni < 0 || nj < 0 || ni >= (long)map.size() || nj >= (long)map[0].size())
         continue;
      if (map[ni][nj] == ' ')
         count++; // visited
   }
   return count;
}
void Solution::shuffle(int a[], int n)
{
   for (int i = 0; i < n; ++i)
   {
      swap(a[i], a[rand() % n]);
   }
}
void Solution::swap(int &a, int &b)
{
   int c = a;
   a = b;
   b = c;
}