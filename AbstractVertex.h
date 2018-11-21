#ifndef ABSTRACTVERTEX_H
#define ABSTRACTVERTEX_H
//#include "AbstractEdge.h"
#include <map>
#include <set>
#include <iostream>
#include <stdint.h>
#include "Utils.hpp"
using namespace std;

class AbstractVertex
{
public:
    AbstractVertex(uint32_t vertexId);  //current vertexId
    AbstractVertex(uint32_t vertexId, unordered_map<uint32_t, map<uint32_t,bool*>*> *edgeMap);  //currentVertexId
    AbstractVertex(const AbstractVertex* fvertex);
    ~AbstractVertex();
    bool existEdge(uint32_t vertexId); //adjacent vertexId
    bool existEdgeTime(uint32_t vertexId, uint32_t timestamp); //adjacent vertexId
    bool isSatisfiedVertex(uint32_t k, uint32_t h);
    //queue<uint32_t>* findRedundantNeighbor(uint32_t h);

    uint32_t getVertexId();  //current vertexId
    uint32_t getNeighborNum();
    uint32_t getNeighborEdgeNum(uint32_t vertexId); //neighbor vertexId
    vector<uint32_t>* getAdjacentVertexId();
    void getEdgeSet(set<pair<uint32_t,uint32_t>>* edgeSet);  //<v1,v2>
    void getNeighborEdgeSet(set<pair<uint32_t,uint32_t>>* edgeSet);  //<neighborVertexId, timestamp>
    vector<uint32_t>* getUnsatisfiedNeighborId(uint32_t h);


    //vector<AbstractEdge*>* getEdge(uint32_t vertexId);  //adjacent vertexId
    void insertAdjacentEdge(uint32_t vertexId);  //adjacent vertexId
    void insertAdjacentEdge(uint32_t vertexId, uint32_t timestamp); //adjacent vertexId
    void eraseNeighborVertex(uint32_t vertexId); //adjacent vertexId
    void eraseEdge(uint32_t vertexId, uint32_t timestamp);
    //void eraseVertex();

    void printEdges();
private:
    uint32_t vertexId;  //currentVertexId
    unordered_map<uint32_t, map<uint32_t,bool*>*> *edgeMap;  //key:Adjacent vertexId;
};

#endif // ABSTRACTVERTEX_H
