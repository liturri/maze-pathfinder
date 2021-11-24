#include "AStar.hxx"
#include "maze.hxx"
#include <benchmark/benchmark.h>

static void f1(benchmark::State &state)
{
   for (auto _ : state)
   {
      constexpr auto columns = 61;
      constexpr auto lines = 61;

      AStar::Generator generator;
      MazeGenRecursive gen(0);
      MazeGenerator s(gen);

      mapType map{(std::size_t)columns, std::vector<char>((std::size_t)lines, '\0')};
      s.Generate(map);

      generator.setWorldSize({columns, lines});
      map[columns - 1][lines - 1] = ' ';
      for (int l = 0; l < columns; l++)
      {
         for (int c = 0; c < lines; c++)
         {
            if (map[l][c] == '*')
               generator.addCollision({l, c});
         }
      }
      // generator.findPath({0, 0}, {columns - 1, lines - 1});
      // auto path = generator.getPath();
   }
}

static void f2(benchmark::State &state)
{
   for (auto _ : state)
   {
      constexpr auto columns = 63;
      constexpr auto lines = 63;

      AStar::Generator generator;
      MazeGenRecursive gen(0);
      MazeGenerator s(gen);

      mapType map{(std::size_t)columns, std::vector<char>((std::size_t)lines, '\0')};
      s.Generate(map);

      generator.setWorldSize({columns, lines});
      map[columns - 1][lines - 1] = ' ';
      for (int l = 0; l < columns; l++)
      {
         for (int c = 0; c < lines; c++)
         {
            if (map[l][c] == '*')
               generator.addCollision({l, c});
         }
      }
      generator.findPath({0, 0}, {columns - 1, lines - 1});
      auto path = generator.getPath();
   }
}

BENCHMARK(f1);
BENCHMARK(f2);

BENCHMARK_MAIN();