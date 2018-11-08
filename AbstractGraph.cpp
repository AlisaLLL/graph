#include "AbstractGraph.h"
#include <iostream>

AbstractGraph::AbstractGraph()
{
    vertexMap=new unordered_map<uint32_t,AbstractVertex*>;
}

AbstractGraph::AbstractGraph(unordered_map<uint32_t,AbstractVertex*> *vertexMap)
{
    this->vertexMap=vertexMap;
}

AbstractGraph::AbstractGraph(const AbstractGraph* fgraph)
{
    uint32_t vertexId;
    vertexMap = new unordered_map<uint32_t,AbstractVertex*>;
    unordered_map<uint32_t,AbstractVertex*>::iterator it;
    for(it = fgraph->vertexMap->begin(); it != fgraph->vertexMap->end(); it++)
    {
        vertexId = it->first;
        AbstractVertex* vertex = new AbstractVertex(it->second);
        vertexMap->insert({vertexId,vertex});
    }
}

AbstractGraph::~AbstractGraph()
{
    Utils::releaseMemory(this->vertexMap);
}
/*
AbstractVertex* AbstractGraph::getVertex(uint32_t vertexId)
{
    if(vertexMap->count(vertexId))
    {
        return vertexMap->find(vertexId)->second;
    }
    return nullptr;
}*/

set<uint32_t>* AbstractGraph::getVertexIds()
{
    set<uint32_t>* vIds = new set<uint32_t>;
    unordered_map<uint32_t,AbstractVertex*>::iterator it;
    for(it = vertexMap->begin(); it != vertexMap->end(); it++)
    {
        vIds->insert(it->first);
    }
    return vIds;
}

uint32_t AbstractGraph::getNeighborNum(uint32_t vertexId)
{
    return vertexMap->find(vertexId)->second->getNeighborNum();
}

void AbstractGraph::_addEdge(uint32_t sourceVertexId, uint32_t destVertexId, uint32_t timestamp)
{
    //vertexMap->find(sourceVertexId)->second->insertAdjacentEdge(destVertexId,timestamp);
    AbstractVertex *vertex = getVertex(sourceVertexId);
    if(vertex == nullptr)
    {
        vertex = new AbstractVertex(sourceVertexId);
        vertex->insertAdjacentEdge(destVertexId,timestamp);
        vertexMap->insert({sourceVertexId,vertex});
    }
    else
        vertex->insertAdjacentEdge(destVertexId,timestamp);
}

void AbstractGraph::addEdge(uint32_t sourceVertexId, uint32_t destVertexId, uint32_t timestamp)
{
    _addEdge(sourceVertexId,destVertexId,timestamp);
    _addEdge(destVertexId,sourceVertexId,timestamp);
}

/*void AbstractGraph::addVertex(uint32_t vertexId, AbstractVertex* edgeMap)
{
    vertexMap->insert({vertexId,edgeMap});
}*/

/*vector<AbstractEdge*>* AbstractGraph::getEdge(uint32_t sourceVertexId,uint32_t destinationvertexId)
{
    auto sourceVertex=getVertex(sourceVertexId);
    if(sourceVertex->existEdge(destinationvertexId))
    {
        return sourceVertex->getEdge(destinationvertexId);
    }
    return nullptr;
}*/

uint32_t AbstractGraph::getVertexNum()
{
    return uint32_t(vertexMap->size());
}

uint32_t AbstractGraph::getVertexPairNum()
{
    set<pair<uint32_t,uint32_t>> *edgeSet = getEdgeSet();
    uint32_t num = uint32_t(edgeSet->size());
    delete edgeSet;
    return num;
}

set<pair<uint32_t,uint32_t>>* AbstractGraph::getEdgeSet()
{
    set<pair<uint32_t,uint32_t>>* edgeSet = new set<pair<uint32_t,uint32_t>>;
    unordered_map<uint32_t,AbstractVertex*>::iterator it;
    for(it = vertexMap->begin(); it != vertexMap->end(); it++)
    {
        it->second->getEdgeSet(edgeSet);
    }
    return edgeSet;
}

void AbstractGraph::getNeighborEdgeSet(uint32_t vertexId, set<pair<uint32_t,uint32_t>>* edgeSet)
{
    vertexMap->find(vertexId)->second->getEdgeSet(edgeSet);
}

void AbstractGraph::printEdges()
{
    //cout << "graph printEdges()" << endl;
    unordered_map<uint32_t,AbstractVertex*>::iterator it;
    for(it = vertexMap->begin(); it != vertexMap->end(); it++)
    {
        //cout << "for vertexMap iterator" << endl;
        it->second->printEdges();
    }
}

queue<uint32_t>* AbstractGraph::unSatisfiedVertex(uint32_t k,uint32_t h)
{
    //cout << "function:unSatisfiedVertex" << endl;
    queue<uint32_t> *q = new queue<uint32_t>;
    unordered_map<uint32_t,AbstractVertex*>::iterator it;
    for(it = vertexMap->begin(); it != vertexMap->end(); it++)
    {
        bool isSatisfied = it->second->isSatisfiedVertex(k,h);
        if(!isSatisfied)
        {
            q->push(it->first);
            //cout << "q->push" << it->first << endl;
        }
    }
    return q;
}

bool AbstractGraph::isSatisfiedKVertex(uint32_t vertexId, uint32_t k)
{
    if(vertexMap->find(vertexId)->second->getNeighborNum()<k)
        return false;
    return true;
}

bool AbstractGraph::isSatisfiedHEdge(pair<uint32_t,uint32_t> edge, uint32_t h)
{
    if(vertexMap->find(edge.first)->second->getNeighborEdgeNum(edge.second) < h)
        return false;
    return true;
}


void AbstractGraph::unSatisfiedVertexAndEdge(uint32_t k, uint32_t h, set<uint32_t> *vertexSet, set<pair<uint32_t,uint32_t>> *edgeSet)
{
    unordered_map<uint32_t,AbstractVertex*>::iterator it;
    for(it = vertexMap->begin(); it != vertexMap->end(); it++)
    {
        uint32_t neighborNum = getNeighborNum(it->first);
        if(neighborNum < k)
        {
            vertexSet->insert(it->first);
        }
        vector<uint32_t> *unSatisfiedNeighborId = it->second->getUnsatisfiedNeighborId(h);
        vector<uint32_t>::iterator neighborId;
        for(neighborId=unSatisfiedNeighborId->begin(); neighborId!=unSatisfiedNeighborId->end(); neighborId++)
        {
            if(it->first < *neighborId)
                edgeSet->insert({it->first, *neighborId});
            else
                edgeSet->insert({*neighborId, it->first});
        }
        delete unSatisfiedNeighborId;
    }
}
void AbstractGraph::eraseVertex(uint32_t vertexId)
{
    //erase current vertex from adjacent vertexes(erase edge)
    vector<uint32_t>* v = vertexMap->find(vertexId)->second->getAdjacentVertexId();
    if(v != nullptr)
    {
        vector<uint32_t>::iterator itv;
        for(itv = v->begin(); itv != v->end(); ++itv)
        {
            vertexMap->find(*itv)->second->eraseNeighborVertex(vertexId);
        }
    }

    //erase current vertex
    unordered_map<uint32_t,AbstractVertex*>::iterator it;
    it = vertexMap->find(vertexId);
    //cout << "delete it->second" << endl;
    delete it->second;
    //cout << "delete it->second finished" << endl;
    //it->second = nullptr;
    vertexMap->erase(it);
    //cout << "erase it finished" << endl;
}

void AbstractGraph::eraseVertex(queue<uint32_t>* q)
{
    while(!q->empty())
    {
        //cout << "function: Graph eraseVertex" << endl;
        uint32_t currentVertex = q->front();
        eraseVertex(currentVertex);
        q->pop();
        //cout << "q->pop()" << endl;
    }
}

void AbstractGraph::eraseVertex(uint32_t vertexId, set<uint32_t> *vertexSet, uint32_t k)
{
    eraseVertex(vertexId);
    vector<uint32_t>* v = vertexMap->find(vertexId)->second->getAdjacentVertexId();
    if(v != nullptr)
    {
        vector<uint32_t>::iterator it;
        for(it = v->begin(); it != v->end(); it++)
        {
            if(getNeighborNum(*it) < k)
                vertexSet->insert(*it);
        }
    }
}


void AbstractGraph::eraseEdges(uint32_t v1, uint32_t v2)
{
    vertexMap->find(v1)->second->eraseNeighborVertex(v2);
    vertexMap->find(v2)->second->eraseNeighborVertex(v1);
}

/*
void AbstractGraph::deleteOneEdge(uint32_t u, uint32_t v, uint32_t t)
{
    //find B+: adjacent vertices
    unordered_set<uint32_t> s;
    s.insert(u); s.insert(v);
    vector<uint32_t> *neighbor = vertexMap->find(u)->second->getAdjacentVertexId();
    vector<uint32_t>:: iterator it;
    for(it = neighbor->begin(); it != neighbor->end(); it++)
    {
        s.insert(*it);
    }
    neighbor = vertexMap->find(v)->second->getAdjacentVertexId();
    for(it = neighbor->begin(); it != neighbor->end(); it++)
    {
        s.insert(*it);
    }
}
*/







