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

#include "shortestPath.h"

#include "fabric.h"
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"

using namespace tlp;
using namespace std;

PLUGIN(shortestPath)

shortestPath::shortestPath(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
}

//Find minimum distance between two nodes
int shortestPath::nodes_map::min_distance(map<int, shortestPath::nodes_map::myNode*> map1, bool visited[]){
    int min = INT_MAX;
    int min_index = 0;

    for(int i = 0; i<v; i++){
        if(!visited[i] && map1[i]->getDist() < min)
            min = map1[i]->getDist(), min_index = i;
    }

    return min_index;
}

//Dijkstra algorithm
map<int,shortestPath::nodes_map::myNode*> shortestPath::nodes_map::dijkstra(int src) {
    map<int, shortestPath::nodes_map::myNode*> distmap;
    bool visited[v];

    for (int i = 0; i < v; i++) {
        distmap[i] = new shortestPath::nodes_map::myNode(i, INT_MAX);
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

vector<unsigned int> shortestPath::nodes_map::tracePath(map<int, shortestPath::nodes_map::myNode*> distmap, int target, int src){
    vector<unsigned int> path;
    unsigned int pos = target;
    for(int i = 0; i<v-1; i++){
        path.push_back(pos);
        pos = distmap[pos]->getFrom();
        if(pos == (unsigned int)src){
            path.push_back(pos);
            break;
        }
    }

    return path;
}

//Find node from its id
const tlp::node & shortestPath::find_node(unsigned int id){
    tlp::Iterator<tlp::node> *itnodes = graph->getNodes();

    while(itnodes->hasNext()){
        const tlp::node &node = itnodes->next();
        if(node.id == id)
            return node;
    }

    itnodes = graph->getNodes();
    const tlp::node &temp = itnodes->next();
    delete itnodes;
    return temp;
}

//Tulip's main function
bool shortestPath::run()
{
    assert(graph);

    static const size_t STEPS = 5;
     //PluginProcess interacts with users as the plugin runs
    
    if(pluginProgress)
    {
        pluginProgress->setComment("Getting selected nodes");
        pluginProgress->progress(0, STEPS);
    }

    //Store the total number of nodes
    int v = graph->numberOfNodes();

    //Tulip's selection algorithm
    BooleanProperty *selectBool = graph->getLocalProperty<BooleanProperty>("viewSelection"); 

    //Get an iterator to access all the selected nodes
    tlp::Iterator<node> *selections = selectBool->getNodesEqualTo(true,NULL);

    int path_node[2]; //an array to store node and destination node ids
    path_node[0]=0; // Default source node is 0
    int path_id = 0;

    while(selections->hasNext()){
        const node &mynode = selections->next();
        
        //If more than two nodes are selected show an error
        if(path_id>=2)
        {
           if(pluginProgress)
           pluginProgress->setError("More than two nodes are selected");

           return false;
         }
         
       path_node[path_id++] = mynode.id;
    }
    delete selections;

    //If less than two nodes are selected show an error
    if(path_id <= 1)
    {
      if(pluginProgress)
        pluginProgress->setError("Less than two nodes are selected");

      return false;
    }

    if(pluginProgress)
    {
        pluginProgress->progress(1, STEPS);
        pluginProgress->setComment("Applying Dijkstra's algorithm to source node.");
    }

    //Apply Dijkstra's algorithm from source node
    nodes_map *graphAnalysis = new nodes_map(graph,v);
    map<int, shortestPath::nodes_map::myNode*> mymap = graphAnalysis->dijkstra(path_node[0]);
    
        if(pluginProgress)
    {
        pluginProgress->progress(2, STEPS);
        pluginProgress->setComment("Storing path found by Dijkstra's algorithm.");
    }

    //Vector to store path edge indices
    std::vector<unsigned int> mypath;

    //Iterator for all nodes in graph
    tlp::Iterator<tlp::node> *itnodes = graph->getNodes();

        if(pluginProgress)
    {
        pluginProgress->progress(3, STEPS);
        pluginProgress->setComment("Selecting path.");
    }
    
    //Select the found path
    mypath = graphAnalysis->tracePath(mymap,path_node[1],path_node[0]);
    tlp::ColorProperty * resetColor = graph->getLocalProperty<tlp::ColorProperty>("viewColor");
    itnodes = graph->getNodes();
    while(itnodes->hasNext()){
        const tlp::node &node = itnodes->next();
        for(unsigned int ID : mypath){
            if(node.id == ID){
                selectBool->setNodeValue(node, true);
            }
        }
    }
    delete itnodes;
    
    for(unsigned int i = 0; i<mypath.size()-1; i++){
        const tlp::node &source = find_node(mypath[i]);
        tlp::Iterator<tlp::edge> *itedges = graph->getOutEdges(source);
        while(itedges->hasNext()){
            const tlp::edge &edge = itedges->next();
            if(graph->target(edge).id == mypath[i+1])
                selectBool->setEdgeValue(edge, true);
        }
        delete itedges;
    }
    
        if(pluginProgress)
    {
        pluginProgress->progress(4, STEPS);
        pluginProgress->setComment("Printing path length to terminal.");
    }
    
    cout << "Distance between nodes: " << mypath.size() << endl;

    if(pluginProgress)
    {
        pluginProgress->setComment("Shortest path found and selected.");
        pluginProgress->progress(STEPS, STEPS);
    }

    return true;
}
