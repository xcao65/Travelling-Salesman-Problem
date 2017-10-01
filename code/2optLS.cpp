//
//  main.cpp
//  2optsolution
//
//  Created by Bravado on 12/1/16.
//  Copyright © 2016 Bravado. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include "common.h"
#define INFI 10000000;
using namespace std;
/*struct City {
    int city_id;
    double x, y;
};


//loading data
vector<City> createGraph(string inputFile){
    ifstream input;
    input.open(inputFile,ios::in);
    string s;
    
    //read the first five lines
    int i = 0;
    int numCities=0;
    while(i < 5){
        if(i == 2){
            string temp;
            input >> temp;
            input >> numCities;
        }
        getline(input,s);
        ++i;
    }
    
    City city;
    vector<City> graph;
    for(int i = 0; i < numCities; ++i) {
        input >> city.city_id >> city.x >> city.y;
        graph.push_back(city);
    }
    
    input.close();
    return graph;
}
*/
//Function to calculate the distance between cities
int City_distance(struct City city1, struct City city2)
{
    double x = city1.x-city2.x;
    double y = city1.y-city2.y;
    int dis = (int)(sqrt(x*x + y*y)+0.5);
    return dis ;
}


//creating distance matrix between different cities;
vector<int> create_DMatrix(vector<City>& graph){
    int numCities =(int)graph.size();
    
    //creat DMatrix
    vector<int> DMatrix(numCities*numCities);
    
    //using base number of cities to arrange matrix
    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            DMatrix[i*numCities+j] = City_distance(graph[i], graph[j]);
        }
    }
    for (int i=0; i<numCities; ++i) {
        DMatrix[i*numCities+i]=INFI;
    }
    
    return DMatrix;
}


//Randomly generate a tour

vector<City> generate_tour(vector<City> graph){
    vector<City> tour=graph;
    //randomly shuffle cities in graph
    random_shuffle(tour.begin(), tour.end());
    random_shuffle(tour.begin(), tour.end());
    random_shuffle(tour.begin(), tour.end());
    random_shuffle(tour.begin(), tour.end());
    City tour_start=tour[0];
    tour.push_back(tour_start);
    return tour;
}


//2-opt local search implementation

vector<City> opt_search(const vector<City> tour,const vector<int> distanceMatrix,string traceFile){
    vector<City> opt_tour=tour;
    vector<int> dm=distanceMatrix;
    int num=(int)tour.size()-1,mini=0,minj=0;
    int change=0,minchange=0;
    int olenth=INFI;
    auto t1 = chrono::high_resolution_clock::now();
    auto t2= chrono::high_resolution_clock::now();
    
    ofstream otracefile(traceFile);
    if (!otracefile) {
        cerr<<"open error!"<<endl;
        exit(1);
    }
    
    do{
        minchange=0;
        for (int i=0; i<num-1; ++i) {
            for (int j=i+2; j<num; ++j) {
                int disij=dm[num*(opt_tour[i].city_id-1)+(opt_tour[j].city_id-1)];
                int disi1j1=dm[num*(opt_tour[i+1].city_id-1)+(opt_tour[j+1].city_id-1)];
                int disii1=dm[num*(opt_tour[i].city_id-1)+(opt_tour[i+1].city_id-1)];
                int disjj1=dm[num*(opt_tour[j].city_id-1)+(opt_tour[j+1].city_id-1)];
                
                
                change=disij+disi1j1-disii1-disjj1;
                
                
                if (minchange>change) {
                    minchange=change;
                    mini=i;
                    minj=j;
                }
            }
        }
        //Swap mini minj;
        if (minchange<0) {
            City temp=opt_tour[mini+1];
            opt_tour[mini+1]=opt_tour[minj];
            opt_tour[minj]=temp;
            int tlenth=0;
            for(int i = 0; i < num; ++i) {
                tlenth += dm[num*(opt_tour[i].city_id-1)+(opt_tour[i+1].city_id-1)];
            }
            if (tlenth<olenth) {
                olenth=tlenth;
                t2 = chrono::high_resolution_clock::now();
                otracefile<<setprecision(2) << fixed <<
                chrono::duration<double>(t2-t1).count()<<",";
                otracefile<<tlenth<<endl;
            }
        }
        
    }while (minchange<0);
    otracefile.close();
    return opt_tour;
}



// Calculate the length of a tour
int tourLength(const vector<City>& tour,
               const vector<int>& distanceMatrix){
    int tlength = 0;
    int numCities = (int)tour.size()-1;
    for(int i = 0; i < tour.size()-1; ++i) {
        tlength += distanceMatrix[numCities*(tour[i].city_id-1)+(tour[i+1].city_id-1)];
    }
    return tlength;
}



//output optimal tour strategy
void opt_tour_output(const vector<City> &opt_tour,string outputFile,vector<int> dismatrix){
    ofstream outfile(outputFile,ios::out);
    if (!outfile) {
        cerr<<"open error!"<<endl;
        exit(1);
    }
    int quality;
    quality=tourLength(opt_tour, dismatrix);
    outfile<<quality<<endl;
    int i=0;
    int numCities=(int)opt_tour.size()-1;
    for (i=0; i<numCities; ++i) {
        int distance=City_distance(opt_tour[i], opt_tour[i+1]);
        outfile<<opt_tour[i].city_id<<' '<<opt_tour[i+1].city_id<<' '<<distance<<endl;
    }
    outfile.close();
}

int wrappedOptTour(std::string input, std::string algo, int cutoff_time, 
		   int seed){
  srand(seed);
  vector<City> graph,tour,opt_tour;
  graph=createGraph(input);
  vector<int> dismatrix;
  dismatrix=create_DMatrix(graph);
  tour=generate_tour(graph);
  std::size_t found = input.find_last_of("/\\");
  std::string strippedInstance = input.substr(found+1);
  strippedInstance = strippedInstance.substr(0,strippedInstance.length()-4);
  std::string fileName = strippedInstance+'_'+algo+'_'+std::to_string
    (static_cast<long long>(cutoff_time))+'_'+std::to_string(static_cast<long long>(seed));
  opt_tour=opt_search(tour, dismatrix,fileName+".trace");
  
  opt_tour_output(opt_tour,fileName+".sol", dismatrix);
  
  return 0;
}

/*int main(int argc, const char * argv[]) {
    vector<City> graph,tour,opt_tour;
    graph=createGraph("/Users/bravado/Downloads/DATA/UMissouri.tsp");
    vector<int> dismatrix;
    dismatrix=create_DMatrix(graph);
    tour=generate_tour(graph);

    
    opt_tour=opt_search(tour, dismatrix,"/Users/bravado/Downloads/TSP_OUTPUT_TRACE/UMissouri.trace");
   
    
    
    opt_tour_output(opt_tour,"/Users/bravado/Downloads/TSP_OUTPUT/UMissouri.tsp",dismatrix);
    
    
    
    return 0;
    }*/
