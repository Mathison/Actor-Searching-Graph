/*
 Student Name:
 Jiawei Li  ID:A53226117
 Ziyan Zhu  ID:A53219779
 */

/**
 ** Assignment 4
 ** Date: 09/05/2017
 */
#ifndef GNODE_H
#define GNODE_H

#include <iostream>
#include <vector>
#include <unordered_map>
//#include <set>
#include "GEdge.hpp"
using namespace std;


class GEdge;
class GNode{
public:
    string actor;           //everynode contains an actor
    vector<GEdge*> Gedge;    //there could be multiple edges to one node
    
    bool visit;
    GNode* prev;       //note for the edge to the parent only used in the BFS
    GNode* parent; //used for union-find
    string prev_movie;
    int prev_time;
    int dist;  //used for Dij algorithm
    int height;  //used to calculate the height of the tree in disjointed set
    
    //////////////extension part
    //extension part:: used to collect the set of the movie an actor participated in
    vector<string> movie_set;
    // used to store the set of movies that are the key to connnect a component
    //their could be multiple union_key to represent the component, so we need to set a vector<vector>
    vector<vector<string>> movie_key;
    
    GNode(string actor)
    : actor(actor),visit(false),prev(0),parent(NULL),height(0),dist(2147483646){}
};

#endif
