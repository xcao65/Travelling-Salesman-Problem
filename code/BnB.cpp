//
//  BFS.h
//  TSP_BnB
//
//  Created by leyna on 12/2/16.
//  Copyright © 2016 leyna. All rights reserved.
//

#ifndef BFS_h
#define BFS_h

#include <fstream>
#include <cmath>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <queue>
#include <iomanip>
#include <stack>
#include "common.h"

using namespace std;

class MST_Bound
{
public:
    MST_Bound(vector<vector<int>>& Matrix, vector<int> &path);
    int minKey(vector<int> key, bool mstSet[]);
    int Bound_Cal(vector<vector<int>>& Matrix, vector<int> &path);
private:
    unsigned long int dimension;
};

MST_Bound::MST_Bound(vector<vector<int>>& Matrix, vector<int> &path)
{
    dimension = Matrix.size()-1;
}

//Function to find the nearest node from a known path
int MST_Bound::minKey(vector<int> key, bool mst_judge[])
{
    int min = INT32_MAX;
    int min_index;
    
    for(int i = 0; i <= dimension ; i ++)
    {
        if(mst_judge[i] == false && key[i]<min)
        {
            min = key[i];
            min_index = i;
        }
    }
    return min_index;
}

//Function to find the MST lower bound with Prim algorithm
int MST_Bound::Bound_Cal(vector<vector<int> > &Matrix, vector<int> &path)
{
    int i,j,k;
    int dismin,bound = 0;
    int temp_to;
    int city_to;
    int current;
    vector<int> mst_Path;
    bool mst_Judge [dimension+1];
    
    //make the visited note marked;
    mst_Judge[0] = true;
    for(i=1;i<dimension+1;i++)
    {
        mst_Judge[i] = false;
    }
    for (i=0;i<path.size();i++)
    {
        k = path.at(i);
        mst_Judge[k]=true;
    }
    //find two nearest edge from the path
    int start_p = path.at(0);
    int end_p = path.at(path.size()-1);
    
    int dis_node;
    int dismin1 = INT32_MAX;
    int dismin2 = INT32_MAX;
    int random;
    
    for(i=0;i< dimension+1;i++)
    {
        if(mst_Judge[i] == false && Matrix.at(start_p).at(i)<dismin1)
        {
            dismin1 = Matrix.at(start_p).at(i);
            random = i;
        }
    }
    
    for(i=0;i< dimension+1;i++)
    {
        if(mst_Judge[i] == false && Matrix.at(end_p).at(i)<dismin2)
        {
            dismin2 = Matrix.at(end_p).at(i);
        }
    }
    dis_node = dismin1 + dismin2;

    //find the mst length
    
    if(path.size() == (dimension -1))
    {
        bound = 0;
    }
    else
    {
        mst_Path.push_back(random);
        mst_Judge[random] = true;
        vector<int> key;
        
        for(i=0;i<(dimension-1-path.size());i++)
        {
            dismin = INT32_MAX;
            for(j=0; j<mst_Path.size();j++)
            {
                key = Matrix.at(mst_Path[j]);
                temp_to= minKey(key, mst_Judge);
                current = key.at(temp_to);
                if(current <dismin)
                {
                    dismin = current;
                    city_to = temp_to;
                }
            }
            bound +=  dismin;
            mst_Path.push_back(city_to);
            mst_Judge[city_to] = true;
        }
    }
    return bound+dis_node;
}

class BFS
{
public:
    vector<vector<int>> ReadMap(string instance);
    int Path_Cal(vector<vector<int>> &Matrix, vector<int> &path);
};

//Function to create an adjacent matrix
vector<vector<int>> BFS::ReadMap(string instance)
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

//Function to calculate the lenth of a path
int BFS::Path_Cal(vector<vector<int>> &Matrix, vector<int> &path)
{
    double path_dis;
    double temp;
    for(int i=0; i < path.size()-1;i++)
    {
        temp = Matrix.at(path.at(i)).at(path.at(i+1));
        path_dis += temp;
    }
    return path_dis;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*class Farthest_Insert
{
    public :
    int Farthest_Node(vector<int>& key, bool path_Judge[]);
    int Select_Node(vector<vector<int>>& Matrix, vector<int>& path);
    int Insert_Node(vector<vector<int>>& Matrix, vector<int>& path, int r);
};

//Function to find a farthest node from a known path
int Farthest_Insert::Farthest_Node(vector<int>& key, bool path_Judge[])
{
    unsigned long int dimension = key.size();
    int max = INT_MIN;
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
    int dis_max = INT_MIN;
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
    int temp = INT_MAX;
    int dis_min = INT_MAX;
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
    }*/

//Function to start the farthest insertion
vector<int> Farthest_Insertion(vector<vector<int>> &Matrix)
{
    int i;
    
    Farthest_Insert FarIn;
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
   

    return path;
}

//Function to use DFS search for a path
void BnB(string instance, int cutoff_time)
{
    clock_t start ,interim, finish;
    BFS bfs_s;
    start = clock();
    int i,j ;
    vector<vector<int>> Matrix = bfs_s.ReadMap(instance);
    unsigned long int dimension = (Matrix.size()-1);

        string method = "_BnB_";
        string cut_off_time = to_string(static_cast<long long>(cutoff_time));
        string sol_suffix = ".sol";
        string trace_suffix = ".trace";
//        string name_sol = instance + method+cut_off_time + sol_suffix;
//        string name_trace = instance + method+cut_off_time + trace_suffix;
    
    
            size_t found = instance.find_last_of("/\\");
            string strippedInstance = instance.substr(found+1);
            strippedInstance = strippedInstance.substr(0,strippedInstance.length()-4);
            string name_sol = strippedInstance+method +  cut_off_time + sol_suffix;
            string name_trace = strippedInstance+method +  cut_off_time + trace_suffix;

        ofstream output_sol(name_sol);
        ofstream output_trace(name_trace);
    
    //ofstream output_trace("/Users/leyna/Desktop/DATA/output/Atlanta_BnB.trace");
    
    int bound_MST;
    int path_dis;
    
    //create a vector to store all cities
    vector<int> city_all;
    vector<int> city_left;
    
    //store the city left and refresh the matrix
    for(i = 1 ; i<Matrix.size();i++)
    {
        city_all.push_back(i);
    }
    
    //stack to store path
    stack<vector<int>> myStack;
    vector<int> first = {1};
    myStack.push(first);
    
    int Bound_All;
    vector<int> path_before;
    vector<int> new_path;
    vector<int> Best_PathUtil = Farthest_Insertion(Matrix);
    int Best_Path = 0;
    for(i=0;i<Best_PathUtil.size()-1;i++)
    {
        Best_Path += Matrix.at(Best_PathUtil.at(i)).at(Best_PathUtil.at(i+1)) ;
    }

    int temp;
    
    interim = clock();
    output_trace <<setiosflags(ios::fixed)<<setprecision(2)<<((double)interim/CLOCKS_PER_SEC) << ",";
    output_trace << Best_Path <<endl;
    
    while(!myStack.empty())
    {
        interim = clock();
        if((interim-start)/CLOCKS_PER_SEC < cutoff_time)
        {
            path_before = myStack.top();
            new_path = myStack.top();
            myStack.pop();
        
            //store the city left and refresh the matrix
            city_left = city_all;
            for(j = 0; j<path_before.size();j++)
            {
                i = 0;
                while(path_before.at(j)!=city_left.at(i))
                {
                    i++;
                }
                city_left.erase(city_left.begin()+i);
            }
        
            //check the possible path entering Queue
            for(i = 0; i<city_left.size();i++)
            {
                path_before = new_path;
                path_before.push_back(city_left.at(i));
                MST_Bound mst_bound(Matrix,path_before);
                path_dis = bfs_s.Path_Cal(Matrix,path_before);
                //if  solution found
                if(path_before.size()==dimension)
                {
                    temp = path_before.back();
                    path_dis += Matrix[temp][1];
                    if(path_dis< Best_Path)
                    {
                        Best_Path = path_dis;
                        interim = clock();
                        output_trace <<setiosflags(ios::fixed)<<setprecision(2)<<((double)interim/CLOCKS_PER_SEC) << ",";
                        output_trace << Best_Path <<endl;
                        //cout <<setiosflags(ios::fixed)<<setprecision(2)<<((double)interim/CLOCKS_PER_SEC) << ",";
                        //cout << Best_Path <<endl;
                        path_before.push_back(1);
                        Best_PathUtil = path_before;
                    }
                }
                else
                {
                    bound_MST = mst_bound.Bound_Cal(Matrix,path_before);
                    Bound_All = bound_MST + path_dis;
                    if (Bound_All < Best_Path)
                    {
                        myStack.push(path_before);
                    }
                }
            }
        }
        else
        {
            break;
        }
    }
   
    output_sol << Best_Path << endl;
    
    //cout << "The best path within 10min is \n";
    for(i=0;i<Best_PathUtil.size()-1 ;i++)
    {
      //cout << Best_PathUtil.at(i)-1 << " " << Best_PathUtil.at(i+1)-1 <<" "<< Matrix.at(Best_PathUtil.at(i)).at(Best_PathUtil.at(i+1))<< endl;
        output_sol << Best_PathUtil.at(i)-1 << " " << Best_PathUtil.at(i+1)-1 <<" "<< Matrix.at(Best_PathUtil.at(i)).at(Best_PathUtil.at(i+1))<< endl;
    }   
    //cout << "The best path's distance is " << Best_Path << endl;
    
    finish = clock();
    double duration = (float)(finish - start)/CLOCKS_PER_SEC;
    //cout << "The total run time is " << duration << endl;
    
      output_sol.close();
      output_trace.close();

    
}

#endif /* BFS_h */
