#include <vector>
#include <random>
#include <ctime>
#include "maze.hxx"

int main(int /*argc*/, char const * /*argv*/[])
{
   MazeGenerator s(time(0));
   int height = 111;
   int width = 66;
   mapType map{(std::size_t)height, std::vector<char>((std::size_t)width, '\0')};
   s.maze(map);
   s.showMaze(map);
   return 0;
}