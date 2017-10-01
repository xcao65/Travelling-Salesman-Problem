// Jonathan Kho
/* SimulatedAnnealing.cpp solves the traveling salesperson problem
   using simulated annealing with reheating.*/

#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include "common.h"

// Calculate the length of a tour
int tourLength(const std::vector<std::pair<int,int> >& tour, 
	       const std::vector<int>& distanceMatrix){ 
  int resultLength = 0;
  int numCities = tour.size();
  for(int i = 0; i < tour.size(); ++i) {
    resultLength += distanceMatrix[numCities*tour[i].first+tour[i].second];
  }
  return resultLength;
}

std::vector<int> createDistanceMatrix(std::vector<City>& graph){
  int numCities = graph.size(); 
  std::vector<int> distanceMatrix(numCities*numCities);
  // Use city coordinates to calculate the distance matrix
  for (int i = 0; i < numCities; ++i) { 
    for (int j = 0; j < numCities; ++j) {
      distanceMatrix[i*numCities+j] = Dis_Cal(graph[i], graph[j]);
    }
  }
  return distanceMatrix;
}
  
// Find approximate TSP using simulated annealing and 2 exchange
std::vector<std::pair<int,int>> simulatedAnnealing(std::vector<City>& graph,
						   std::vector<int>& distanceMatrix, std::string input, std::string algo, int cutoff_time, int seed){

  int numCities = graph.size();

  std::vector<City> shuffledCities = graph; 
  // Initialize a tour randomly connecting the cities
  // To-do: Initialize a tour such  that it could logically cause the
  // algorithm to converge quicker or with better quality 
  std::random_shuffle(shuffledCities.begin(), shuffledCities.end()); 

  std::vector<std::pair<int, int>> tour;
  // Connect an initial tour in that order
  for (int i = 0; i < numCities; ++i){ 
    if (i == numCities-1)
      tour.push_back(std::make_pair(shuffledCities[i].city_id-1,
				    shuffledCities[0].city_id-1));
    else
      tour.push_back(std::make_pair(shuffledCities[i].city_id-1,
				    shuffledCities[i+1].city_id-1));
  }

  std::vector<std::pair<int, int>> bestTour = tour;
  // Store the best tour so the local search tour quality can fluctuate
  // without ending at a lower quality than the best observed quality
  int bestTourLength = tourLength(bestTour, distanceMatrix);
  int startTemp = numCities*200; // To-do: tune this heuristic
  int tempOfBest = startTemp; // Should accept nearly all potential
			      // solutions at the beginning
  auto t1 = std::chrono::high_resolution_clock::now();
  std::size_t found = input.find_last_of("/\\");
  std::string strippedInstance = input.substr(found+1);
  strippedInstance = strippedInstance.substr(0,strippedInstance.length()-4);
  std::string fileName = strippedInstance+'_'+algo+'_'+std::to_string(static_cast<long long>(cutoff_time))+'_'+std::to_string(static_cast<long long>(seed));
  std::ofstream trace (fileName+".trace");
  std::ofstream tourOut (fileName+".sol");
  double temperature = startTemp;
  double coolingRate = 0.98; // Geometrically decrease the temperature
  int maxReheats = 5, minTemp = 10, maxIterPerTemp = 6000, reheatIter = 0;
  double theTime = 0.0;
  while (reheatIter < maxReheats && (theTime < cutoff_time)) {
    while (temperature > minTemp) {
      int iterAtTemp = 0;
      while (iterAtTemp < maxIterPerTemp) {
	int a = rand()%numCities; // Pick two edges to 2-exchange
	int b = rand()%numCities;
	std::vector<std::pair<int, int> > tempTour = tour;
	
	int min = a;
	int max = b;
	if (a > b) {
	  min = b;
	  max = a;
	}
	if (min != max && (min != max-1)){
	  int temp = tempTour[min].second; // Do the 2-exchange
	  tempTour[min].second = tempTour[max].first;
	  tempTour[max].first = temp;
	  // Arrange the edges correctly
	  std::reverse(tempTour.begin()+min+1, tempTour.begin()+max); 
	  for (int i = min+1; i < max ; ++i){
	    int temp = tempTour[i].second;
	    tempTour[i].second = tempTour[i].first;
	    tempTour[i].first = temp;
	  }
	  
	  int tempTourLength = tourLength(tempTour, distanceMatrix);
	  int saTourLength = tourLength(tour, distanceMatrix);
	  // Calculate difference between new and old tours
	  int delta = tempTourLength - saTourLength; 
	  // If the new tour is better than the old tour take it as the
	  // new best tour 
	  if (delta < 0) { 
	    tour = tempTour;
	    saTourLength = tempTourLength;
	  }
	  else {
	    double probability = (double)rand() / RAND_MAX;
	    // If the new tour is worse than the old tour, take it with
	    // probability e^(-delta/T) 
	    if (probability <= exp(-delta/temperature)){ 
	      tour = tempTour;
	      saTourLength = tempTourLength;
	    }
	  }
	  if (saTourLength < bestTourLength) {
	    bestTour = tour;
	    bestTourLength = saTourLength;
	    tempOfBest = temperature;
	    reheatIter = 0;
	    auto t2 = std::chrono::high_resolution_clock::now();
	    theTime = std::chrono::duration<double>(t2-t1).count();
	    if (trace.is_open()) {
	      trace << std::setprecision(2) << std::fixed <<
		std::chrono::duration<double>(t2-t1).count() << ", " <<
		bestTourLength << std::endl;
	    }
	  }
	  auto t2 = std::chrono::high_resolution_clock::now();
	  theTime = std::chrono::duration<double>(t2-t1).count();
	}
	++iterAtTemp;
      }
      temperature *= coolingRate;
    }
    ++reheatIter;
    temperature = 60 * tempOfBest;
    std::cout << tempOfBest << std::endl;
    tour = bestTour;
  }
  trace.close();

  //std::cout << "tempOfBest\t" << tempOfBest << std::endl;
  int result = bestTourLength;
  tourOut << result << std::endl;
  for(int i = 0; i < bestTour.size(); ++i){
    tourOut << bestTour[i].first << ' ' << bestTour[i].second << ' ' <<
      distanceMatrix[numCities*bestTour[i].first+bestTour[i].second] <<
      std::endl;
  }
  tourOut.close();
    
  return bestTour;
}

int wrappedSimulatedAnnealing(std::string input, std::string algo, int cutoff_time, int seed){
  srand(seed);
  std::vector<City> graph = createGraph(input);
  std::vector<int> distanceMatrix = createDistanceMatrix(graph);
  auto t1 = std::chrono::high_resolution_clock::now();
  std::vector<std::pair<int,int>> saTour = simulatedAnnealing(graph, distanceMatrix, input, algo, cutoff_time, seed);
  auto t2 = std::chrono::high_resolution_clock::now();
  //std::cout << std::setprecision(2) << std::fixed <<
  //  std::chrono::duration<double>(t2-t1).count() << ", " <<
  //  tourLength(saTour, distanceMatrix) << std::endl;
  return 0;
}

