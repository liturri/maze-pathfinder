#pragma once
#ifndef __MAZE_HXX__
#define __MAZE_HXX__

#include <vector>
#include <random>

using mapType = std::vector<std::vector<char>>;

class MazeGenBase
{
protected:
   std::default_random_engine randomEngine;

public:
   MazeGenBase(unsigned long _randomEngine) : randomEngine(_randomEngine){};
   void operator()(mapType &map)
   {
      CleanMaze(map);
      Generate(map, 0, 0);
   };
   void CleanMaze(mapType &map);
   virtual void Generate(mapType &map, int x, int y) = 0;
};

class MazeGenerator
{
protected:
   // std::default_random_engine randomEngine;
   MazeGenBase &generator;

public:
   static const char charEmpty = ' ';
   static const char charWall = '*';

   MazeGenerator(MazeGenBase &_generator) : generator(_generator){};

   // MazeGenerator(unsigned long seed, MazeGenBase &_generator) : randomEngine(seed), generator(_generator){};
   // void MazeDepthFirstSearch(mapType &map);
   // void MazeRecursive(mapType &map);
   void Generate(mapType &map) { generator(map); };
   void showMaze(mapType &map);

protected:
   void _MazeDepthFirstSearch(mapType &map, int x, int y);
   // void _MazeRecursive(mapType &map, int x, int y);
};

class MazeGenDepthFirsSearch : public MazeGenBase
{
public:
   void Generate(mapType &map, int x, int y) override;

private:
   using MazeGenBase::MazeGenBase;
   int countVisitedNeighbor(mapType &map, int x, int y);
   void shuffle(int a[], int n);
};

class MazeGenRecursive : public MazeGenBase
{
   static const int NORTH = 0;
   static const int EAST = 1;
   static const int SOUTH = 2;
   static const int WEST = 3;

public:
   using MazeGenBase::MazeGenBase;
   // MazeGenRecursive(std::default_random_engine &_randomEngine) : MazeGenBase(_randomEngine){};
   void Generate(mapType &map, int x, int y) override;
};

#endif