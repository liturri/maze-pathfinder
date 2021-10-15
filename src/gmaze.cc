#include <benchmark.hxx>
#include "AStar.hxx"
#include "maze.hxx"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <iostream>
#include <iomanip>
#include <cassert>

class boxx : public Fl_Box
{
public:
   boxx(const mapType &_map, const AStar::CoordinateList &_path) : Fl_Box(0, 0, 800, 800), map(_map), path(_path), sizeX(10), sizeY(10) {}

   void draw()
   {
      fl_rect(0, 0, (map.size() + 1) * sizeX, (map[0].size() + 1) * sizeY);
      for (const auto &coordinate : path)
      {
         if (coordinate.x > (int)map.size() ||
             coordinate.y > (int)map[0].size())
            continue;
         map[coordinate.x][coordinate.y] = '$';
      }
      for (size_t locX = 0; locX < map.size(); locX++)
      {
         for (size_t locY = 0; locY < map[0].size(); locY++)
         {
            char c = map[locX][locY];
            switch (c)
            {
            case '*':
               fl_rectf(locX * sizeX, locY * sizeY, sizeX + 1, sizeY + 1, FL_BLUE);
               break;
            case '$':
               fl_rectf(locX * sizeX + 2, locY * sizeY + 2, sizeX - 1, sizeY - 1, FL_YELLOW);
               // fl_circle(locX * sizeX + (sizeX/2), locY * sizeY + (sizeY/2), sizeX/2);
            case ' ':
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
   AStar::CoordinateList path;
   int sizeX;
   int sizeY;
};

int main(int argc, char **argv)
{
   AStar::Generator generator;
   Solution s;
   srand(3);
   int lines = 140;
   int columns = 90;

   mapType map{(std::size_t)lines, std::vector<char>((std::size_t)columns, '\0')};
   s.maze(map);

   generator.setWorldSize({lines, columns});
   if (argc == 2)
   {
      std::cout << "Heuristic: ";

      switch (argv[1][0])
      {
      case '1':
         std::cout << "euclidean";
         generator.setHeuristic(AStar::Heuristic::euclidean);
         break;
      case '2':
         std::cout << "octagonal";
         generator.setHeuristic(AStar::Heuristic::octagonal);
         break;
      case '3':
         std::cout << "manhattan";
         generator.setHeuristic(AStar::Heuristic::manhattan);
         break;
      };
      std::cout << std::endl;
   };
   // generator.setDiagonalMovement(true);
   auto worldSize = generator.getWorldSize();
   map[0][0] = ' ';
   map[lines - 1][columns - 1] = ' ';
   for (int l = 0; l < lines; l++)
   {
      for (int c = 0; c < columns; c++)
      {
         if (map[l][c] == '*')
            generator.addCollision({l, c});
      }
      // generator.addCollision({rand() % worldSize.x, rand() % worldSize.y});
   }

   Benchmark b;
   generator.findPath({0, 0}, {worldSize.x - 1, worldSize.y - 1});
   auto path = generator.getPath();
   std::cout << "Size: " << worldSize.x << 'x' << worldSize.y << "  Lenght: " << path.size() << "  Time: " << std::fixed << std::setprecision(4) << b.elapsed() << std::endl;

   Fl_Window *window = new Fl_Window(lines * 10 + 1, columns * 10 + 1);
   /* auto *box = */ new boxx(map, path);

   // Fl_Box *box = new Fl_Rect(20, 40, 300, 100, "Hello, World!");
   // box->box(FL_UP_BOX);
   // box->labelfont(FL_BOLD + FL_ITALIC);
   // box->labelsize(36);
   // box->labeltype(FL_SHADOW_LABEL);
   // window->end();
   // window->resizable(window);
   window->show(argc, argv);
   return Fl::run();
}
