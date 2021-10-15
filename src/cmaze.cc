#include <vector>
#include <random>
#include <ctime>
#include "maze.hxx"

int main(int /*argc*/, char const * /*argv*/[])
{
   Solution s;
   int height = 40;
   int width = 60;
   srand(time(0));
   //   std::vector<char> row(width);
   mapType map{(std::size_t)height, std::vector<char>((std::size_t)width, '\0')};
   // for (int i = 0; i < height; ++i)
   // {
   //    map.push_back(row);
   // }
   s.maze(map);
   s.showMaze(map);
   return 0;
}