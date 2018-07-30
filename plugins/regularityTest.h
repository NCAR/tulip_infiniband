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

#ifndef REGULARITY_TEST_H
#define REGULARITY_TEST_H


class regularityTest: public tlp::Algorithm{
public:
    PLUGININFORMATION("Regularity Test",
                      "Todd Yoder",
                      "07/24/18",
                      "Determines if a graph is irregular or regular and of what degree.",
                      "alpha",
                      "Infiniband")
    
//Constructor
    regularityTest(tlp::PluginContext* context); 

//Main function
    bool run();
    
};
#endif //REGULARITY_TEST_H
