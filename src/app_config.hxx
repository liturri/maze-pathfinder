#ifndef __APP_CONFIG_HXX__
#define __APP_CONFIG_HXX__
// #include <cstdlib>
#include <string>
#include "seeds.hxx"
#include <argparse/argparse.hpp>

class AppConfig
{
protected:
   int lines;
   int columns;
   int heuristic;
   TypeSeed seed;
   bool debugMode;

   std::string appName;
   argparse::ArgumentParser appOptions;

public:
   bool DebugMode() const { return debugMode; };
   AppConfig(std::string _appName) : appName(_appName), appOptions(_appName){};
   void LoadArguments(int argc, char *argv[]);

   virtual void FillArgumentsList();
   virtual void ProcessArguments();

   int GetLines() { return lines; };
   int GetColumns() { return columns; };
   int GetHeuristic() { return heuristic; };
   TypeSeed GetSeed() { return seed; };
};

#endif
