// Common TSP functions

#ifndef COMMON_H
#define COMMON_H

struct City {
  int city_id;
  double x, y;
}city;

//Function to calculate the distance between cities
int Dis_Cal(struct City city1, struct City city2)
{
    double x = city1.x-city2.x;
    double y = city1.y-city2.y;
    int dis = (int)(sqrt(x*x + y*y)+0.5);
    return dis ;
}

std::vector<City> createGraph(std::string inputFile){
  std::ifstream input;
  input.open(inputFile.c_str());
  std::string s;
    
  //read the first five lines
  int i = 0;
  std::size_t numCities;
  while(i < 5){
    if(i == 2){
      std::string temp;
      input >> temp;
      input >> numCities;
    }
    std::getline(input,s);
    ++i;
  }

  City city;
  std::vector<City> graph;
  for(int i = 0; i < numCities; ++i) {
    input >> city.city_id >> city.x >> city.y;
    graph.push_back(city);
  }

  input.close();
  return graph;
}

#endif
