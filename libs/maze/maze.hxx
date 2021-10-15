#ifndef __MAZE_HXX__
#define __MAZE_HXX__
#include <vector>

using mapType = std::vector<std::vector<char>>;

class Solution
{
public:
   // Definition for singly-linked list.
   void maze(mapType &map);
   void showMaze(mapType &map);

   void _maze(mapType &map, int i, int j);
   int countVisitedNeighbor(mapType &map, int i, int j);
   void shuffle(int a[], int n);
   void swap(int &a, int &b);
};

#endif