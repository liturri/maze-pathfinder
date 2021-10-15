#include <chrono>
#include <thread>
#include "AStar.hxx"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>


using namespace std::placeholders;

bool AStar::Vec2i::operator==(const Vec2i &coordinates_)
{
   return (x == coordinates_.x && y == coordinates_.y);
}

AStar::Vec2i operator+(const AStar::Vec2i &left_, const AStar::Vec2i &right_)
{
   return {left_.x + right_.x, left_.y + right_.y};
}

AStar::Node::Node(Vec2i coordinates_, Node *parent_)
{
   parent = parent_;
   coordinates = coordinates_;
   G = H = 0;
}

AStar::uint AStar::Node::getScore()
{
   return G + H;
}

AStar::Generator::Generator()
{
   setDiagonalMovement(false);
   setHeuristic(&Heuristic::manhattan);
   direction = {
       {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
}

void AStar::Generator::setWorldSize(Vec2i worldSize_)
{
   worldSize = worldSize_;
}

void AStar::Generator::setDiagonalMovement(bool enable_)
{
   directions = (enable_ ? 8 : 4);
}

void AStar::Generator::setHeuristic(HeuristicFunction heuristic_)
{
   heuristic = std::bind(heuristic_, _1, _2);
}

void AStar::Generator::addCollision(Vec2i coordinates_)
{
   assert(coordinates_.x < worldSize.x && coordinates_.y < worldSize.y);
   walls.push_back(coordinates_);
}

void AStar::Generator::removeCollision(Vec2i coordinates_)
{
   auto it = std::find(walls.begin(), walls.end(), coordinates_);
   if (it != walls.end())
   {
      walls.erase(it);
   }
}

void AStar::Generator::clearCollisions()
{
   walls.clear();
}

void AStar::Generator::findPath(Vec2i source_, Vec2i target_)
{
   Node *current = nullptr;
   NodeSet openSet, closedSet;
   // openSet.reserve(100);
   // closedSet.reserve(100);
   openSet.push_back(new Node(source_));

   while (!openSet.empty())
   {
      auto current_it = openSet.begin();
      current = *current_it;

      for (auto it = openSet.begin(); it != openSet.end(); it++)
      {
         auto node = *it;
         if (node->getScore() <= current->getScore())
         {
            current = node;
            current_it = it;
         }
      }

      if (current->coordinates == target_)
      {
         break;
      }

      closedSet.push_back(current);
      openSet.erase(current_it);

      for (uint i = 0; i < directions; ++i)
      {
         Vec2i newCoordinates(current->coordinates + direction[i]);
         if (detectCollision(newCoordinates) ||
             findNodeOnList(closedSet, newCoordinates))
         {
            continue;
         }

         uint totalCost = current->G + ((i < 4) ? 10 : 14);

         Node *successor = findNodeOnList(openSet, newCoordinates);
         if (successor == nullptr)
         {
            successor = new Node(newCoordinates, current);
            successor->G = totalCost;
            successor->H = heuristic(successor->coordinates, target_);
            openSet.push_back(successor);
         }
         else if (totalCost < successor->G)
         {
            successor->parent = current;
            successor->G = totalCost;
         }
      }

      // CoordinateList path;
      // path.clear();
      // auto tmpCurrent = current;
      // while (tmpCurrent != nullptr)
      // {
      //    path.push_back(tmpCurrent->coordinates);
      //    tmpCurrent = tmpCurrent->parent;
      // }
      // std::cout << "\033[2J";
      // std::cout.flush();
      // showMaze();
      // std::this_thread::sleep_for(std::chrono::milliseconds(50));
   }

   auto tmpCurrent = current;
   path.clear();
   while (tmpCurrent != nullptr)
   {
      path.push_back(tmpCurrent->coordinates);
      tmpCurrent = tmpCurrent->parent;
   }
   releaseNodes(openSet);
   releaseNodes(closedSet);
   // return path;
}

AStar::Node *AStar::Generator::findNodeOnList(NodeSet &nodes_, Vec2i coordinates_)
{
   for (const auto &node : nodes_)
   {
      if (node->coordinates == coordinates_)
      {
         return node;
      }
   }
   return nullptr;
}

void AStar::Generator::releaseNodes(NodeSet &nodes_)
{
   for (auto it = nodes_.begin(); it != nodes_.end();)
   {
      delete *it;
      it = nodes_.erase(it);
   }
}

bool AStar::Generator::detectCollision(Vec2i coordinates_)
{
   if (coordinates_.x < 0 || coordinates_.x >= worldSize.x ||
       coordinates_.y < 0 || coordinates_.y >= worldSize.y ||
       std::find(walls.begin(), walls.end(), coordinates_) != walls.end())
   {
      return true;
   }
   return false;
}

void AStar::Generator::showMaze()
{
   // auto worldSize = getWorldSize();
   std::vector<std::vector<int>> map{(size_t)worldSize.x, std::vector<int>((size_t)worldSize.y, 0)};
   for (const auto &coordinate : walls)
   {
      map[coordinate.x][coordinate.y] = 1;
   }
   for (const auto &coordinate : path)
   {
      map[coordinate.x][coordinate.y] = 2;
   }

   std::cout << '+' << std::string(worldSize.y * 2, '-') << "+\n";
   for (const auto &row : map)
   {
      std::cout << '|';
      for (const auto &col : row)
      {
         char c = col == 0 ? ' ' : col == 1 ? '#'
                                            : '.';
         std::cout << c << c;
      }
      std::cout << "|\n";
   }
   std::cout << '+' << std::string(worldSize.y * 2, '-') << '+' << std::endl;
}

AStar::Vec2i AStar::Heuristic::getDelta(Vec2i source_, Vec2i target_)
{
   return {abs(source_.x - target_.x), abs(source_.y - target_.y)};
}

AStar::uint AStar::Heuristic::manhattan(Vec2i source_, Vec2i target_)
{
   auto delta = getDelta(source_, target_);
   return static_cast<uint>(10 * (delta.x + delta.y));
}

AStar::uint AStar::Heuristic::euclidean(Vec2i source_, Vec2i target_)
{
   auto delta = getDelta(source_, target_);
   return static_cast<uint>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}

AStar::uint AStar::Heuristic::octagonal(Vec2i source_, Vec2i target_)
{
   auto delta = getDelta(source_, target_);
   return 10 * (delta.x + delta.y) + (-6) * std::min(delta.x, delta.y);
}
