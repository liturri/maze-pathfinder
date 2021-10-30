#include <benchmark.hxx>
#include "AStar.hxx"
#include "maze.hxx"
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <thread>
#include "app_config.hxx"

class GMazeAppConfig : public AppConfig
{
protected:
   int sizeX;
   int sizeY;
   bool animated;

public:
   GMazeAppConfig(std::string name) : AppConfig(name){};
   void FillArgumentsList() override
   {
      AppConfig::FillArgumentsList();
      appOptions.add_argument("-x", "--sizex").help("Number of pixels per column").default_value(10).scan<'i', int>();
      appOptions.add_argument("-y", "--sizey").help("Number of pixels per line").default_value(10).scan<'i', int>();
      appOptions.add_argument("-a", "--animated").help("Animated display").default_value(false).implicit_value(true);
   };
   void ProcessArguments() override
   {
      AppConfig::ProcessArguments();
      sizeX = appOptions.get<int>("--sizex");
      sizeY = appOptions.get<int>("--sizey");
      animated = appOptions.get<bool>("--animated");
   };
   bool GetAnimated() { return animated; };
   int GetSizeX() { return sizeX; };
   int GetSizeY() { return sizeY; };
};

GMazeAppConfig appConfig("gmaze");

class boxx : public Fl_Box
{
public:
   boxx(const mapType &_map, AStar::Generator &_generator, int _sizeX, int _sizeY)
       : Fl_Box(0, 0, 800, 800), map(_map), generator(_generator), sizeX(_sizeX), sizeY(_sizeY) {}

   void draw()
   {
      auto path = generator.getPath();
      for (const auto &coordinate : path)
      {
         if (coordinate.x > (int)map.size() ||
             coordinate.y > (int)map[0].size())
            continue;
         map[coordinate.x][coordinate.y] = '$';
      }
      for (size_t locY = 0; locY < map[0].size(); locY++)
      {
         for (size_t locX = 0; locX < map.size(); locX++)
         {
            char c = map[locX][locY];

            switch (c)
            {
            case '*':
               fl_rectf(locX * sizeX, locY * sizeY, sizeX + 1, sizeY + 1, FL_BLUE);
               break;
            case '$':
               map[locX][locY] = ' ';
               fl_rectf(locX * sizeX + 2, locY * sizeY + 2, sizeX - 1, sizeY - 1, FL_RED);
               break;
               // fl_circle(locX * sizeX + (sizeX/2), locY * sizeY + (sizeY/2), sizeX/2);
            case ' ':
               fl_rectf(locX * sizeX, locY * sizeY, sizeX + 1, sizeY + 1, FL_WHITE);
               break;
            default:
               fl_rectf(locX * sizeX, locY * sizeY, sizeX + 1, sizeY + 1, FL_RED);
               break;
            }
         }
      }
      fl_rect(0, 0, (map.size()) * sizeX + 1, (map[0].size()) * sizeY + 1, FL_BLACK);
   }

private:
   mapType map;
   AStar::Generator &generator;
   int sizeX;
   int sizeY;
};

class WorkThread
{
   class refresh
   {
   public:
      refresh(AStar::Generator &_generator, Fl_Double_Window *_widget) : generator(_generator), widget(_widget){};
      void operator()()
      {
         std::this_thread::sleep_for(std::chrono::milliseconds(1));
         // std::cout << "\033[2J";
         // std::cout.flush();
         // generator.showMaze();
         Fl::lock();
         widget->redraw();
         Fl::check();
         Fl::unlock();
         Fl::awake();
      };

   protected:
      AStar::Generator &generator;
      Fl_Double_Window *widget;
   };

public:
   WorkThread(AStar::Generator &_generator, Fl_Double_Window *_widget) : generator(_generator), widget(_widget){};
   void operator()(void)
   {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      std::cout << "Work start." << std::endl;
      auto worldSize = generator.getWorldSize();
      Benchmark b;
      if (appConfig.GetAnimated())
      {
         refresh fun(generator, widget);
         generator.findPath({0, 0}, {worldSize.x - 1, worldSize.y - 1}, fun);
      }
      else
      {
         generator.findPath({0, 0}, {worldSize.x - 1, worldSize.y - 1});
      }
      std::cout << "Work end: " << b.elapsed() << " Steps: " << generator.getPath().size() << std::endl;
      Fl::lock();
      widget->redraw();
      Fl::check();
      Fl::unlock();
      Fl::awake();
   }

protected:
   AStar::Generator &generator;
   Fl_Double_Window *widget;
};

void ResolveMaze(int seed, int lines, int columns, int scaleX, int scaleY, int heuristic)
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
   map[0][0] = ' ';
   map[columns - 1][lines - 1] = ' ';
   for (int l = 0; l < columns; l++)
   {
      for (int c = 0; c < lines; c++)
      {
         if (map[l][c] == '*')
            generator.addCollision({l, c});
      }
   }

   // Benchmark b;
   // generator.findPath({0, 0}, {worldSize.x - 1, worldSize.y - 1});
   // auto path = generator.getPath();
   // std::cout << "Size: " << worldSize.x << 'x' << worldSize.y << "  Length: " << path.size() << "  Time: " << std::fixed << std::setprecision(4) << b.elapsed() << std::endl;
   Fl_Double_Window *window = new Fl_Double_Window(columns * scaleX + 1, lines * scaleY + 1);
   /* auto *box =  */ new boxx(map, generator, scaleX, scaleY);

   window->show();

   WorkThread workThread(generator, window);

   std::thread th(workThread);
   Fl::run();

   th.detach();
}

int main(int argc, char **argv)
{
   appConfig.LoadArguments(argc, argv);
   // int seed = atoi(argv[1]);
   // int heuristic = atoi(argv[2]);
   // int scaleX = 10;
   // int scaleY = 10;
   // int columns = 111 /* 1670 / scaleX */;
   // int lines = 66 /* 1000 / scaleY */;

   ResolveMaze(appConfig.GetSeed(), appConfig.GetLines(), appConfig.GetColumns(), appConfig.GetSizeX(), appConfig.GetSizeY(),
               appConfig.GetHeuristic());

   return 0;
}
