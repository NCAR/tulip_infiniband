/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux, University Corporation
 * for Atmospheric Research
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include <fstream>
#include <algorithm>
#include <vector>

#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/GlScene.h>
#include <tulip/GraphIterator.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>

#include "Dijkstra.h"

#include "fabric.h"
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"

using namespace tlp;
using namespace std;

PLUGIN(Dijkstra)

//Constructor
Dijkstra::Dijkstra(tlp::PluginContext* context)
: tlp::Algorithm(context){
}


//Find minimum distance between two nodes
int Dijkstra::nodes_map::min_distance(map<int, Dijkstra::nodes_map::myNode*> map1, bool visited[]){
    int min = INT_MAX;
    int min_index = 0;

    for(int i = 0; i<v; i++){
        if(!visited[i] && map1[i]->getDist() < min)
            min = map1[i]->getDist(), min_index = i;
    }

    return min_index;
}


//Print
void Dijkstra::nodes_map::printResult(map<int, Dijkstra::nodes_map::myNode*> map1) {
    for(int i = 0; i<v; i++){
        cout<<i<<" is from: "<<map1[i]->getFrom()<<" its distance is: "<<map1[i]->getDist()<<endl;
    }
}


//Apply Dijkstra's Algorithm
map<int,Dijkstra::nodes_map::myNode*> Dijkstra::nodes_map::dijkstra(int src) {
    map<int, Dijkstra::nodes_map::myNode*> distmap;
    bool visited[v];

    for (int i = 0; i < v; i++) {
        distmap[i] = new Dijkstra::nodes_map::myNode(i, INT_MAX);
        visited[i] = false;
    }
    distmap[src]->setDist(0);

    for (int count = 0; count < v - 1; count++) {
        int u = min_distance(distmap, visited);
        visited[u] = true;

        for (int i = 0; i < v; i++) {
            if (!visited[i] && adjacent_matrix[u][i] && distmap[u]->getDist() != INT_MAX &&
                distmap[u]->getDist() + adjacent_matrix[u][i] < distmap[i]->getDist())
            {
                distmap[i]->setDist(distmap[u]->getDist() + adjacent_matrix[u][i]);
                distmap[i]->setFrom(u);
            }
        }
    }
    
    return distmap;
}

//Trace back the path suggested by Dijkstra
vector<unsigned int> Dijkstra::nodes_map::tracePath(map<int, Dijkstra::nodes_map::myNode*> distmap, int target, int src){
    cout<<"the destination is: "<<target<<endl;
    vector<unsigned int> path;
    int pos = target;
    for(int i = 0; i<v-1; i++){
       path.push_back(pos);
       cout<<"<--"<<distmap[pos]->getFrom()<<" ";
       pos = distmap[pos]->getFrom();
       if(pos == src){
         path.push_back(pos);
         break;
       }
    }
    cout<<" "<<endl;
   
   return path;
}


//Find node from its id
const tlp::node & Dijkstra::find_node(unsigned int id){
     tlp::Iterator<tlp::node> *itnodes = graph->getNodes();
       
       while(itnodes->hasNext()){
          const tlp::node &node = itnodes->next();
             if(node.id == id)
                return node;  
       }
}

//Tulip's main function
bool Dijkstra::run()
{
    assert(graph);

    static const size_t STEPS = 4;
   
   //PluginProcess interacts with users as the plugin runs
    if(pluginProgress)
    {
        pluginProgress->showPreview(false);
        pluginProgress->setComment("Importing Routes");
        pluginProgress->progress(0, STEPS);
    }
   
    //Get the total number of nodes 'v'
    tlp::Iterator<tlp::node> *itnod = graph->getNodes();

    int v = 0; 
    while(itnod->hasNext()){
       itnod->next();
        v++;
    }
   
        
    //Tulip's selection algorithm
    BooleanProperty *selectBool = graph->getLocalProperty<BooleanProperty>("viewSelection"); //Tulip's Boolean Property to access "viewSelection."
   
    //Get an iterator to access all the selected nodes(To select means to set the node value of viewSelection to be true)
    tlp::Iterator<node> *selections = selectBool->getNodesEqualTo(true,NULL); 
    
    int path_node[1]; //an array to store source node and destination node ids 
    path_node[0]=0; // Default source node is 0
    int path_id = 0;
   
    while(selections->hasNext()){
        const node &mynode = selections->next();
      
         //If more than one node is selected show an error
         if(path_id>=1)
         {
           if(pluginProgress)
             pluginProgress->setError("More than one node is selected");

           return false;
         }
      
        path_node[path_id++] = mynode.id;
    }

    //If no node is selected show the error
    if(path_id == 0)
    {
      if(pluginProgress)
        pluginProgress->setError("No node is selected");

      return false;
    }   
   
    if(pluginProgress)
    {
        pluginProgress->setComment("Implementing Dijkstra's algorithm on the graph...");
        pluginProgress->progress(1, STEPS);
    }
   
    nodes_map *graphAnalysis = new nodes_map(graph,v);
    
    map<int, Dijkstra::nodes_map::myNode*> mymap = graphAnalysis->dijkstra(path_node[0]);

    int max = 1;
    
        if(pluginProgress)
    {
        pluginProgress->setComment("Calculating the minimum distances..");
        pluginProgress->progress(2, STEPS);
    }
    
    //Print Distance and find out the max and min numbers
    for(int i = 0; i<v; i++){
        max = std::max(max,mymap[i]->getDist());
        cout<<"Node id: "<<i<<" ---- The shortest distance: "<<mymap[i]->getDist()<<endl;
    }
   
    cout<<"*************************************************************************"<<endl;
    cout<<""<<endl;
   
    tlp::IntegerProperty * ibHop = graph->getProperty<tlp::IntegerProperty>("ibHop");
    assert(ibHop);
   
    if(pluginProgress)
    {
        pluginProgress->setComment("Printing the minimum steps");
        pluginProgress->progress(3, STEPS);
    }

    tlp::Iterator<tlp::node> *itnodes = graph->getNodes();
    while(itnodes->hasNext()){
        const tlp::node &node = itnodes->next();
        const int &temp = mymap[node.id]->getDist();
        ibHop->setNodeValue(node, temp);
    }
   
    if(pluginProgress)
    {
        pluginProgress->setComment("Dijkstra's Algorithm Finished");
        pluginProgress->progress(STEPS, STEPS);
    }

    return true;
}















