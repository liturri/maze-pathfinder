#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <benchmark.hxx>
#include "AStar.hxx"
#include "maze.hxx"

int main(int argc, char *argv[])
{
   AStar::Generator generator;
   Solution s;
   srand(20);
   int lines = /* atoi(std::getenv("LINES")) - 6*/ 30;
   int columns = /*atoi(std::getenv("COLUMNS")) - 1*/ 30;

   mapType map{(std::size_t)lines, std::vector<char>((std::size_t)columns, '\0')};
   s.maze(map);

   generator.setWorldSize({lines, columns});
   if (argc == 2)
   {
      std::cout << "Heuristic: ";

      switch (argv[1][0])
      {
      case '1':
         std::cout << "euclidean";
         generator.setHeuristic(AStar::Heuristic::euclidean);
         break;
      case '2':
         std::cout << "octagonal";
         generator.setHeuristic(AStar::Heuristic::octagonal);
         break;
      case '3':
         std::cout << "manhattan";
         generator.setHeuristic(AStar::Heuristic::manhattan);
         break;
      };
      std::cout << std::endl;
   };
   // generator.setDiagonalMovement(true);
   auto worldSize = generator.getWorldSize();
   map[lines-1][columns-1]=' ';
   for (int l = 0; l < lines; l++)
   {
      for (int c = 0; c < columns; c++)
      {
         if (map[l][c] == '*')
            generator.addCollision({l, c});
      }
      // generator.addCollision({rand() % worldSize.x, rand() % worldSize.y});
   }
   {
      Benchmark b;
      generator.findPath({0, 0}, {worldSize.x - 1, worldSize.y - 1});
      auto path = generator.getPath();
      generator.showMaze();
      std::cout << "Size: " << worldSize.x << 'x' << worldSize.y << "  Lenght: " << path.size() << "  Time: " << std::fixed << std::setprecision(4) << b.elapsed() << std::endl;
   }
   // for(auto& coordinate : path) {
   //     std::cout << coordinate.x << " " << coordinate.y << "\n";
   // }
}
