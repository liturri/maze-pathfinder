// #include <string>
// #include <fstream>
// #include <iostream>
// #include <sstream>
// #include <errno.h>
#include <gzstream.h>
// #include <vector>
// #include <algorithm>
#include <benchmark.hxx>
#include "seeds.hxx"
#include <locale>

// std::vector<std::string> split(const std::string &text, char delimiter)
// {
//    std::string tmp;
//    std::vector<std::string> stk;
//    std::stringstream ss(text);
//    while (std::getline(ss, tmp, delimiter))
//    {
//       if (tmp.size() != 0)
//       {
//          stk.push_back(tmp);
//       }
//    }
//    return stk;
// }

int main()
{
   Benchmark b;
   MazeSeedsDB seedsDB;
   std::cerr.imbue(std::locale(""));

   // size_t importedItems;
   {
      auto fileFD = igzstream("/tmp/bench1.gz");
      seedsDB.ImportBinFromFile(fileFD);
   }
   // {
   //    auto fileFD = igzstream("/tmp/bench.gz");
   //    seedsDB.ImportTextFromFile(fileFD);
   // }
   // {
   //    auto fileFD = std::ifstream("/tmp/bench2");
   //    importedItems = seedsDB.ImportTextFromFile(fileFD);
   // }
   // {
   //    auto fileFD = std::ifstream("/tmp/bench3");
   //    importedItems += seedsDB.ImportTextFromFile(fileFD);
   // }
   // if (importedItems > 0)
   // {
   //    auto fileFD = ogzstream("/tmp/bench1.gz");
   //    seedsDB.ExportBinToFile(fileFD);
   // }
   // seedsDB.ShowListStatus();
   seedsDB.SumarizeLengths();
   return 0;
};