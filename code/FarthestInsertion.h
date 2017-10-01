//Farthest insertion header file

#ifndef FARTHESTINSERTION_H
#define FARTHESTINSERTION_H

#include <vector>

class Farthest_Insert
{
 public :
  std::vector<std::vector<int>> ReadMap(std::string instance);
  int Farthest_Node(std::vector<int>& key, bool path_Judge[]);
  int Select_Node(std::vector<std::vector<int>>& Matrix, std::vector<int>& path);
  int Insert_Node(std::vector<std::vector<int>>& Matrix, std::vector<int>& path, int r);
};

void FarthestInsertion(std::string instance);

#endif
