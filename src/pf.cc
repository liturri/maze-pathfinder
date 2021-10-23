#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <benchmark.hxx>
#include "AStar.hxx"
#include "maze.hxx"
#include "seeds.hxx"
#include <thread>

MazeLengthsDB lengthsDB;

int lines;
int columns;
int heuristic;

TypeLength CreateMaze(TypeSeed seed, int lines, int columns, int heuristic)
{
   AStar::Generator generator;
   MazeGenerator s(seed);
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
   // std::cout << "Heuristic: " << heuristicStr << " Seed: " << seed << " Size: " << columns << 'x' << lines << "  Length: " << path.size() << "  Time: " << std::fixed << std::setprecision(4) << b.elapsed() << std::endl;
   // for(auto& coordinate : path) {
   //     std::cout << coordinate.x << " " << coordinate.y << "\n";
   // }
   return path.size();
}

void workThread(TypeSeed from, TypeSeed to)
{
   std::cerr << "Work: " << std::this_thread::get_id() << std::endl;
   for (TypeSeed s = from; s < to; s++)
   {
      Benchmark b;
      auto length = CreateMaze(s, lines, columns, heuristic);
      if (lengthsDB.IsEmpty(length))
      {
         lengthsDB.AddItem(length, s);
         std::cout << "Len: " << length << " Seed: " << s << std::endl;
      }
      // lengthsDB.IsEmpty(s, length);
      // std::cerr << "Len: " << length << " Seed: " << s << " Time: " << b.elapsed() << std::endl;
   }
}

int main(int argc, char *argv[])
{
   int seed = atoi(argv[1]);
   lines = /*atoi(std::getenv("COLUMNS")) - 1*/ 66;
   columns = /* atoi(std::getenv("LINES")) - 6*/ 111;
   heuristic = argc > 2 ? atoi(argv[2]) : 3;
   const auto processorsCount = std::thread::hardware_concurrency();
   TypeSeed totalCantItems = 10000;
   TypeSeed itemsPerThread = totalCantItems / processorsCount;
   Benchmark b;

   {
      auto fileFD = std::ifstream("/tmp/seeds.db");
      lengthsDB.ImportTextFromFile(fileFD);
   }

   std::vector<std::thread> threadList;

   for (unsigned int threadNumber = 0; threadNumber < processorsCount; threadNumber++)
   {
      std::thread th = std::thread(workThread, seed + itemsPerThread * threadNumber, seed + itemsPerThread * (threadNumber + 1));
      threadList.push_back(std::move(th));
   }

   for (std::thread &th : threadList)
   {
      // If thread Object is Joinable then Join that thread.
      if (th.joinable())
         th.join();
   }
   std::cerr << "time: " << b.elapsed() << std::endl;
   {
      auto fileFD = std::ofstream("/tmp/seeds.db");
      lengthsDB.ExportTextToFile(fileFD);
   }
}
