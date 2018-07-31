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

#pragma once

#include <tulip/TulipPluginHeaders.h>
#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <map>
#include <vector>

#ifndef NODE_ON_CYCLE_TEST_H
#define N0DE_ON_CYCLE_TEST_H


class nodeOnCycleTest: public tlp::Algorithm{
public:
    PLUGININFORMATION("Node on Cycle Test",
                      "Todd Yoder",
                      "07/31/18",
                      "Determines if the selected node lies on a cycle. Treats graph as undirected.",
                      "alpha",
                      "Infiniband")
    
//Constructor
    nodeOnCycleTest(tlp::PluginContext* context); 

//Main function
    bool run();
    
};
#endif //NODE_ON_CYCLE_TEST_H
