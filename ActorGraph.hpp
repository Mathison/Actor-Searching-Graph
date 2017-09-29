/*
 Student Name:
 Jiawei Li  ID:A53226117
 Ziyan Zhu  ID:A53219779
 */

/**
 ** Assignment 4
 ** Date: 09/05/2017
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include "GEdge.hpp"
#include "GNode.hpp"

// Maybe include some data structures here

using namespace std;

class ActorGraph {
private:
    bool BFS_helper(string actor1, string actor2,int year);
public:
    ActorGraph(void);
    
    ////the record of what actor we have in the graph used to find the node in pathfinder
    unordered_map<string, GNode* > Actornode;
    
    ///used to record the edge we have in order to establish the graph
    unordered_map<string, GEdge*> Movie_edge;
    
    map<int, vector<GEdge*>> Movie_sort;  //this is a set sorted by the movie time
    
    void BFS(const char* out_filename, vector<string> actor1, vector<string> actor2);
    
    void Dij(const char* out_filename, vector<string> actor1, vector<string> actor2);
    
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);
    
    //////////////////////////////////actor connection////////////////////////////////////////////
    /*this is used for actor connection*/
    bool CollectData(const char* in_filename, bool use_weighted_edges);
    
    
    //this part is for BFS
    void connection_BFS(const char* out_filename, vector<string> actor1,
                        vector<string> actor2);
    
    
    //this part isfor union find
    /*used to build the graph, return the year that current to build the graph*/
    /*year count is used to count the position we get*/
    /*return -1 if we reach the end of Movie_sort*/
    int buildGraph();   //time is year count
    
    void Union(GNode* actor1, GNode* actor2);
    
    //find root of the actornode
    string Ufind(GNode* act);
    
    void Union_write(const char* out_filename, vector<string> actor1,
                     vector<string> actor2);
    
    void clean();
    
    void clean_connection();
    
    
    /////////////////////////////extension part
    bool find_actor(string act);
    
    vector<string> movie_com(vector<string> actor);
    
    vector<string> movie_union(vector<string> first, vector<string> second);
    
    vector<string> movie_inter(vector<string> first, vector<string> second);
    
    void movie_display();
    
    ~ActorGraph();
};


#endif // ACTORGRAPH_H
