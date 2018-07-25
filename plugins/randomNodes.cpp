//
// Created by anantat 6/22/17
//
/*
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

#include<fstream>
#include "randomNodes.h"
#include "fabric.h"
#include <tulip/GlScene.h>
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"
#include <stdlib.h> 
#include <ctime>
#include <cstdlib>

#include <tulip/BooleanProperty.h>

PLUGIN(randomNodes)

        //Constructor 
randomNodes::randomNodes(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
}

//Tulip's Main Function 
bool randomNodes::run()
{
    assert(graph);
        
    static const size_t STEPS = 6;
    if(pluginProgress)
    {
        pluginProgress->showPreview(false);
        pluginProgress->setComment("Starting to generate random Node Ids...");
        pluginProgress->progress(0, STEPS);
    }

    if(pluginProgress)
    {
        pluginProgress->setComment("Determining suitable range for random numbers..");
        pluginProgress->progress(1, STEPS);
    }
    
    //Store total number of nodes 
    int v = graph->numberOfNodes();
    
    if(pluginProgress)
    {
        pluginProgress->setComment("Range determined..");
        pluginProgress->progress(2, STEPS);
    }
     
   //Apply Tulip's selection algorithm
    tlp::BooleanProperty * pick = graph->getLocalProperty<tlp::BooleanProperty>("viewSelection");
    
    //Initialize iterators for all nodes and all edges
    tlp::Iterator<tlp::node> *itnodes = graph->getNodes();
    tlp::Iterator<tlp::edge> *itedges = graph->getEdges();

    unsigned int randSource = 0; //Random source initialized to 0
    unsigned int randDest = 0; //Random destination initialized to 0

    if(pluginProgress)
    {
        pluginProgress->setComment("Generating random numbers..");
        pluginProgress->progress(3, STEPS);
    }
    
    //Create random numbers of a suitable range
    while (true){
        
        srand(time(NULL));
        randSource = std::rand() % v;
        randDest = std::rand() % v;
        if ((randSource < v) && (randDest != randSource) && (randDest < v)){
            break;
        }
    }
    
    if(pluginProgress)
    {
        pluginProgress->setComment("Random numbers generated..");
        pluginProgress->progress(4, STEPS);
    }

        if(pluginProgress)
    {
        pluginProgress->progress(5, STEPS);
        pluginProgress->setComment("Assigning nodes to random numbers..");
    }
    
    unsigned int maxID = std::max(randSource,randDest);

    //Deselect everything on graph
    
    //Select the nodes corresponding the two random integers (ids) generated above.
    //Deselect all other nodes
    while(itnodes->hasNext()){
        const tlp::node &mynode = itnodes->next();
        if (mynode.id == randSource)
            pick->setNodeValue(mynode,true);
        else if(mynode.id == randDest)
            pick->setNodeValue(mynode,true);
        else
            pick->setNodeValue(mynode,false);
    }
    
    //Deselect all edges
    while(itedges->hasNext()){
        const tlp::edge &myedge = itedges->next();
        pick->setEdgeValue(myedge,false);
    }
    
    if(pluginProgress)
    {
        pluginProgress->setComment("Nodes assigned and selected..");
        pluginProgress->progress(STEPS, STEPS);
    }

    return true;
}
