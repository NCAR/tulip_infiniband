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

#include "regularityTest.h"

#include "fabric.h"
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"


PLUGIN(regularityTest)

regularityTest::regularityTest(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
}

//Tulip's main function
bool regularityTest::run()
{
    assert(graph);
    
    //Store the total number of nodes
    unsigned int v = graph->numberOfNodes();
    
    //Variable to indicate two distinct node degrees
    bool reg = true;
    
    //Variable to indicate two equal node degrees
    bool irreg = true;
    
    //Array to store each node's degree
    unsigned int deg[v];
    
    //Iterator for all nodes in graph
    tlp::Iterator<tlp::node> *itnodes = graph->getNodes();
    
    //Iteration counter
    unsigned int i = 0;
    
    while(itnodes->hasNext()){
        const tlp::node &currentNode = itnodes->next(); //Iterate nodes
            deg[i] = graph->deg(currentNode); //Store degree of each node
        if(graph->deg(currentNode) != deg[0]) 
            reg = false; //not regular if any two degrees don't match
        for(int j=0; j<i; j++){ //compare node's degree to all previous nodes' degrees
            if(graph->deg(currentNode) == deg[j]) 
                irreg = false; //not irregular if any two degrees match
        }
        i++;
    }
    
    //Print result to output
    if(irreg)
        std::cout << "Graph is irregular." << std::endl;
    else if(reg)
        std::cout << "Graph is " << deg[0] << "-regular." << std::endl;
    else
        std::cout << "Graph is neither regular nor irregular." << std::endl;
    
    return true;
}
