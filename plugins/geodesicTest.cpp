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
#include <string>

#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/GlScene.h>
#include <tulip/GraphIterator.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ForEach.h>
#include <tulip/Algorithm.h>

#include "geodesicTest.h"
#include "shortestPath.h"

#include "fabric.h"
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"

using namespace tlp;
using namespace std;

PLUGIN(geodesicTest)

geodesicTest::geodesicTest(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
}

//
//
//TODO: All functions besides bool geodesicTest::run() are copied from geodesicTest.h
//It would be better to call that plugin and use its output
//
//

    //Find minimum distance between two nodes
int geodesicTest::nodes_map::min_distance(map<int, geodesicTest::nodes_map::myNode*> map1, bool visited[]){
    int min = INT_MAX;
    int min_index = 0;

    for(int i = 0; i<v; i++){
        if(!visited[i] && map1[i]->getDist() < min)
            min = map1[i]->getDist(), min_index = i;
    }

    return min_index;
}

//Dijkstra algorithm
map<int,geodesicTest::nodes_map::myNode*> geodesicTest::nodes_map::dijkstra(int src) {
    map<int, geodesicTest::nodes_map::myNode*> distmap;
    bool visited[v];

    for (int i = 0; i < v; i++) {
        distmap[i] = new geodesicTest::nodes_map::myNode(i, INT_MAX);
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

vector<unsigned int> geodesicTest::nodes_map::tracePath(map<int, geodesicTest::nodes_map::myNode*> distmap, int target, int src){
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
const tlp::node & geodesicTest::find_node(unsigned int id){
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
bool geodesicTest::run()
{
    assert(graph);
    
    static const size_t STEPS = 4;
    
    bool geodesic = true;
    
    //Variable to store the number of edges selected
    unsigned int edgeCount = 0;
    
    //Variable to store the number of end nodes
    unsigned int endNodes = 0;
    
    //End nodes of selected path
    tlp::node endNode1;
    tlp::node endNode2;
    
    //Bool to store when the first end node is found
    bool endNodeFound = false;
    
    //Tulip's selection property
    tlp::BooleanProperty *selectBool = graph->getLocalProperty<tlp::BooleanProperty>("viewSelection");
    
    //This loop checks if the selected edges make a path by looking for nodes with more than two incident selected edges and by looking for end nodes. More than two end nodes indicates multiple paths are selected, and the test fails. 
    //TODO: this could be better. Most nodes get checked twice because we consider both the target and source of each edge.
    tlp::edge e;
    forEach(e, selectBool->getEdgesEqualTo(true,NULL)){ //for each selected edge
        edgeCount++; //count number of edges
        tlp::node source = graph->source(e);
        tlp::node target = graph->target(e);
        tlp::edge incident;
        unsigned int degS = 0, degT = 0;
        forEach(incident, graph->getInOutEdges(source)){
            if(selectBool->getEdgeValue(incident))
                degS++; //count number of selected edges incident with node
        }
        forEach(incident, graph->getInOutEdges(target)){
            if(selectBool->getEdgeValue(incident))
                degT++; //count number of selected edges incident with node
        }
        if(degS > 2 || degT > 2){
            geodesic = false; //either not a path or contains a loop if deg>2 for any node
            if(pluginProgress)
                pluginProgress->setError("Node has degree > 2: Not a path, or cycle detected.");
            return false;
        }
        if(degS == 1){
            endNodes++; //count end nodes
            //Assign node as end node
            if(!endNodeFound){ 
                endNode1 = source;
                endNodeFound = true;
            }
            else
                endNode2 = source;
        }
        if(degT == 1){
            endNodes++; //count end nodes
            //Assign node as end node
            if(!endNodeFound){
                endNode1 = target;
                endNodeFound = true;
            }
            else
                endNode2 = target;
        }
    }
    
    if(edgeCount == 0)
    {
      if(pluginProgress)
        pluginProgress->setError("No path selected");

      return false;
    }
    
    //Test fails if more than one path is selected
    if(endNodes != 2){
        geodesic = false;
           if(pluginProgress)
           pluginProgress->setError("Not exactly two end nodes: not a path, or cycle detected.");
        return false;
    }
    
    //
    //
    //TODO: This just selects the two end nodes for the lengthBetween code
    //Could be much cleaner
    //
    //
    
    BooleanProperty *select = graph->getLocalProperty<BooleanProperty>("viewSelection"); 
    
    tlp::node nodes;
    forEach(nodes, graph->getNodes()){
        select->setNodeValue(nodes, false);
    }
    
    select->setNodeValue(endNode1, true);
    select->setNodeValue(endNode2, true);
    
    
    //
    //
    //TODO: Here is more code from lengthBetween
    //
    //
    
    
    //Store the total number of nodes
    int v = graph->numberOfNodes(); 
    
    tlp::Iterator<node> *selectedNodes = select->getNodesEqualTo(true,NULL);
    
    int path_node[2]; //an array to store source and destination node ids
    path_node[0]=0; // Default source node is 0
    int path_id = 0;

    while(selectedNodes->hasNext()){
        const node &mynode = selectedNodes->next();
        
        //If more than two nodes are selected show an error
        if(path_id>=2)
        {
           if(pluginProgress)
           pluginProgress->setError("More than two nodes are selected");

           return false;
         }
         
       path_node[path_id++] = mynode.id;
    }
    delete selectedNodes;

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
    map<int, geodesicTest::nodes_map::myNode*> mymap = graphAnalysis->dijkstra(path_node[0]);

    //Vector to store path edge indices
    std::vector<unsigned int> mypath;

    mypath = graphAnalysis->tracePath(mymap,path_node[1],path_node[0]);

    int length = mypath.size()-1;
    
    if(length == edgeCount)
        cout << "Path is geodesic." << endl;
    else
        cout << "Path is not geodesic." << endl;
    
    return true;
}
        
    
    
    
