/*
    Copyright (c) 2015, Damian Barczynski <daan.net@wp.eu>
    Following tool is licensed under the terms and conditions of the ISC license.
    For more information visit https://opensource.org/licenses/ISC.
*/
#ifndef __ASTAR_HPP_8F637DB91972F6C878D41D63F7E7214F__
#define __ASTAR_HPP_8F637DB91972F6C878D41D63F7E7214F__

#include <vector>
#include <functional>
//#include <set>

namespace AStar
{

   struct Vec2i
   {
      int x, y;

      bool operator==(const Vec2i &coordinates_);
   };

   Vec2i operator+(const Vec2i &left_, const Vec2i &right_);

   using uint = unsigned int;
   using HeuristicFunction = std::function<uint(Vec2i, Vec2i)>;
   using CoordinateList = std::vector<Vec2i>;

   struct Node
   {
      uint G, H;
      Vec2i coordinates;
      Node *parent;

      Node(Vec2i coord_, Node *parent_ = nullptr);
      uint getScore();
   };

   using NodeSet = std::vector<Node *>;

   class Generator
   {
      bool detectCollision(Vec2i coordinates_);
      Node *findNodeOnList(NodeSet &nodes_, Vec2i coordinates_);
      void releaseNodes(NodeSet &nodes_);

   public:
      Generator();
      void setWorldSize(Vec2i worldSize_);
      void setDiagonalMovement(bool enable_);
      void setHeuristic(HeuristicFunction heuristic_);
      // void findPath(Vec2i source_, Vec2i target_);
      template<typename T>
      void findPath(Vec2i source_, Vec2i target_, T F)
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
            
            path.clear();
            auto tmpCurrent = current;
            while (tmpCurrent != nullptr)
            {
               path.push_back(tmpCurrent->coordinates);
               tmpCurrent = tmpCurrent->parent;
            }
            F();
            
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

      CoordinateList getPath() { return path; };
      void addCollision(Vec2i coordinates_);
      void removeCollision(Vec2i coordinates_);
      void clearCollisions();
      Vec2i getWorldSize() const { return worldSize; };
      const CoordinateList getWalls() const { return walls; };
      void showMaze();

   private:
      HeuristicFunction heuristic;
      CoordinateList direction, walls;
      Vec2i worldSize;
      uint directions;
      CoordinateList path;
   };

   class Heuristic
   {
      static Vec2i getDelta(Vec2i source_, Vec2i target_);

   public:
      static uint manhattan(Vec2i source_, Vec2i target_);
      static uint euclidean(Vec2i source_, Vec2i target_);
      static uint octagonal(Vec2i source_, Vec2i target_);
   };
}

#endif // __ASTAR_HPP_8F637DB91972F6C878D41D63F7E7214F__
