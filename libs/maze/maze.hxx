#ifndef __MAZE_HXX__
#define __MAZE_HXX__
#include <vector>
#include <random>

using mapType = std::vector<std::vector<char>>;

class MazeGenerator
{
   static const int NORTH = 0;
   static const int EAST = 1;
   static const int SOUTH = 2;
   static const int WEST = 3;

protected:
   std::default_random_engine randomEngine;

public:
   static const char charEmpty = ' ';
   static const char charWall = '*';

   MazeGenerator(unsigned long seed) : randomEngine(seed){};
   void MazeDepthFirstSearch(mapType &map);
   void MazeRecursive(mapType &map);
   void showMaze(mapType &map);

protected:
   void _MazeDepthFirstSearch(mapType &map, int x, int y);
   void _MazeRecursive(mapType &map, int x, int y);
   int countVisitedNeighbor(mapType &map, int x, int y);
   void shuffle(int a[], int n);
};

#endif