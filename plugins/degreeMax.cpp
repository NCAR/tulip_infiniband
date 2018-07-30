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
#include <queue>

#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/GlScene.h>
#include <tulip/GraphIterator.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>

#include "degreeMax.h"

#include "fabric.h"
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"


PLUGIN(degreeMax)

//Constructor
degreeMax::degreeMax(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
}

//Tulip's main function
bool degreeMax::run()
{
    assert(graph);
    
    //Initialize max
    unsigned int max = 0;
    
    //Initialize max node locations
    std::queue<unsigned int> maxNodes;
    
    //Pointer to selection property
    tlp::BooleanProperty * pick = graph->getLocalProperty<tlp::BooleanProperty>("viewSelection");
    
    //Deselect all edges
    tlp::edge e;
    forEach(e, graph->getEdges()){
        pick->setEdgeValue(e,false);
    }
    
    //Iterator for all nodes in graph
    tlp::Iterator<tlp::node> *itnodes = graph->getNodes();
    
    //Find max degree value and deselect all nodes
    while(itnodes->hasNext()){
        const tlp::node &currentNode = itnodes->next(); //Iterate nodes
        pick->setNodeValue(currentNode,false); //deselect node
        if(graph->deg(currentNode) >= max) //compare node's degree with running max
             max = graph->deg(currentNode); //update max
    }
    delete itnodes; 
    
    //Select and store nodes with max degree 
    tlp::node n;
    forEach(n, graph->getNodes()){
        if(graph->deg(n) == max){ //compare node's degree with global max
            pick->setNodeValue(n,true); //select node
            maxNodes.push (n.id); //store node's id
        }
    }
    
    //Print results to output
    std::cout << "Maximum degree: " << max << ", at node(s) " << std::endl;
    while (!maxNodes.empty()){
        std::cout << maxNodes.front() << " ";
        maxNodes.pop();
    }
    std::cout << std::endl;

    return true;
}
