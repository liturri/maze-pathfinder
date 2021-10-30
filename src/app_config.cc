#include "app_config.hxx"
#include <string>

void AppConfig::FillArgumentsList(argparse::ArgumentParser &appOptions)
{
   appOptions.add_argument("-s", "--seed").help("Random number generator seed").default_value(0).scan<'i', long>();
   appOptions.add_argument("-l", "--lines").help("Number of lines").default_value(66).scan<'i', int>();
   appOptions.add_argument("-c", "--columns").help("Number of columns").default_value(110).scan<'i', int>();
   appOptions.add_argument("-e", "--heuristic").help("Heuristic: 1-euclidean, 2-octagonal 3-manhattan").default_value(3).scan<'i', int>();
   appOptions.add_argument("-d", "--verbose").help("Debug mode").default_value(false).implicit_value(true);
};

void AppConfig::ProcessArguments(argparse::ArgumentParser &appOptions)
{
   columns = appOptions.get<int>("--columns");
   lines = appOptions.get<int>("--lines");
   heuristic = appOptions.get<int>("--heuristic");
   seed = appOptions.get<long>("--seed");
   debugMode = appOptions.get<bool>("--verbose") == true;
}

void AppConfig::LoadArguments(int argc, char *argv[])
{
   argparse::ArgumentParser appOptions(appName);
   FillArgumentsList(appOptions);
   try
   {
      appOptions.parse_args(argc, argv);
   }
   catch (const std::runtime_error &err)
   {
      std::cout << err.what() << std::endl;
      std::cout << appOptions;
      std::exit(0);
   }

   ProcessArguments(appOptions);
};
