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

    AbstractVertex* getVertex(uint32_t vertexId);
    set<uint32_t>* getVertexIds();
    uint32_t getNeighborNum(uint32_t vertexId);
    vector<uint32_t>* getAdjacentVertexId(uint32_t vertexId);

    //uint32_t getEdgeNum(uint32_t v1, uint32_t v2);
    uint32_t getVertexNum();
    uint32_t getVertexPairNum(); //<v1,v2> pair number,<v1,v2> exist at least an edge
    set<pair<uint32_t,uint32_t>>* getEdgeSet(); // set<(v1,v2)>, v1<v2;
    set<pair<uint32_t,uint32_t>>* getNeighborEdgeSet(uint32_t vertexId); // input currentVertexId,return set(neighborVertexId,timestamp)
    void printEdges();

    void _addEdge(uint32_t sourceVertexId, uint32_t destVertexId, uint32_t timestamp);
    void addEdge(uint32_t sourceVertexId, uint32_t destVertexId, uint32_t timestamp);
    //void addVertex(uint32_t vertexId, AbstractVertex* edgeMap);
    //vector<AbstractEdge*>* getEdge(uint32_t sourceVertexId,uint32_t destinationvertexId);
    void joinGraph(AbstractGraph* graph);


    queue<uint32_t>* unSatisfiedVertex(uint32_t k,uint32_t h); //old

    bool isSatisfiedKVertex(uint32_t vertexId, uint32_t k);
    bool isSatisfiedHEdge(pair<uint32_t,uint32_t> edge, uint32_t h);
    bool existEdge(uint32_t v1, uint32_t v2, uint32_t timestamp);
    bool existVertex(uint32_t vertexId);

    //verex<k,egdes between (v1,v2) < h
    void unSatisfiedVertexAndEdge(uint32_t k, uint32_t h, set<uint32_t> *vertexSet, set<pair<uint32_t,uint32_t>> *edgeSet);

    void eraseVertex(uint32_t vertexId);
    void eraseVertex(uint32_t vertexId, set<uint32_t> *vertexSet, uint32_t k);// erase *vertex and push its unsaitsfied neighbor into vertexSet
    void eraseVertex(queue<uint32_t>* q);
    void eraseEdges(uint32_t v1, uint32_t v2);
    void eraseOneEdge(uint32_t v1, uint32_t v2, uint32_t timestamp);
private:
    unordered_map<uint32_t,AbstractVertex*> *vertexMap;
};

#endif // ABSTRACTGRAPH_H
