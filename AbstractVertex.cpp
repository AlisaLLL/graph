#include "AbstractVertex.h"
using namespace std;

AbstractVertex::AbstractVertex(uint32_t vertexId)
{
    this->vertexId=vertexId;
    this->edgeMap=new unordered_map<uint32_t,map<uint32_t,bool*>*>;
}


AbstractVertex::AbstractVertex(uint32_t vertexId, unordered_map<uint32_t,map<uint32_t,bool*>*>* edgeMap)
{
    this->vertexId=vertexId;
    this->edgeMap=edgeMap;
}

AbstractVertex::AbstractVertex(const AbstractVertex* fvertex)
{
    this->vertexId = fvertex->vertexId;
    this->edgeMap = new unordered_map<uint32_t,map<uint32_t,bool*>*>;
    unordered_map<uint32_t,map<uint32_t,bool*>*>::iterator it;
    for(it=fvertex->edgeMap->begin(); it!=fvertex->edgeMap->end(); it++)
    {
        map<uint32_t,bool*>* m = new map<uint32_t,bool*>;
        map<uint32_t,bool*>::iterator it2;
        for(it2=it->second->begin(); it2!=it->second->end(); it2++)
        {
            m->insert({it2->first,0});
        }
        edgeMap->insert({it->first,m});
    }
}

AbstractVertex::~AbstractVertex()
{
    for(auto pair:*edgeMap)
    {
        //delete unordered_map value : map
        //map value is nullptr,so delete map directly. ???
        for(auto pair2:*pair.second)
            delete pair2.second;
        delete pair.second;
    }
    delete edgeMap;
}

bool AbstractVertex::existEdge(uint32_t vertexId)
{
    if(edgeMap->count(vertexId)==1)
    {
        return true;
    }
    return false;
}

bool AbstractVertex::existEdgeTime(uint32_t vertexId, uint32_t timestamp)
{
    if(!existEdge(vertexId))
       return false;
    if(edgeMap->find(vertexId)->second->count(timestamp) == 1)
        return true;
    return false;
}

uint32_t AbstractVertex::getVertexId()
{
    return this->vertexId;
}

uint32_t AbstractVertex::getNeighborNum()
{
    return uint32_t(edgeMap->size());
}

uint32_t AbstractVertex::getNeighborEdgeNum(uint32_t vertexId)
{
    return uint32_t(edgeMap->find(vertexId)->second->size());
}
vector<uint32_t>* AbstractVertex::getAdjacentVertexId()
{
    if(edgeMap->size()==0)
        return nullptr;
    vector<uint32_t>* v = new vector<uint32_t>;
    unordered_map<uint32_t,map<uint32_t,bool*>*>::iterator it;
    for(it=edgeMap->begin(); it!=edgeMap->end(); it++)
    {
        v->push_back(it->first);
    }
    return v;
}

void AbstractVertex::getEdgeSet(set<pair<uint32_t,uint32_t>>* edgeSet)
{

    unordered_map<uint32_t,map<uint32_t,bool*>*>::iterator it;
    for(it=edgeMap->begin(); it!=edgeMap->end(); it++)
    {
        map<uint32_t,bool*>::iterator it2;
        for(it2=it->second->begin(); it2!=it->second->end(); it2++)
            edgeSet->insert({it->first,it2->first});
    }
}

vector<uint32_t>* AbstractVertex::getUnsatisfiedNeighborId(uint32_t h)
{
    vector<uint32_t>* v = new vector<uint32_t>;
    unordered_map<uint32_t,map<uint32_t,bool*>*>::iterator it;
    for(it=edgeMap->begin(); it!=edgeMap->end(); it++)
    {
        if(it->second->size() < h)
            v->push_back(it->first);
    }
    return v;
}

bool AbstractVertex::isSatisfiedVertex(uint32_t k, uint32_t h)
{
    //cout << "function:isSatisfiedVertex" << endl;
    if(this->getNeighborNum() < k)
        return false;
    unordered_map<uint32_t,map<uint32_t,bool*>*>::iterator it;
    uint32_t count = 0;
    for(it=edgeMap->begin(); it!=edgeMap->end(); it++)
    {
        if(it->second->size() >= h)
            count++;
    }
    if(count < k)
        return false;

    return true;
}

/*queue<uint32_t>* AbstractVertex::findRedundantNeighbor(uint32_t h)
{
    queue<uint32_t> *q = new queue<uint32_t>;
    unordered_map<uint32_t,map<uint32_t,bool*>*>::iterator it;
    for(it=edgeMap->begin(); it!=edgeMap->end(); it++)
    {
        if(it->second->size() < h)
            q->push(it->first);
    }
    return q;
}*/


void AbstractVertex::insertAdjacentEdge(uint32_t vertexId)
{
   if(!edgeMap->count(vertexId))
   {
       edgeMap->insert({vertexId,nullptr});
   }
}

void AbstractVertex::insertAdjacentEdge(uint32_t vertexId, uint32_t timestamp)
{
    //cout << "insertAdjacentEdge: " << vertexId << "timestamp: " << timestamp << endl;
    if(!edgeMap->count(vertexId))   //vertex didn't exist
    {
        map<uint32_t,bool*> *m = new map<uint32_t,bool*>;  //new!!! angry!!!
        m->insert({timestamp,0});
        edgeMap->insert({vertexId,m});
        //cout << m->begin()->first << endl;
    }
    else  //vertex existed, edge didn't exist
    {
        edgeMap->find(vertexId)->second->insert({timestamp,0});
        //cout << edgeMap->find(vertexId)->second->begin()->first << endl;
    }
}

void AbstractVertex::eraseNeighborVertex(uint32_t vertexId)
{
    //cout << "function: Vertex eraseVertex(vertexId)" << vertexId << endl;
    unordered_map<uint32_t,map<uint32_t,bool*>*>::iterator it;
    it = edgeMap->find(vertexId);
    delete it->second;
    it->second = nullptr;
    edgeMap->erase(it);
    //cout << "erase edge " << this->vertexId << " " << vertexId << endl;
    //cout << "function: Vertex eraseVertex(vertexId)" << vertexId << "finished" << endl;
}

void AbstractVertex::printEdges()
{
    //cout << "AbstractVertex printEdges" << endl;
    cout << "v1: " << this->vertexId << " ";

    unordered_map<uint32_t,map<uint32_t,bool*>*>::iterator it;
    for(it=edgeMap->begin(); it!=edgeMap->end(); it++)
    {
        cout << "v2: " << it->first << " " << "timestamps: ";

        map<uint32_t,bool*>::iterator it2;
        for(it2 = it->second->begin(); it2!= it->second->end(); it2++)
            cout << it2->first << " ";
        cout << "    " << endl;
    }
    cout << endl;
}



