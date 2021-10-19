#include <string>
#include <fstream>
#include <iostream>
#include <errno.h>
#include <gzstream.h>
#include <vector>
#include <benchmark.hxx>

class dataFile
{
private:
   igzstream gz;
   int seed;
   std::string size;
   int length;
   float time;

public:
   int GetSeed() const { return seed; }
   std::string GetSize() const { return size; }
   float GetTime() const { return time; };
   int GetLength() const { return length; };

   dataFile(const char *fileName)
   {
      gz.open(fileName);
      if (!gz.good())
      {
         std::cerr << errno << ": " << fileName << std::endl;
      }
   };
   std::istream &readLine()
   {
      // std::string null;
      // Heuristic: manhattan Seed: 2239643 Size: 111x66  Lenght: 218  Time: 0.0002
      // return gz >> null >> null >> null >> seed >> null >> size >> null >> length >> null >> time;
      return gz >> length >> seed;
   }
};

int main()
{
   Benchmark b;
   std::vector<std::vector<int>> seedsList{};
   dataFile data = ("/tmp/bench.gz");
   std::string maxCommon="";
   int maxCommonLen=0;
   size_t len;
   while (data.readLine())
   {
      len=data.GetLength();
      if (len >= seedsList.size())
      {
         // std::cout << "Seed: " << data.GetSeed() << "  Len: " << data.GetLength() << "  ListSize: " << seedsList.size() << "  List Capacity: " << seedsList.capacity() << std::endl;
         seedsList.resize(len + 1);
      }
      seedsList[len].push_back(data.GetSeed());
   };
   int cantItems=0;
   int tmpSize;
   for (int pos = 0; (size_t) pos < seedsList.size(); pos++)
   {
      tmpSize = seedsList[pos].size();
      if (tmpSize == 0)
         continue;
      cantItems += tmpSize;
      if (tmpSize > maxCommonLen)
      {
         maxCommonLen = tmpSize;
         maxCommon = std::to_string(pos);
      } else if (tmpSize == maxCommonLen)
      {
         maxCommon = maxCommon + ", "+ std::to_string(pos);
      }
      // std::cout << "Len: " << pos << " Items:" << tmpSize << std::endl;
   }
   std::cout << "Len:" << maxCommon << " Items: " << maxCommonLen << std::endl;
   std::cout << "Cant Items: " << cantItems << std::endl;
   std::cout << "Time elapsed: " << b.elapsed() << std::endl;
   return 0;
};