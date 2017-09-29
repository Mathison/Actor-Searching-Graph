/*
 Student Name:
 Jiawei Li  ID:A53226117
 Ziyan Zhu  ID:A53219779
 */

/**
 ** Assignment 4
 ** Date: 09/05/2017
 */
#ifndef GEDGE_H
#define GEDGE_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "GNode.hpp"

using namespace std;

class GNode;
class GEdge {
public:
    vector<GNode*> actor;       //used when establish the graph
    string movie_title;
    string full_title;
    int movie_time;
    
    GEdge(string movie_title,
          int movie_time)
    : movie_title(movie_title),movie_time(movie_time){
    }
};


#endif
