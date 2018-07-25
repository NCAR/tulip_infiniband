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
    
    //Variable to store the first node's degrees
    unsigned int deg;
    
    //Iterator for all nodes in graph
    tlp::Iterator<tlp::node> *itnodes = graph->getNodes();
    
    //Iteration counter
    unsigned int i = 0;
    
    while(itnodes->hasNext() && reg){
        const tlp::node &node = itnodes->next(); //Iterate nodes
        if(i==0)
            deg = graph->deg(node); //Set deg as degree of the first node
        else if(graph->deg(node) != deg) 
            reg = false; //quit loop if degree doesn't match deg
        i++;
    }
    
    //No nontrivial graph is irregular
    if(v>1)
        irreg = false;
    
    //Print result to output
    if(irreg)
        std::cout << "Graph is irregular." << std::endl;
    else if(reg)
        std::cout << "Graph is " << deg << "-regular." << std::endl;
    else
        std::cout << "Graph is neither regular nor irregular." << std::endl;
    
    return true;
}
