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
   ReadData data;
   size_t importedItems;
   {
      auto fileFD = igzstream("/tmp/bench1.gz");
      data.ImportBinFromFile(fileFD);
   }
   // {
   //    auto fileFD = igzstream("/tmp/bench.gz");
   //    data.ImportTextFromFile(fileFD);
   // }
   {
      auto fileFD = std::ifstream("/tmp/bench3");
      importedItems = data.ImportTextFromFile(fileFD);
   }
   if (importedItems > 0)
   {
      auto fileFD = ogzstream("/tmp/bench1.gz");
      data.ExportBinToFile(fileFD);
   }
   data.ShowListStatus();
   data.SumarizeLengths();
   return 0;
};