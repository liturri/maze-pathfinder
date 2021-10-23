#ifndef __MAZE_HXX__
#define __MAZE_HXX__
#include <vector>
#include <random>

using mapType = std::vector<std::vector<char>>;

class MazeGenerator
{
protected:
   std::default_random_engine randomEngine;
public:
   MazeGenerator(unsigned long seed): randomEngine(seed) { };
   // Definition for singly-linked list.
   void maze(mapType &map);
   void showMaze(mapType &map);

   void _maze(mapType &map, int i, int j);
   int countVisitedNeighbor(mapType &map, int i, int j);
   void shuffle(int a[], int n);
   void swap(int &a, int &b);
};

#endif