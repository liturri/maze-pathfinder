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
   mapType map{(std::size_t)width, std::vector<char>((std::size_t)height, '\0')};
   s.MazeDepthFirstSearch(map);
   // s.MazeRecursive(map);
   s.showMaze(map);
   return 0;
}