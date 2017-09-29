/*
 Student Name:
 Jiawei Li  ID:A53226117
 Ziyan Zhu  ID:A53219779
 */

/**
 ** Assignment 4
 ** Date: 09/05/2017
 */
#include "ActorGraph.hpp"
#include <fstream>
#include <sstream>
int main(int argc, char** argv){
    ActorGraph* graph = new ActorGraph();
    /////////////create graph
    if(!graph->loadFromFile(argv[1],argv[2]))
        cout<<"faill to create graph"<<endl;
    /////////////find the two actor we need to find
    ifstream test_file(argv[3]);
    
    bool have_header = false;
    vector <string> actor_record;
    vector <string> actor1_name;
    vector <string> actor2_name;
    // keep reading lines until the end of file is reached
    while (!test_file.eof()) {
        string s;
        // get the next line
        if (!getline( test_file, s )){
            break;
        }
        
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        
        istringstream ss( s );
        
        while (ss) {
            string next;
            
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;
            actor_record.push_back( next );
        }
        
        if (actor_record.size()%2 != 0) {
            // we should have exactly 2 columns
            continue;
        }
        
    }
    int i=0;
    while((i+1)<actor_record.size()){
        actor1_name.push_back(actor_record[i]);         //first name
        actor2_name.push_back(actor_record[i+1]);         //second name
        i+=2;
    }
    
    if((*argv[2])=='u')
        graph->BFS(argv[4],actor1_name,actor2_name);
    else if((*argv[2])=='w')
        graph->Dij(argv[4],actor1_name,actor2_name);
    else
        cout<<"Illegal command!!!"<<endl;
    delete graph;
    return 0;
}
