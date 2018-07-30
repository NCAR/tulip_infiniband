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
 * refine by Zhenzhen Liu on 6/21/17
 */

#include <tulip/TulipPluginHeaders.h>
#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <map>
#include <vector>

#ifndef LENGTH_BETWEEN_H
#define LENGTH_BETWEEN_H

using namespace tlp;
using namespace std;

class lengthBetween: public tlp::Algorithm{
public:
    PLUGININFORMATION("Length Between Nodes",
                      "Todd Yoder",
                      "07/30/18",
                      "Prints the length of the shortest path between two nodes.",
                      "alpha",
                      "Infiniband")
    
//Constructor
    lengthBetween(tlp::PluginContext* context); 

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
    nodes_map(const Graph *graph, int v){
        initMap(graph, v);
    }

    //Destructor
    ~nodes_map(){
        delete[] adjacent_matrix;
    }

    //Initialize adjacency matrix
    void initMap(const Graph *graph, int v){
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
        tlp::Iterator<edge> *ite = graph->getEdges();
        while(ite->hasNext()){
            edge e = ite->next();
            int s = graph->source(e).id, t = graph->target(e).id;
            if(!adjacent_matrix[s][t]){
                adjacent_matrix[s][t] = 1;
                adjacent_matrix[t][s] = 1;
            }
        }
    }

//Dijkstra Algorithm
    //Calculate minimum distance between two nodes
    int min_distance(map<int, myNode*> map1, bool visited[]);
    
    //Apply Dijkstra's algorithm to entire graph
    map<int,myNode*> dijkstra(int src);

    //Trace the path in Tulip
    vector<unsigned int> tracePath(map<int, myNode*> distmap, int target,int src);
    };

//Main function
    bool run();
    
//Return a node from its id
    const tlp::node & find_node(unsigned int id);
};
#endif //LENGTH_BETWEEN_H
