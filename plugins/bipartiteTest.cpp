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

#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/GlScene.h>
#include <tulip/GraphIterator.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ForEach.h>

#include "bipartiteTest.h"

#include "fabric.h"
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"


PLUGIN(bipartiteTest)

bipartiteTest::bipartiteTest(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
}

//Tulip's main function
bool bipartiteTest::run()
{
    assert(graph);
    
    //Assume graph is bipartite until found otherwise
    bool bipartite = true;
        
    //Store the total number of nodes
    unsigned int v = graph->numberOfNodes();
    
    //Make array with node ids
    tlp::Iterator<tlp::node> *idsIt = graph->getNodes();
    unsigned int ids[v];
    for(int i=0; i<v; i++){
        const tlp::node &tmp = idsIt->next();
        ids[i] = tmp.id;
    }
    delete idsIt;
    
    //Array to store group labels 
    int groups[v];
    
    //Fill group label array with placeholders
    for(int j=0; j<v; j++)
        groups[j] = -1;
    
    //Get first node (should be a better way)
    tlp::Iterator<tlp::node> *itnodes = graph->getNodes();   
    const tlp::node &firstNode = itnodes->next();
    delete itnodes;
    
    //Assign first node to group "0"
    groups[0] = 0;
    
    //Queue to store nodes whos neighbors need checked
    std::queue <tlp::node> nodes;
    nodes.push(firstNode);

    
    while(!nodes.empty()){
        tlp::node currentNode = nodes.front();
        nodes.pop();
        
        //Test for self-loops
        tlp::node inOutNodes;
        forEach(inOutNodes, graph->getInOutNodes(currentNode)){
            if(inOutNodes == currentNode)
                bipartite = false;
        }
        
        //Get index corresponding with ID of current node
        unsigned int currentIndex;
        for( int m=0; m<v; m++){
            if( currentNode.id == ids[m])
                currentIndex = m;
        }
        
        tlp::node neighbor;
        forEach(neighbor, graph->getInOutNodes(currentNode)){
            for( int k=0; k<v; k++){
                if( ids[k] == neighbor.id && groups[k] == -1){ //If unvisited neighbor
                    groups[k] = 1-groups[currentIndex]; //Put neighbor in opposite subset
                    nodes.push(neighbor); //Now neighbor's neighbors will need checked 
                }
                else if( ids[k] == neighbor.id && groups[k] == groups[currentIndex])
                    bipartite = false; //Not bipartite if neighbor and currentNode are in same subset
            }
        }
        
        //Test for other graph components if current one is finished
        if(nodes.empty()){
            bool componentFound = false; 
            tlp::node thisNode;
            forEach(thisNode, graph->getNodes()){
                for( int n=0; n<v; n++){
                    //Assign node to group and check neighbors if it hasn't been grouped yet and is the first node found in a new component
                    if( groups[n] == -1 && thisNode.id == ids[n] && !componentFound){
                        nodes.push(thisNode);
                        groups[n] = 0;
                        componentFound = true;
                    }
                }
            }
        }
    }
    bipartite ? std::cout<< "Graph is bipartite." << std::endl 
        : std::cout << "Graph is not bipartite." << std::endl;
    
    return true;
}
