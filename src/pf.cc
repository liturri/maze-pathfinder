#include "AStar.hxx"
#include "app_config.hxx"
#include "maze.hxx"
#include "seeds.hxx"
#include <benchmark.hxx>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <mutex>
#include <thread>
#include <vector>

std::mutex console;

MazeLengthsDB lengthsDB;

class PathFindAppConfig : public AppConfig
{
protected:
   std::string SeedFileName;
   TypeSeed countOfItems;

public:
   PathFindAppConfig(std::string name) : AppConfig(name){};
   void FillArgumentsList(argparse::ArgumentParser &appOptions) override
   {
      AppConfig::FillArgumentsList(appOptions);
      appOptions.add_argument("seeds_file").help("Seeds db filename").required();
      appOptions.add_argument("--count").help("Number of iterations to calculate").default_value(100);
   };
   void ProcessArguments(argparse::ArgumentParser &appOptions) override
   {
      AppConfig::ProcessArguments(appOptions);
      SeedFileName = appOptions.get<std::string>("seeds_file");
      if (appOptions.is_used("--count"))
         countOfItems = appOptions.get<int>("--count");
      else
         countOfItems = 100000;
   };
   std::string GetSeedFilename() { return SeedFileName; };
   TypeSeed GetCountOfItems() { return countOfItems; };
};

TypeLength CreateMaze(TypeSeed seed, int lines, int columns, int heuristic);
void workThread(TypeSeed from, TypeSeed to);

PathFindAppConfig appConfig("pathfind");

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
   TypeSeed totalCantItems = appConfig.GetCountOfItems();
   TypeSeed itemsPerThread = totalCantItems / processorsCount;

   if (appConfig.DebugMode())
   {
      std::cerr << "Seed: " << appConfig.GetSeed() << " Cols*Lines: " << appConfig.GetColumns() << '*' << appConfig.GetLines() << std::endl;
   }
   Benchmark b;

   // std::cerr.imbue(std::locale("es_AR.UTF-8"));
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
   auto t = b.elapsed();
   std::cerr << "Seeds: " << seed << '-' << seed + totalCantItems << " CPUs: " << processorsCount << " time: " << t
             << " Items p/sec: " << totalCantItems / t << " p/cpu: " << totalCantItems / t / processorsCount << std::endl;
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

TypeLength CreateMaze(TypeSeed seed, int lines, int columns, int heuristic)
{
   AStar::Generator generator;
   MazeGenRecursive gen(seed);
   MazeGenerator s(gen);
   std::string heuristicStr = "manhattan";

   mapType map{(std::size_t)columns, std::vector<char>((std::size_t)lines, '\0')};
   s.Generate(map);

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
      }
      // lengthsDB.IsEmpty(s, length);
      // std::cerr << "Len: " << length << " Seed: " << s << " Time: " << b.elapsed() << std::endl;
   }
}
