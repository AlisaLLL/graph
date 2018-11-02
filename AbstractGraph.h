#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_Hx
#include "AbstractVertex.h"

class AbstractGraph
{
public:
    AbstractGraph();
    AbstractGraph(unordered_map<uint32_t,AbstractVertex*> *vertexMap);
    AbstractGraph(const AbstractGraph* fgraph);
    ~AbstractGraph();

    AbstractVertex* getVertex(uint32_t vertexId); //old
    uint32_t getNeighborNum(uint32_t vertexId);
    //uint32_t getEdgeNum(uint32_t v1, uint32_t v2);
    uint32_t getSize();
    void printEdges();

    void _addEdge(uint32_t sourceVertexId, uint32_t destVertexId, uint32_t timestamp);
    void addEdge(uint32_t sourceVertexId, uint32_t destVertexId, uint32_t timestamp);
    //void addVertex(uint32_t vertexId, AbstractVertex* edgeMap);
    //vector<AbstractEdge*>* getEdge(uint32_t sourceVertexId,uint32_t destinationvertexId);


    queue<uint32_t>* unSatisfiedVertex(uint32_t k,uint32_t h); //old

    void unSatisfiedVertexAndEdge(uint32_t k, uint32_t h, set<uint32_t> *vertexSet, set<pair<uint32_t,uint32_t>> *edgeSet);

    void eraseVertex(uint32_t vertexId);  //old
    void eraseVertex(uint32_t vertexId, set<uint32_t> *vertexSet, uint32_t k);
    void eraseVertex(queue<uint32_t>* q);
    void eraseEdges(uint32_t v1, uint32_t v2);
    //void deleteOneEdge(uint32_t u, uint32_t v, uint32_t t);
private:
    unordered_map<uint32_t,AbstractVertex*> *vertexMap;
};

#endif // ABSTRACTGRAPH_H
