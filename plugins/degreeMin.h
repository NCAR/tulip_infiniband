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

#ifndef DEGREE_MIN_H
#define DEGREE_MIN_H


class degreeMin: public tlp::Algorithm{
public:
    PLUGININFORMATION("Degree Min",
                      "Todd Yoder",
                      "07/25/18",
                      "Gives the minimum node degree on graph and selects the nodes with the minimum degree",
                      "alpha",
                      "Infiniband")
    
//Constructor
    degreeMin(tlp::PluginContext* context); 

//Main function
    bool run();
    
};
#endif //DEGREE_MIN_H
