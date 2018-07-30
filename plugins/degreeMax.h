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

#ifndef DEGREE_MAX_H
#define DEGREE_MAX_H


class degreeMax: public tlp::Algorithm{
public:
    PLUGININFORMATION("Degree Max",
                      "Todd Yoder",
                      "07/25/18",
                      "Gives the maximum node degree on graph and selects the nodes with the maximum degree",
                      "alpha",
                      "Infiniband")
    
//Constructor
    degreeMax(tlp::PluginContext* context); 

//Main function
    bool run();
    
};
#endif //DEGREE_MAX_H
