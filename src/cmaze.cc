#include <vector>
#include <random>
#include <ctime>
#include "maze.hxx"
#include "app_config.hxx"

AppConfig appConfig("pathfind");

int main(int argc, char *argv[])
{
   appConfig.LoadArguments(argc, argv);
   MazeGenerator s(appConfig.GetSeed());
   int height = appConfig.GetLines();
   int width = appConfig.GetColumns();
   mapType map{(std::size_t)height, std::vector<char>((std::size_t)width, '\0')};
   s.maze(map);
   s.showMaze(map);
   return 0;
}