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

#ifndef GEODESIC_TEST_H
#define GEODESIC_TEST_H


class geodesicTest: public tlp::Algorithm{
public:
    PLUGININFORMATION("Geodesic Test",
                      "Todd Yoder",
                      "07/30/18",
                      "Determines if a selected path is the shortest path between the two end nodes.",
                      "alpha",
                      "Infiniband")
    
//Constructor
    geodesicTest(tlp::PluginContext* context); 

//
//
//TODO: The rest of this (except for bool run()) is copied from lengthBetween.h
//It would be better to call that plugin and use its output
//

    
//Declaration of adjacency matrix
    class nodes_map{
    private:
        int v; //number of nodes in graph;
        int **adjacent_matrix;
    public:
        class myNode{
        private:
            int from;
            int dist;
        public:
            myNode(int from, int dist = INT_MAX){
                this->from = from;
                this->dist = dist;
            };

            void setDist(int d){dist = d; }
            int getDist(){return dist;}

            void setFrom(int f){from = f;}
            int getFrom(){return from;}
        };

    //Constructor
    nodes_map(const tlp::Graph *graph, int v){
        initMap(graph, v);
    }

    //Destructor
    ~nodes_map(){
        delete[] adjacent_matrix;
    }

    //Initialize adjacency matrix
    void initMap(const tlp::Graph *graph, int v){
        this->v = v;
        adjacent_matrix = new int* [v];
        for(int i = 0; i<v; i++){
            adjacent_matrix[i] = new int[v];
        }

        //Initialize adjacency matrix
        for(int i = 0; i<v; i++){
            for(int j = 0; j<v; j++){
                adjacent_matrix[i][j] = 0;
            }
        }

        //Fill adjacency matrix
        tlp::Iterator<tlp::edge> *ite = graph->getEdges();
        while(ite->hasNext()){
            tlp::edge e = ite->next();
            int s = graph->source(e).id, t = graph->target(e).id;
            if(!adjacent_matrix[s][t]){
                adjacent_matrix[s][t] = 1;
                adjacent_matrix[t][s] = 1;
            }
        }
    }

//Dijkstra Algorithm
    //Calculate minimum distance between two nodes
    int min_distance(std::map<int, myNode*> map1, bool visited[]);
    
    //Apply Dijkstra's algorithm to entire graph
    std::map<int,myNode*> dijkstra(int src);

    //Trace the path in Tulip
    std::vector<unsigned int> tracePath(std::map<int, myNode*> distmap, int target,int src);
    };
    
//Return a node from its id
    const tlp::node & find_node(unsigned int id);
    
    
//Main function
    bool run();
    
};
#endif //GEODESIC_TEST_H
