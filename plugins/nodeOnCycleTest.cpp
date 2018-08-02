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
#include <queue>
#include <vector>

#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/GlScene.h>
#include <tulip/GraphIterator.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ForEach.h>

#include "nodeOnCycleTest.h"

#include "fabric.h"
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"


PLUGIN(nodeOnCycleTest)

nodeOnCycleTest::nodeOnCycleTest(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
}

//Tulip's main function
bool nodeOnCycleTest::run()
{
    assert(graph);
    
    //Store number of nodes in graph
    int v = graph->numberOfNodes();
    
    //Assume not on cycle until found otherwise
    bool cycle = false;
    
    static const size_t STEPS = 0;
    
    //Count of how many nodes are selected
    int numSel = 0;
    
    //Tulip's selection property
    tlp::BooleanProperty *selectBool = graph->getLocalProperty<tlp::BooleanProperty>("viewSelection");
    
    //Iterator over all selected nodes
    tlp::Iterator<tlp::node> *selected = selectBool->getNodesEqualTo(true,NULL);
    
    tlp::node src;
    
    //Check how many nodes are selected
    while(selected->hasNext()){
        src = selected->next();
        numSel++; //iterate counter
        //Error if too many nodes are selected
        if(numSel>1){
            if(pluginProgress)
                pluginProgress->setError("More than one node selected");
        return false;
        }
    }
    //Error if no nodes are selected
    if(numSel<1){
        if(pluginProgress)
            pluginProgress->setError("No node selected");
        return false;
    }
    delete selected;
    
    //Test for self-loop
    tlp::node inOutNodes;
    forEach(inOutNodes, graph->getInOutNodes(src)){
        if(inOutNodes == src){
            cycle = true;
            std::cout << "Node lies on a self-loop." << std::endl;
            return true; //stop plugin
        }
    }
    
    //Test for end node
    if( graph->deg(src) < 2 ){
        std::cout << "Node does not lie on a cycle." << std::endl;
        return true; //stop plugin
    }
    
    //Test for two edges connecting src node and some other node
    //NOTE: this plugin treats all graphs as undirected
    std::vector<int> adj; 
    tlp::edge srcEdge;
    forEach(srcEdge, graph->getInOutEdges(src)){ 
        if( graph->source(srcEdge) != src) //Store for neighbors which are sources of the edge
            adj.push_back(graph->source(srcEdge));
        else //Store neighbors which are targets of the edge
            adj.push_back(graph->target(srcEdge));
    }
    for( int i = 0; i < adj.size(); i++ ){
        for( int j = 0; j < adj.size(); j++ ){
            if( adj[i] == adj[j] && i != j ){ //Check for two edges leading to the same node
                cycle = true;
                std::cout << "Node lies on a cycle of length 2." << std::endl;
                return true; //stop plugin
            }
        }
    }
    
    //Find the largest node ID in the graph. This guards against having indexing issues in parent array (next section) due to nodes being deleted
    unsigned int maxID = 0;
    tlp::node allNodes;
    forEach(allNodes, graph->getNodes()){
        if(allNodes.id > maxID)
            maxID = allNodes.id;
    }    
    
    //Array to store the previous node on a path. Fill with dummy number
    unsigned int parent[maxID+1];
    for(int i = 0; i <= maxID; i++)
        parent[i] = maxID + 1;
    //Assign different dummy number for src node
    parent[src.id] = maxID + 2; 
        
    //Queue for nodes which need visited
    std::queue<tlp::node> Q; 
    
    //Put src node in queue
    Q.push(src);
    
    while(!Q.empty() && !cycle){
        tlp::node currentNode = Q.front();
        tlp::node neighbor;
        //for each adjacent node
        forEach(neighbor, graph->getInOutNodes(currentNode)){ 
            //if neighbor doesn't have a parent yet
            if( parent[neighbor.id] == maxID + 1 ){ 
                //store current node as parent of neighbor
                parent[neighbor.id] = currentNode.id;
                //add neighbor to queue to be visited
                Q.push(neighbor);
            }
            //if neighbor has a path already found and is not the parent of currentNode
            else if( neighbor.id != parent[currentNode.id] ){ 
                //Store two paths which backtrack from currentNode and neighbor to src
                //array to store node IDs along path from src to currentNode (traversing backwards)
                unsigned int path1[v]; 
                bool stop1 = false;
                //first path starts backtracking from currentNode
                unsigned int ID1 = currentNode.id; 
                //store node ID in path
                path1[0] = ID1;
                
                int count1 = 0;
                //continue until path has traversed back to src
                while( !stop1 && count1 < v ){
                    count1++;
                    //don't store src in path, else path1 and path2 will always share a node
                    if( parent[ID1] != src.id ){
                        //store node's ID in next unused element in path1
                        path1[count1] = ID1;
                        //move to the parent and look for its parent
                        ID1 = parent[ID1];
                    }
                    else 
                        //stop if we have backtracked all the way to src
                        stop1 = true;
                }
                
                //Same process as path1 above
                unsigned int path2[v];
                bool stop2 = false;
                //second path starts backtracking from neighbor
                unsigned int ID2 = neighbor.id; 
                path2[0] = ID2; 
                
                int count2 = 0;
                while( !stop2 && count2 < v ){ 
                    count2++;
                    if( parent[ID2] != src.id ){
                        path2[count2] = ID2;
                        ID2 = parent[ID2]; 
                    }
                    else
                        stop2 = true;
                }
                
                //if the paths share a node, then 
                //src->path1(backwards)->currentNode->neighbor->path2->src
                //does not form a cycle. It's possible we found something like
                //src->someNode->currentNode->neighbor->someNode->src
                //which does not imply that src is on a cycle. So we must check if path1
                //and path2 are disjoint (this is why we did not include src in the paths)
                bool shareNode = false;
                
                //compare each filled entry of path1 against path2
                //for src to be on a cycle, we need no nodes in the paths to match
                //NOTE: vectors would be better than arrays for paths, but they were
                //NOTE: throwing memory allocation errors
                for( int i = 0; i < std::max(count1,count2); i++ ){
                    for( int j = 0; j < std::max(count1,count2); j++ ){
                        if( path1[i] == path2[j] )
                            shareNode = true;
                    }
                }
                if( !shareNode ){
                    cycle = true;
                }
            }
        }
        //we are done looking at the neighbors of currentNode
        Q.pop();
    }
    
    if(cycle)
        std::cout << "Node lies on a cycle." << std::endl;
    else
        std::cout << "Node does not lie on a cycle." << std::endl;
    
    return true;
}
