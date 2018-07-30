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

#pragma once

#include <tulip/TulipPluginHeaders.h>
#include <iostream>
#include <algorithm>

#ifndef IB_RAND_NODES_H
#define IB_RAND_NODES_H

class randomNodes: public tlp::Algorithm {
public:
    PLUGININFORMATION("Two Random Nodes",
    "Ananta Thapaliya & Zenzhen Liu, updated by Todd Yoder",
    "06/16/17, updated 07/24/18",
    "Selects two random nodes from the graph.",
    "alpha",
    "Infiniband")

//Constructor
    randomNodes(tlp::PluginContext* context);

//Tulip's Main Function 
    bool run();
};

#endif // IB_RAND_NODES_H

