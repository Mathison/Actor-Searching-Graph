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
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char** argv){
    
    ActorGraph* graph = new ActorGraph();
    /////////////create graph
    if(!graph->loadFromFile(argv[1],true)){
        cout<<"faill to create graph"<<endl;
        return 0;
    }
    cout<<"Our goal is to find the least number of movie that can connect the actors you selected!!"<<endl;
    cout<<"please choose several actor names from the files"<<endl;
    
    string input;
    vector<string> in;
    ////////////////////get the input
    //display all the actor in the movie
    //graph->movie_display();
    getline(cin, input);
    while(input!="done"){
        if(!graph->find_actor(input)){
            cout<<"please input an actor/actress in the file"<<endl;
            getline(cin, input);
            continue;
        }
        in.push_back(input);
        cout<<"please select another actor/actress, if you want to end this please input 'done'."<<endl;
        getline(cin, input);
    }
    if(in.size()==0){
        cout<<"No input!!! Thank you!!!"<<endl;
        return 0;
    }
    vector<string> result=graph->movie_com(in);
    
    cout<<"The movie we need to connect these actors are:"<<endl;
    for(auto movie : result){
        cout<<movie<<endl;
    }
    cout<<endl;
    cout<<"Thanks for using!!!"<<endl;
    
    
    return 0;
}
