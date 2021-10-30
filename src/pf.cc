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
#include <mutex>
#include <locale>
#include "app_config.hxx"

std::mutex console;

MazeLengthsDB lengthsDB;

class PathFindAppConfig : public AppConfig
{
protected:
   std::string SeedFileName;

public:
   PathFindAppConfig(std::string name) : AppConfig(name){};
   void FillArgumentsList() override
   {
      AppConfig::FillArgumentsList();
      appOptions.add_argument("seeds_file").help("Seeds db filename").required();
   };
   void ProcessArguments() override
   {
      AppConfig::ProcessArguments();
      SeedFileName = appOptions.get<std::string>("seeds_file");
   };
   std::string GetSeedFilename() { return SeedFileName; };
};

PathFindAppConfig appConfig("pathfind");

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
   return path.size();
}

void workThread(TypeSeed from, TypeSeed to)
{
   if (appConfig.DebugMode())
   {
      std::lock_guard<std::mutex> lock(console);
      std::cerr << "Thread: " << std::this_thread::get_id() << " From: " << from << " To: " << to << std::endl;
   }
   for (TypeSeed s = from; s < to; s++)
   {
      Benchmark b;
      auto length = CreateMaze(s, appConfig.GetLines(), appConfig.GetColumns(), appConfig.GetHeuristic());
      if (lengthsDB.IsEmpty(length))
      {
         lengthsDB.AddItem(length, s);
         std::lock_guard lock(console);
         std::cout << "Len: " << length << " Seed: " << s << std::endl;
         // console.unlock();
      }
      // lengthsDB.IsEmpty(s, length);
      // std::cerr << "Len: " << length << " Seed: " << s << " Time: " << b.elapsed() << std::endl;
   }
}

int main(int argc, char *argv[])
{
   appConfig.LoadArguments(argc, argv);
   if (!appConfig.DebugMode())
   {
      lengthsDB.DisableDebug();
   };
   TypeSeed seed = appConfig.GetSeed();
   std::string seedsFileName = appConfig.GetSeedFilename();
   const auto processorsCount = std::thread::hardware_concurrency();
   TypeSeed totalCantItems = 100000;
   TypeSeed itemsPerThread = totalCantItems / processorsCount;

   if (appConfig.DebugMode())
   {
      std::cerr << "Seed: " << appConfig.GetSeed() << " Cols*Lines: " << appConfig.GetColumns() << '*' << appConfig.GetLines() << std::endl;
   }
   Benchmark b;

   std::cerr.imbue(std::locale("es_AR.UTF-8"));
   {
      auto fileFD = std::ifstream(seedsFileName);
      lengthsDB.ImportTextFromFile(fileFD);
   }

   std::vector<std::thread> threadList;

   for (unsigned int threadNumber = 0; threadNumber < processorsCount - 1; threadNumber++)
   {
      std::thread th = std::thread(workThread, seed + itemsPerThread * threadNumber, seed + itemsPerThread * (threadNumber + 1));
      threadList.push_back(std::move(th));
   }
   std::thread th = std::thread(workThread, seed + itemsPerThread * (processorsCount - 1), seed + totalCantItems);
   threadList.push_back(std::move(th));

   for (std::thread &th : threadList)
   {
      // If thread Object is Joinable then Join that thread.
      if (th.joinable())
         th.join();
   }
   console.lock();
   std::cerr << "Seeds: " << seed << '-' << seed + totalCantItems << "  time: " << b.elapsed() << std::endl;
   console.unlock();
   {
      auto fileFD = std::ifstream(seedsFileName);
      lengthsDB.ImportTextFromFile(fileFD);
   }
   {
      auto fileFD = std::ofstream(seedsFileName);
      lengthsDB.ExportTextToFile(fileFD);
   }
}
