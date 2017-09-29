/*
 Student Name:
 Jiawei Li  ID:A53226117
 Ziyan Zhu  ID:A53219779
 */

/**
 ** Assignment 4
 ** Date: 09/05/2017
 */


#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <algorithm>
#include "ActorGraph.hpp"

using namespace std;

/* All methods used in pathfinder and actorconnection are implemented in this class
 */
ActorGraph::ActorGraph(void) {}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);
    
    bool have_header = false;
    int num_node=0; //count num of the node
    int num_edge=0; //count num of the edge
    int num_movie=0;//count the number of movie
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
        
        // get the next line
        if (!getline( infile, s )) break;
        
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        
        istringstream ss( s );
        vector <string> record;
        
        while (ss) {
            string next;
            
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;
            
            record.push_back( next );
        }
        
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }
        
        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
        
        //first we check whether the node exist
        //if the vector is empty or the first actor is not the actor we currently read,
        // that means we met a new actor, so we create a new node and insert it
        string movie_un(record[1]+record[2]);
        //movie_un stands for movie_unique , we should give it to full_title in GEdge
        if(Actornode.find(actor_name)==Actornode.end())
        {
            GNode* node = new GNode(actor_name);
            Actornode.insert(pair<string, GNode*>(actor_name, node));
            num_node++;
        }
        //then we check whether there is a same movie( whether the edge exist)
        GNode* curr_actor=Actornode[actor_name];
        //movie doesn't exist in the map
        if(Movie_edge.find(movie_un)==Movie_edge.end()){
            //insert the node to the same set of the movie
            GEdge* edge=new GEdge(movie_title, movie_year);
            edge->full_title=movie_un;
            Movie_edge.insert(pair<string, GEdge*>(movie_un, edge));
            Movie_sort[movie_year].push_back(edge);     //sort the movie by its time
            num_movie++;
        }
        
        //if the map movie is not empty we need to search whether there exist the same movie
        //Movie_edge[movie_un]->actor.push_back(actor_name);
        GEdge* curr_edge=Movie_edge[movie_un];
        curr_actor->Gedge.push_back(curr_edge);
        curr_edge->actor.push_back(curr_actor);
        ///////////////////extensioin part
        curr_actor->movie_set.push_back(movie_un);
        if(curr_actor->movie_key.size()==0){
            vector<string> first;
            first.push_back(movie_un);
            curr_actor->movie_key.push_back(first);
        }
        else{
            curr_actor->movie_key[0].push_back(movie_un);
        }
        
    }
    
    //count the number of edges
    for(unordered_map<string,GEdge*>::iterator it=Movie_edge.begin();
        it!=Movie_edge.end();it++){
        int n=it->second->actor.size();
        num_edge+=(n+1)*n/2;
        
    }
    cout<<"There are"<<num_node<<"node"<<endl;
    cout<<"Edge:"<<num_edge<<endl;
    cout<<"We have "<<num_movie<<"movies"<<endl;
    /////////////////finishing create the graph///////////////////////
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    
    return true;
}

bool ActorGraph::BFS_helper(string actor1, string actor2,int year){
    queue<GNode*> q;
    GNode* curr;
    GEdge* edge;
    vector<GNode*> act;
    
    curr=Actornode.find(actor1)->second;
    q.push(curr);
    while(!q.empty()){
        curr = q.front();
        curr->visit = true;
        q.pop();
        if(curr->actor == actor2){
            clean();
            return true;
        }
        for(int j=0; j<curr->Gedge.size(); j++){
            edge = curr->Gedge[j];
            if(edge->movie_time <= year){
                act = edge->actor;
                for(auto& it : act){
                    if((!it->visit) && it!=curr){
                        it->visit = true;
                        q.push(it);
                    }
                }
            }
        }
    }
    clean_connection();
    return false;
}

void ActorGraph::connection_BFS(const char* out_filename, vector<string> actor1,
                                vector<string> actor2){
    ofstream out(out_filename);
    out<<"Actor1"<<"\t"<<"Actor2"<<"\t"<<"Year"<<endl;
    int min_year = Movie_sort.begin()->first;
    int max_year = Movie_sort.rbegin()->first;
    bool connection = false;
    for(int i = 0; i<actor1.size();i++){
        //the actor is not in the original file
        if(Actornode.find(actor1[i])==Actornode.end()
           || Actornode.find(actor2[i])==Actornode.end()){
            out<<actor1[i]<<"\t"<<actor2[i]<<"\t"<<9999<<endl;
            continue;
        }
        for(int year = min_year; year<=max_year;year++){
            connection = BFS_helper(actor1[i], actor2[i], year);
            if(connection){
                out<<actor1[i]<<"\t"<<actor2[i]<<"\t"<<year<<endl;
                connection = true;
                break;
            }
        }
        if(!connection){
            out<<actor1[i]<<"\t"<<actor2[i]<<"\t"<<9999<<endl;
        }
    }
}


void ActorGraph::Union_write(const char* out_filename, vector<string> actor1,
                             vector<string> actor2){
    ofstream out(out_filename);
    out<<"Actor1"<<"\t"<<"Actor2"<<"\t"<<"Year"<<endl;
    vector<int> year(actor1.size(),9999);  //this is the vector used to sav the year
    int time=0;   //count the time to build the tree
    int y=buildGraph(),i;
    while(y!=-1){
        i=0;
        while(i<actor1.size()){
            //cout<<"search for "<<actor1[i]<<" and "<<actor2[i]<<endl;
            if(year[i]!=9999){   //we already find the earlieast year
                i++;
                continue;
            }
            //the actor is not in the original file
            if(Actornode.find(actor1[i])==Actornode.end()
               || Actornode.find(actor2[i])==Actornode.end()){
                i++;
                continue;
            }
            GNode* act1=Actornode[actor1[i]];
            GNode* act2=Actornode[actor2[i]];
            //we find the year that these two actors first connect
            if(Ufind(act1)==Ufind(act2)){
                year[i]=y;
            }
            i++;
        }
        time++;
        y=buildGraph();
    }
    
    int count=0;
    while(count<actor1.size()){
        out<<actor1[count]<<"\t"<<actor2[count]<<"\t"<<year[count]<<endl;
        count++;
    }
    out.close();
}

int ActorGraph::buildGraph(){
    if(Movie_sort.size()==0)  //the graph building is finished
        return -1;
    map<int,vector<GEdge*>>::iterator it=Movie_sort.begin();
    int i=0;
    while(i<it->second.size()){
        //get the movie
        GEdge* m=it->second[i];
        //begin to union actor by different movie
        int i_a=1;
        while(i_a<m->actor.size()){
            Union(m->actor[0],m->actor[i_a]);
            i_a++;
        }
        i++;
    }
    //finish this years' gaph building
    //delete this year in map
    Movie_sort.erase(it);
    return it->first;
}

void ActorGraph::Union(GNode* actor1, GNode* actor2){
    GNode* a1=Actornode[Ufind(actor1)];
    GNode* a2=Actornode[Ufind(actor2)];
    if(a1==a2)
        return;
    else{
        if(a1->height>a2->height){
            a2->parent=a1;
        }
        else if(a1->height==a2->height){
            a2->parent=a1;
            a1->height++;
        }
        else{
            a1->parent=a2;
        }
    }
}

//find root of the actornode
string ActorGraph::Ufind(GNode* act){
    vector<GNode*> collect;
    GNode* curr=act;
    while(curr->parent){
        collect.push_back(curr);
        curr=curr->parent;
    }
    int it=0;
    while(it<collect.size()){
        collect[it]->parent=curr;
        it++;
    }
    return curr->actor;
}

void ActorGraph::BFS(const char* out_filename, vector<string> actor1,
                     vector<string> actor2){
    ofstream out(out_filename);
    out<<"(actor)--[movie#@year]-->(actor)--..."<<endl;
    queue<GNode*> q;
    GNode* curr;
    GEdge* edge;
    vector<GNode*> act;
    // iterate the actor pair list
    for(int i =0; i<actor1.size(); i++){
        if(Actornode.find(actor1[i])==Actornode.end()){
            out<<endl;
            continue;
        }
        if(Actornode.find(actor2[i])==Actornode.end()){
            out<<endl;
            continue;
        }
        curr = Actornode.find(actor2[i])->second;
        q.push(curr); // initialize the queue
        while(!q.empty()){
            curr = q.front();
            curr->visit = true;
            q.pop();
            //we have reach the target(beginning) actor
            if(curr->actor == actor1[i]){
                while(curr->actor != actor2[i]){
                    out<<"("<<curr->actor<<")--["<<curr->prev_movie<<"#@"
                    <<curr->prev_time<<"]-->";
                    curr = curr->prev;
                    
                }
                out<<"("<<curr->actor<<")"<<endl;
                while(!q.empty()){
                    q.pop();
                }
                break;
            }
            //search for the movie this actor participate in
            for(int j=0; j<curr->Gedge.size(); j++){
                edge = curr->Gedge[j];
                act = edge->actor;
                
                for(int k=0;k<act.size();k++){
                    GNode* temp = act[k];
                    if((!temp->visit) && temp!=curr){
                        temp->prev = curr;
                        temp->prev_movie = edge->movie_title;
                        temp->prev_time = edge->movie_time;
                        temp->visit = true;
                        q.push(temp);
                    }
                }
            }
        }
        
        clean();
    }
    
    out.close();
}

void ActorGraph::Dij(const char* out_filename, vector<string> actor1,
                     vector<string> actor2){
    int count=0;
    ofstream out(out_filename);
    out<<"(actor)--[movie#@year]-->(actor)--..."<<endl;
    while(count<actor1.size()){
        //get the actor we need to search for
        string act1=actor1[count],act2=actor2[count];
        if(Actornode.find(act1)==Actornode.end()){
            count++;
            cout<<"can't find "<<act1<<endl;
            out<<endl;
            continue;
        }
        if(Actornode.find(act2)==Actornode.end()){
            count++;
            cout<<"can't find "<<act2<<endl;
            out<<endl;
            continue;
        }
        //int represent the distance needed to reach the node,act as priority queue
        multimap<int, GNode*> pq;
        GNode* curr=Actornode[act2];    //start from end point
        curr->dist=0; //first node with dist 0
        pq.insert(pair<int,GNode*>(curr->dist,curr));
        while(pq.size()!=0){
            curr=pq.begin()->second;
            pq.erase(pq.begin());
            if(curr->visit)
                continue;
            curr->visit=true;
            if(curr->actor==act1){    //we have reach the target(beginning) actor
                while(curr->prev!=0){
                    out<<"("<<curr->actor<<")--["<<curr->prev_movie<<"#@"
                    <<curr->prev_time<<"]-->";
                    curr=curr->prev;   //stands for GNode->GEdge->GNode
                }
                out<<"("<<curr->actor<<")"<<endl;
                multimap<int, GNode*>::iterator remove=pq.begin();
                while(remove!=pq.end()){
                    pq.erase(remove);
                    remove++;
                }
                break;
            }
            int i=0;
            while(i<curr->Gedge.size()){ //search for the movie this actor participate in
                int e=0;
                GEdge* curr_edge=curr->Gedge[i];
                //the node connected by this edge should all have such distance
                int distance=curr->dist+(2016-curr_edge->movie_time);
                
                //begin to insert actor that participate in the same movie with current actor
                while(e<curr_edge->actor.size()){
                    GNode* connect_actor=curr_edge->actor[e];
                    if(distance<connect_actor->dist){
                        connect_actor->dist=distance;
                        //actore previous point to an actor
                        connect_actor->prev=curr;
                        //previous movie
                        connect_actor->prev_movie=curr_edge->movie_title;
                        //previous time
                        connect_actor->prev_time=curr_edge->movie_time;
                        pq.insert(pair<int,GNode*>(distance,connect_actor));
                    }
                    e++;
                }
                i++;     //move to the next edge(movie) this actor participate
            }
            
        }
        
        //initiate all the element we changed for node and edge
        clean();
        count++;  //move to the next set of actor1 and actor2
    }
    out.close();
}



////////////////////////////////////////////////////////////////extension part
bool ActorGraph::find_actor(string act){
    if(Actornode.find(act)!=Actornode.end())
        return true;
    else
        return false;
}

vector<string> ActorGraph::movie_com(vector<string> actor){
     vector<string> result;
     //get the actor from the Actornode
     vector<GNode*> actor_set;   //this is used to store the set of the actor user input
     multimap<int, GEdge*> movie_connect;
     vector<string> movie_all;
     int i=0;
     while(i<actor.size()){
        movie_all=movie_union(movie_all,Actornode[actor[i]]->movie_set);
        actor_set.push_back(Actornode[actor[i]]);
        i++;
     }
 
     int start=0;
     /////if it reach end(), then we have found all the component we need
     while(start<actor_set.size()){     

        int com=start;
        com++;
        while(com<actor_set.size()){
            vector<string> v=movie_inter(actor_set[start]->movie_set,
                actor_set[com]->movie_set);
 
            if(v.size()==0){   //there is no common movie between these two actor;
                com++;
            }
            else{   //now we let the first node(start node) to represent this mew component
                actor_set[start]->movie_set=
                movie_union(actor_set[start]->movie_set,actor_set[com]->movie_set);
                int count=0;
                while(count<actor_set[start]->movie_key.size()){
		      //the new key has no intersection part with the old one
                     if(movie_inter(actor_set[start]->movie_key[count],v).size()==0){   
                           count++;
                     }
                     else{
                        actor_set[start]->movie_key[count]=
                        movie_inter(actor_set[start]->movie_key[count],v);
                        break;
                     }
                }
                if(count==actor_set[start]->movie_key.size()){
                    actor_set[start]->movie_key.push_back(v);
                }
                //already merge it to start as a larger component no longer need it
                actor_set.erase(actor_set.begin()+com); 
            }
        }
        
        start++;
     }
     //till now all remaining node in actor set represent a component with several movie_key
     // inside to connect every node
     i=0;
     while(i<actor_set.size()){
        GNode* curr=actor_set[i];
        int first=0;
        while(first<curr->movie_key.size()){
            //we only need one movie in each key set
            result.push_back(curr->movie_key[first].at(0)); 
            first++;
        }
        i++;
     }
     return result;
}

//get the union set bwtween two sets of movie
vector<string> ActorGraph::movie_union(vector<string> first, vector<string> second){
    vector<string> v(10000);
    vector<string>::iterator it;
    sort(first.begin(), first.begin()+first.size());
    sort(second.begin(), second.begin()+second.size());
    
    it=set_union(first.begin(), first.begin()+first.size(), second.begin(),
                 second.begin()+second.size(), v.begin());
    
    v.resize(it-v.begin());
    return v;
}

//get the intersection set between two sets of movie
vector<string> ActorGraph::movie_inter(vector<string> first, vector<string> second){
    vector<string> v(10000);
    vector<string>::iterator it;
    
    sort(first.begin(), first.begin()+first.size());
    sort(second.begin(), second.begin()+second.size());
    
    it=set_intersection(first.begin(), first.begin()+first.size(),
                        second.begin(), second.begin()+second.size(), v.begin());
    v.resize(it-v.begin());
    return v;
}

///display some actor that are in the same movie
void ActorGraph::movie_display(){
    unordered_map<string, GEdge*>::iterator it=Movie_edge.begin();
    while(it!=Movie_edge.end()){
        cout<<"Now the movie is "<<it->first<<endl;
        vector<GNode*>::iterator act_it=it->second->actor.begin();
        while(act_it!=it->second->actor.end()){
            cout<<(*act_it)->actor<<endl;
            act_it++;
        }
        it++;
    }
}

//this is used to initialize the element in GNode and GEdge
void ActorGraph::clean(){
    for(unordered_map<string,GNode*>::iterator it_node=Actornode.begin();
        it_node!=Actornode.end(); it_node++){
        //for dijastra algorithm
        it_node->second->prev=0;
        it_node->second->prev_movie='0';
        it_node->second->prev_time=0;
        it_node->second->dist=2147483646;
        it_node->second->visit=false;
        ////for union find
        it_node->second->parent=NULL;
        it_node->second->height=0;
    }
    
}

void ActorGraph::clean_connection(){
    for(unordered_map<string,GNode*>::iterator it_node=Actornode.begin();
        it_node!=Actornode.end(); ++it_node){
        it_node->second->visit=false;
    }
}


ActorGraph::~ActorGraph(){
    for(unordered_map<string,GNode*>::iterator it_node=Actornode.begin();
        it_node!=Actornode.end(); ++it_node){
        delete it_node->second;
    }
    for (unordered_map<string,GEdge*>::iterator it_edge=Movie_edge.begin();
         it_edge!=Movie_edge.end(); ++it_edge){ ///delete the edge
        delete it_edge->second;
    }
}
