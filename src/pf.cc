#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <benchmark.hxx>
#include "AStar.hxx"
#include "maze.hxx"

void CreateMaze(int seed, int lines, int columns, int heuristic)
{
   AStar::Generator generator;
   Solution s;
   srand(seed);
   std::string heuristicStr = "manhattan";

   mapType map{(std::size_t)columns, std::vector<char>((std::size_t)lines, '\0')};
   s.maze(map);

   generator.setWorldSize({columns, lines});

   switch (heuristic)
   {
   case 1:
      heuristicStr = "euclidean";
      generator.setHeuristic(AStar::Heuristic::euclidean);
      break;
   case 2:
      heuristicStr = "octagonal";
      generator.setHeuristic(AStar::Heuristic::octagonal);
      break;
   case 3:
      heuristicStr = "manhattan";
      generator.setHeuristic(AStar::Heuristic::manhattan);
      break;
   };
   // std::cout << "  ";
   // std::cout << std::endl;

   // auto worldSize = generator.getWorldSize();
   map[columns - 1][lines - 1] = ' ';
   for (int l = 0; l < columns; l++)
   {
      for (int c = 0; c < lines; c++)
      {
         if (map[l][c] == '*')
            generator.addCollision({l, c});
      }
   }
   Benchmark b;
   generator.findPath({0, 0}, {columns - 1, lines - 1});
   auto path = generator.getPath();
   // generator.showMaze();
   // std::cout << "Heuristic: " << heuristicStr << " Seed: " << seed << " Size: " << columns << 'x' << lines << "  Lenght: " << path.size() << "  Time: " << std::fixed << std::setprecision(4) << b.elapsed() << std::endl;
   // for(auto& coordinate : path) {
   //     std::cout << coordinate.x << " " << coordinate.y << "\n";
   // }
}

int main(int /*argc*/, char *argv[])
{
   int seed = atoi(argv[1]);
   int lines = /*atoi(std::getenv("COLUMNS")) - 1*/ 66;
   int columns = /* atoi(std::getenv("LINES")) - 6*/ 111;
   int heuristic = atoi(argv[2]);
   Benchmark b;
   for (int s = seed; s < seed + 1000; s++)
      CreateMaze(s, lines, columns, heuristic);
   std::cout << "time: " << b.elapsed() << std::endl;
}
