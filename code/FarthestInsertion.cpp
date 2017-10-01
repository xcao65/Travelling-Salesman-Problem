//
//  FarthestInsertion.cpp
//  6140_Project
//
//  Created by leyna on 12/2/16.
//  Copyright © 2016 leyna. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include <limits>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdint>
#include "FarthestInsertion.h"
#include "common.h"

using namespace std;

//Function to create an adjacent matrix
vector<vector<int>> Farthest_Insert::ReadMap(string instance)
{
    string s;
    vector<City> graph = createGraph(instance);
    unsigned long int dimension = graph.size();
    
    //store the edge between cities as matric
    vector<vector<int>> Matrix(dimension+1,vector<int>(dimension+1));
    for (int m = 0; m <= dimension; m++)
    {
        for (int n = 0; n <= dimension; n++)
        {
            if(m==0)
                Matrix[m][n] = n;
            else if(n==0)
                Matrix[m][n] = m;
            else if(m==n)
                Matrix[m][n] = INT32_MAX;
            else
                Matrix[m][n]=Dis_Cal(graph.at(m-1), graph.at(n-1));
        }
    }
    return Matrix;
}

//Function to find a farthest node from a known path
int Farthest_Insert::Farthest_Node(vector<int>& key, bool path_Judge[])
{
    unsigned long int dimension = key.size();
    int max = INT32_MIN;
    int max_index = 0 ;
    
    for(int i = 1; i < dimension ; i ++)
    {
        if(path_Judge[i] == false && key[i]>max)
        {
            max = key[i];
            max_index = i;
        }
    }
    return max_index;
}

//Function to find the position to insert the node
int Farthest_Insert::Select_Node(vector<vector<int>>& Matrix, vector<int>& path)
{
    unsigned long int dimension = Matrix.size();
    int i;
    int count;
    int temp;
    int node_from,node_to;
    int dis_max = INT32_MIN;
    vector<int> key;
    
    //initialize the path that has already covered
    bool path_Judge[dimension] ;
    for (i=0; i<dimension; i++)
    {
        path_Judge[i] = false;
    }
    for (i = 0; i<path.size(); i++)
    {
        count = path.at(i);
        path_Judge[count] = true;
    }
    path_Judge[0] = true;
    
    for(i = 0; i<path.size(); i++)
    {
        key = Matrix.at(path.at(i));
        temp = Farthest_Node(key,path_Judge);
        if(key[temp] > dis_max)
        {
            dis_max = key[temp];
            node_to = temp;
            node_from = path.at(i);
        }
    }
    return node_to;
}

//Function to insert the node into the path
int Farthest_Insert::Insert_Node(vector<vector<int>> &Matrix, vector<int> &path, int r)
{
    int temp = INT32_MAX;
    int dis_min = INT32_MAX;
    int node_from,node_to;
    int insert_from = path.at(0);
    unsigned long int dimension = path.size()-1;
    int i;
    for(i=0; i<dimension; i++)
    {
        node_from = path[i];
        node_to = path[i+1];
        temp = Matrix[node_from][r]+Matrix[r][node_to] - Matrix[node_from][node_to];
        if (temp< dis_min)
        {
            dis_min = temp;
            insert_from = i+1;
        }
    }
    return insert_from;
}

//Function to start the farthest insertion
void FarthestInsertion(string instance)
{
    clock_t start , finish;
    start = clock();
    int i;
    
    Farthest_Insert FarIn;
    vector<vector<int>> Matrix = FarIn.ReadMap(instance);
    vector<int> path ;
    path.push_back(1);
    vector<int> key = Matrix.at(1);
    int second;
    int temp = INT32_MIN;
    for (int i =2; i<key.size(); i++)
    {
        if(key[i]>temp)
        {
            second = i;
            temp=key[i];
        }
    }
    path.push_back(second);
    path.push_back(1);
    
    int next_node,insert_node;
    vector<int>::iterator it;
    while (path.size() < Matrix.size())
    {
        next_node = FarIn.Select_Node(Matrix,path);
        insert_node = FarIn.Insert_Node(Matrix, path, next_node);
        it=path.begin()+insert_node;
        path.insert(it,next_node);
    }
    
    finish = clock();
    double duration = (float)(finish - start)/CLOCKS_PER_SEC;

    string method_sol = "_FarthestInsertion.sol";
    string method_trace = "_FarthestInsertion.trace";
    // string name_sol = instance + method_sol;
    // string name_trace = instance + method_trace;

    size_t found = instance.find_last_of("/\\");
    string strippedInstance = instance.substr(found+1);
    strippedInstance = strippedInstance.substr(0, strippedInstance.size()-4);
    string name_sol = strippedInstance + method_sol;
    string name_trace = strippedInstance + method_trace;
    
    ofstream output_sol(name_sol);
    ofstream output_trace(name_trace);
    
    //cout << "The FARTHESR PATH distance is "  ;
    int distance = 0;
    for(i=0;i<path.size()-1;i++)
    {
        distance += Matrix.at(path.at(i)).at(path.at(i+1)) ;
    }
    //cout << distance << endl;
    output_sol << distance<<endl;
    
    //cout << "The FARTHESR INSERTION PATH IS " << endl ;
    for(i=0;i<path.size()-1;i++)
    {
      //cout << path.at(i) << " " << path.at(i+1) <<" "<< Matrix.at(path.at(i)).at(path.at(i+1))<< endl;
        output_sol << path.at(i)-1 << " " << path.at(i+1)-1 <<" "<<Matrix.at(path.at(i)).at(path.at(i+1)) << endl;
    }
    //cout << endl;
    
    output_trace << duration <<"," <<distance << endl;
    
    output_sol.close();
    output_trace.close();
    //cout <<"The run time of the graph is "<< duration<<endl;
}

