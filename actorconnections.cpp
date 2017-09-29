/*
 Student Name:
 Jiawei Li  ID:A53226117
 Ziyan Zhu  ID:A53219779
 */

/**
 ** Assignment 4
 ** Date: 09/05/2017
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include "ActorGraph.hpp"

using namespace std;

/* In this main method, for a given movie database and a list of actor pairs, the program will
   answer the following query for every actor pair (X, Y) in the given list:
   "After which year did actors X and Y become connected?"
 */
int main(int argc, char** argv){
    
    ActorGraph* graph = new ActorGraph();
    /////////////create graph
    if(!graph->loadFromFile(argv[1],true)){
        cout<<"faill to create graph"<<endl;
        return 0;
    }
    
    ifstream test_file(argv[2]);
    
    bool have_header = false;
    vector <string> actor_record;
    vector <string> actor1_name;
    vector <string> actor2_name;
    // keep reading lines until the end of file is reached
    while (!test_file.eof()) {
        string s;
        // get the next line
        if (!getline( test_file, s )){
            cout<<"Break out"<<endl;
            break;
        }
        
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        
        istringstream ss(s);
        
        while (ss) {
            string next;
            
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;
            
            actor_record.push_back( next );
        }
        
        if (actor_record.size()%2 != 0) {
            // we should have exactly 2 columns
            cout<<"wrong read"<<endl;
            continue;
        }
        
    }
    int i=0;
    while((i+1)<actor_record.size()){
        actor1_name.push_back(actor_record[i]);         //first name
        actor2_name.push_back(actor_record[i+1]);         //second name
        i+=2;
    }
    
    if(strcmp (argv[4],"bfs") == 0){
        graph->connection_BFS(argv[3],actor1_name,actor2_name);
    }
    else if(strcmp (argv[4],"ufind") == 0){
        graph->Union_write(argv[3],actor1_name,actor2_name);
    }
    else{
        cout<<"illegal input please select from 'BFS' and 'Ufind' "<<endl;
    }
    
    
    return 0;
}
