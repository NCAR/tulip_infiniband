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

#include "degreeMin.h"

#include "fabric.h"
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"


PLUGIN(degreeMin)

//Constructor
degreeMin::degreeMin(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
}

//Tulip's main function
bool degreeMin::run()
{
    assert(graph);
    
    //Initialize min
    unsigned int min = INT_MAX;
    
    //Initialize min node locations
    std::queue<unsigned int> minNodes;
    
    //Pointer to selection property
    tlp::BooleanProperty * pick = graph->getLocalProperty<tlp::BooleanProperty>("viewSelection");
    
    //Deselect all edges
    tlp::edge e;
    forEach(e, graph->getEdges()){
        pick->setEdgeValue(e,false);
    }
    
    //Iterator for all nodes in graph
    tlp::Iterator<tlp::node> *itnodes = graph->getNodes();
    
    //Find min degree value and deselect all nodes
    while(itnodes->hasNext()){
        const tlp::node &currentNode = itnodes->next(); //Iterate nodes
        pick->setNodeValue(currentNode,false); //deselect node
        if(graph->deg(currentNode) <= min) //compare node's degree with running min
             min = graph->deg(currentNode); //update min
    }
    delete itnodes; 
    
    //Select and store nodes with min degree
    tlp::node n;
    forEach(n, graph->getNodes()){
        if(graph->deg(n) == min){ //compare node's degree with global min
            pick->setNodeValue(n,true); //select node
            minNodes.push (n.id); //store node's id
        }
    }
    
    //Print results to output
    std::cout << "Minimum degree: " << min << ", at node(s) " << std::endl;
    while (!minNodes.empty()){
        std::cout << minNodes.front() << " ";
        minNodes.pop();
    }
    std::cout << std::endl;

    return true;
}
