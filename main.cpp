#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include "AbstractGraph.h"

using namespace std;

#define KMAX 100
#define HMAX 100

void SplitString(const string& s1, vector<uint32_t>& v, const string& c)
{
    string s = s1;
    for(uint32_t i = 0; i < s.length(); ++i)
    {
        if(s[i] == '\n')
            s = s.replace(i,1,"");
    }
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(uint32_t(atoi(s.substr(pos1, pos2-pos1).c_str())));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(uint32_t(atoi(s.substr(pos1).c_str())));
}

AbstractGraph* buildGraph(string filePath)
{
    //cout << "build graph" << endl;
    //abstract graph
    ifstream ifs(filePath);
    if (!ifs.is_open())
    {
        cout << "Error opening file";
        exit (1);
    }
    string s;
    vector<uint32_t> v;
    AbstractGraph *graph = new AbstractGraph;

    while(getline(ifs,s))
    {
        v.clear();
        SplitString(s, v, " ");  //v1,v2,t
        //cout << "addEdge" << endl;

        graph->addEdge(v[0],v[1],v[2]);
    }
    ifs.close();
    return graph;
}

AbstractGraph* findCoreGraph(uint32_t k, uint32_t h, AbstractGraph* fgraph)
{
    cout << "find " << k << " " << h << "core" << endl;
    AbstractGraph *cgraph = new AbstractGraph(fgraph);
    set<uint32_t> *vertexSet = new set<uint32_t>;
    set<pair<uint32_t,uint32_t>> *edgeSet = new set<pair<uint32_t,uint32_t>>;
    cgraph->unSatisfiedVertexAndEdge(k,h,vertexSet,edgeSet);
    if(vertexSet->size()==cgraph->getVertexNum() || edgeSet->size()==cgraph->getVertexPairNum())
    {
        delete cgraph;
        return nullptr;
    }

    //erase unsatisfied edges
    while(1)
    {
        if(edgeSet->size() == 0)
            break;
        set<pair<uint32_t,uint32_t>>::iterator edge = edgeSet->begin();
        cgraph->eraseEdges(edge->first,edge->second);
        edgeSet->erase(edge);
        if(cgraph->getNeighborNum(edge->first) < k)
            vertexSet->insert(edge->first);
        if(cgraph->getNeighborNum(edge->second) < k)
            vertexSet->insert(edge->second);
    }
    //cout << "test" << endl;
    //erase unsatisfied vertices
    while(1)
    {
        if(vertexSet->size() == 0)
            break;
        set<uint32_t>::iterator vertex = vertexSet->begin();   
        cgraph->eraseVertex(*vertex,vertexSet,k); // erase *vertex and push its unsaitsfied neighbor into vertexSet
        vertexSet->erase(vertex);
    }
    delete vertexSet;
    delete edgeSet;

    if(cgraph->getVertexNum() == 0 || cgraph->getVertexPairNum() == 0)
    {
        delete cgraph;
        return nullptr;
    }

    return cgraph;
}

void treeCoreDecomposition(AbstractGraph* graph,vector<AbstractGraph*>* coreGraphs, uint32_t** index)
{
    AbstractGraph* c11graph = findCoreGraph(1,1,graph);
    coreGraphs->push_back(c11graph);
    index[1][1] = uint32_t(coreGraphs->size()-1);
    queue<AbstractGraph*> graphQueue;
    queue<pair<uint32_t,uint32_t>> khQueue;
    graphQueue.push(c11graph);
    khQueue.push({1,1});

    while(!graphQueue.empty())
    {
        AbstractGraph* cgraph = graphQueue.front();
        graphQueue.pop();
        uint32_t k = khQueue.front().first;
        uint32_t h = khQueue.front().second;
        khQueue.pop();
        AbstractGraph* tgraph;

        if(index[k+1][h] == 0)
        {
            tgraph = findCoreGraph(k+1,h,cgraph);
            if(tgraph != nullptr)
            {
                coreGraphs->push_back(tgraph);
                index[k+1][h] = uint32_t(coreGraphs->size()-1);
                graphQueue.push(tgraph);
                khQueue.push({k+1,h});
            }
        }

        if(index[k][h+1] == 0)
        {
            tgraph = findCoreGraph(k,h+1,cgraph);
            if(tgraph != nullptr)  //? how to express a empty graph
            {
                coreGraphs->push_back(tgraph);
                index[k][h+1] = uint32_t(coreGraphs->size()-1);
                graphQueue.push(tgraph);
                khQueue.push({k,h+1});
            }
        }
    }
}

AbstractGraph* findNeighborGraph(AbstractGraph* subgraph, AbstractGraph* fgraph)
{
    AbstractGraph* ngraph = new AbstractGraph(subgraph);
    set<uint32_t>* vIds = ngraph->getVertexIds();

    set<uint32_t>::iterator it;
    for(it = vIds->begin(); it!= vIds->end(); it++)
    {
        set<pair<uint32_t,uint32_t>>* edgeSet = fgraph->getNeighborEdgeSet(*it);

        set<pair<uint32_t,uint32_t>>::iterator it2;
        for(it2=edgeSet->begin(); it2!=edgeSet->end(); it2++)
        {
            ngraph->addEdge(*it,it2->first,it2->second);
        }

        delete edgeSet;
    }
    delete vIds;

    return ngraph;
}


AbstractGraph* findAproximateCoreGraph(uint32_t k, uint32_t h, AbstractGraph* subgraph, AbstractGraph* fgraph)
{
    AbstractGraph* agraph = new AbstractGraph(subgraph);
    AbstractGraph* ngraph = findNeighborGraph(subgraph, fgraph);
    set<pair<uint32_t,uint32_t>> *edgeSet = agraph->getEdgeSet();
    set<uint32_t>* vertexSet = agraph->getVertexIds();

    set<uint32_t>::iterator it;
    for(it = vertexSet->begin(); it!= vertexSet->end();)
    {
        if(ngraph->isSatisfiedKVertex(*it,k))
            it = vertexSet->erase(it);
        else
            it++;
    }
    set<pair<uint32_t,uint32_t>>::iterator it2;
    for(it2=edgeSet->begin(); it2!=edgeSet->end();)
    {
        if(ngraph->isSatisfiedHEdge(*it2,h))
            it2 = edgeSet->erase(it2);
        else
            it2++;
    }

    if(vertexSet->size()==agraph->getVertexNum() || edgeSet->size()==agraph->getVertexPairNum())
    {
        delete agraph;
        delete vertexSet;
        delete edgeSet;
        delete ngraph;
        return nullptr;
    }

    //erase unsatisfied edges
    while(1)
    {
        if(edgeSet->size() == 0)
            break;
        set<pair<uint32_t,uint32_t>>::iterator edge = edgeSet->begin();
        agraph->eraseEdges(edge->first,edge->second);
        ngraph->eraseEdges(edge->first,edge->second);
        edgeSet->erase(edge);
        if(ngraph->getNeighborNum(edge->first) < k)
            vertexSet->insert(edge->first);
        if(ngraph->getNeighborNum(edge->second) < k)
            vertexSet->insert(edge->second);
    }

    //erase unsatisfied vertices
    while(1)
    {
        if(vertexSet->size() == 0)
            break;
        set<uint32_t>::iterator vertex = vertexSet->begin();
        ngraph->eraseVertex(*vertex);
        agraph->eraseVertex(*vertex,vertexSet,k);// erase *vertex and push its unsaitsfied neighbor into vertexSet
        vertexSet->erase(vertex);
        //cout << "test" << endl;
    }

    delete vertexSet;
    delete edgeSet;
    delete ngraph;  //?

    if(agraph->getVertexNum()==0 || agraph->getVertexPairNum()==0)
    {
        delete agraph;
        return nullptr;
    }

    return agraph;
}

void treeCoreDecompositionOnIncompleteGraph(AbstractGraph* subgraph, AbstractGraph* fgraph,
                                            vector<AbstractGraph*>* approximateCoreGraphs, uint32_t** index)
{
    AbstractGraph* a11graph = findAproximateCoreGraph(1,1,subgraph,fgraph);
    approximateCoreGraphs->push_back(a11graph);
    index[1][1] = uint32_t(approximateCoreGraphs->size()-1);
    queue<AbstractGraph*> graphQueue;
    queue<pair<uint32_t,uint32_t>> khQueue;
    graphQueue.push(a11graph);
    khQueue.push({1,1});

    while(!graphQueue.empty())
    {
        AbstractGraph* cgraph = graphQueue.front(); //current graph
        graphQueue.pop();
        uint32_t k = khQueue.front().first;
        uint32_t h = khQueue.front().second;
        khQueue.pop();

        if(index[k+1][h] == 0)
        {
            AbstractGraph* cNeighborGraph = findNeighborGraph(cgraph,fgraph);
            AbstractGraph* tgraph = findAproximateCoreGraph(k+1,h,cgraph,cNeighborGraph);
            if(tgraph != nullptr)
            {
                graphQueue.push(tgraph);
                khQueue.push({k+1,h});
                approximateCoreGraphs->push_back(tgraph);
                index[k+1][h]=uint32_t(approximateCoreGraphs->size()-1);
            }
            delete cNeighborGraph;
        }

        if(index[k][h+1] == 0)
        {
            AbstractGraph* cNeighborGraph = findNeighborGraph(cgraph,fgraph);
            AbstractGraph* tgraph = findAproximateCoreGraph(k+1,h,cgraph,cNeighborGraph);
            if(tgraph != nullptr)
            {
                graphQueue.push(tgraph);
                khQueue.push({k,h+1});
                approximateCoreGraphs->push_back(tgraph);
                index[k][h+1]=uint32_t(approximateCoreGraphs->size()-1);
            }
            delete cNeighborGraph;
        }
    }
}

AbstractGraph* findCandidateInsertedGraph(AbstractGraph* agraph, AbstractGraph* cgraph_kminus1h, AbstractGraph* cgraph_kh, uint32_t k, uint32_t h)
{
    AbstractGraph* fgraph = new AbstractGraph(agraph);
    set<uint32_t>* vertexSet = fgraph->getVertexIds();
    while(vertexSet->size() != 0)
    {
        set<uint32_t>::iterator it = vertexSet->begin();
        uint32_t v1 = *it;
        set<pair<uint32_t,uint32_t>>* edgeSet = cgraph_kminus1h->getNeighborEdgeSet(v1);

        while(edgeSet->size() != 0)
        {
            set<pair<uint32_t,uint32_t>>::iterator it2 = edgeSet->begin();
            if(cgraph_kh!=nullptr && cgraph_kh->existEdge(*it,it2->first,it2->second))
            {
                edgeSet->erase(it2);
                continue;
            }
            if(cgraph_kminus1h->isSatisfiedKVertex(it2->first,k) && cgraph_kminus1h->isSatisfiedHEdge({*it,it2->first},h))
            {
                uint32_t v2 = it2->first;
                vertexSet->insert(v2);
                fgraph->addEdge(v1,v2,it2->second);
                edgeSet->erase(it2);
                set<pair<uint32_t,uint32_t>>::iterator it3;
                for(it3=edgeSet->begin(); it3!=edgeSet->end();)
                {
                    if(it3->first == v2)
                    {
                        fgraph->addEdge(v1,it3->first,it3->second);
                        it3 = edgeSet->erase(it3);
                    }
                }

            }
        }
        vertexSet->erase(it);
        delete edgeSet;
    }
    delete vertexSet;

    return fgraph;
}


// ograph: current whole graph = lasttime whole graph + inserSubGraph
// solved: (k>1,h)core
// problems to solve: (1,1)core, (1,h>1)core
void updateCoreGraphInsert(vector<AbstractGraph*>* coreGraphs, uint32_t** c_index,AbstractGraph* insertSubGraph, AbstractGraph* ograph)
{
    vector<AbstractGraph*>* approximateCoreGraphs = new vector<AbstractGraph*>;
    uint32_t** a_index = new uint32_t*[KMAX];
    for(uint32_t i=0; i<KMAX; i++)
    {
        a_index[i] = new uint32_t[HMAX]();
    }
    treeCoreDecompositionOnIncompleteGraph(insertSubGraph,ograph,approximateCoreGraphs,a_index);

    vector<AbstractGraph*>::iterator ait;
    uint32_t count = 0;
    uint32_t k=1,h=1;
    for(ait = approximateCoreGraphs->begin(); ait != approximateCoreGraphs->end(); ait++)
    {
        AbstractGraph* agraph = *ait;
        for(k=1; k<KMAX; ++k)
        {
            for(h=1; h<HMAX; ++h)
            {
                if(a_index[k][h] == count)
                    break;
            }
        }
        count++;
        if(c_index[k][h] != 0 || (k==1 && h==1))
        {
            AbstractGraph *cgraph_kminus1h = *coreGraphs->begin()+c_index[k-1][h];
            AbstractGraph *cgraph_kh = *coreGraphs->begin()+c_index[k][h];
            AbstractGraph *fgraph = findCandidateInsertedGraph(agraph,cgraph_kminus1h,cgraph_kh,k,h);
            AbstractGraph* ngraph = findNeighborGraph(fgraph,cgraph_kh);
            AbstractGraph* pgraph = findAproximateCoreGraph(k,h,fgraph,ngraph);
            if(pgraph != nullptr)
                cgraph_kh->joinGraph(pgraph);

            delete fgraph;
            delete ngraph;
            delete pgraph;
        }
        else
        {
            AbstractGraph *cgraph_kminus1h = *coreGraphs->begin()+c_index[k-1][h];
            AbstractGraph *fgraph = findCandidateInsertedGraph(agraph,cgraph_kminus1h,nullptr,k,h);
            AbstractGraph *cgraph_kh = findCoreGraph(k,h,fgraph);
            if(cgraph_kh != nullptr)
            {
                coreGraphs->push_back(cgraph_kh);
            }
            delete fgraph;
        }
    }

    delete approximateCoreGraphs;
    for(uint32_t i=0; i<KMAX; i++)
    {
        delete[] a_index[i];
    }
    delete[] a_index;
}

void updateCoreGraphRemove(vector<AbstractGraph*>* coreGraphs, uint32_t** c_index,AbstractGraph* delSubGraph, AbstractGraph* fgraph)
{
    vector<AbstractGraph*>* approximateCoreGraphs = new vector<AbstractGraph*>;
    uint32_t** a_index = new uint32_t*[KMAX];
    for(uint32_t i=0; i<KMAX; i++)
    {
        a_index[i] = new uint32_t[HMAX]();
    }
    treeCoreDecompositionOnIncompleteGraph(delSubGraph,fgraph,approximateCoreGraphs,a_index);

    vector<AbstractGraph*>::iterator ait;
    uint32_t count = 0;
    uint32_t k=1,h=1;
    for(ait = approximateCoreGraphs->begin(); ait != approximateCoreGraphs->end(); ait++)
    {
        AbstractGraph* agraph = *ait;
        for(k=1; k<KMAX; ++k)
        {
            for(h=1; h<HMAX; ++h)
            {
                if(a_index[k][h] == count)
                    break;
            }
        }
        count++; //index of approximateCoreGraphs
        if(c_index[k][h] == 0)
            if(!(k==1 && h==1))
                continue;
        AbstractGraph *cgraph = *coreGraphs->begin()+c_index[k][h];
        set<pair<uint32_t,uint32_t>>* edgeQueue = new set<pair<uint32_t,uint32_t>>;
        set<uint32_t>* vertexQueue = new set<uint32_t>;

        set<uint32_t>* vIds = agraph->getVertexIds();
        set<uint32_t>::iterator vit;
        for(vit = vIds->begin(); vit!= vIds->end(); vit++)
        {
            set<pair<uint32_t,uint32_t>>* a_edgeSet = agraph->getNeighborEdgeSet(*vit);

            set<pair<uint32_t,uint32_t>>::iterator eit;
            for(eit=a_edgeSet->begin(); eit!=a_edgeSet->end(); eit++)
            {
                if(!cgraph->existEdge(*vit,eit->first,eit->second))
                    continue;
                cgraph->eraseOneEdge(*vit,eit->first,eit->second);
                if(!cgraph->isSatisfiedHEdge({*vit,eit->first},h))
                {
                    if(*vit < eit->first)
                        edgeQueue->insert({*vit,eit->first});
                    else
                        edgeQueue->insert({eit->first,*vit});
                }
                if(!cgraph->isSatisfiedKVertex(*vit,k))
                    vertexQueue->insert(*vit);
                if(!cgraph->isSatisfiedKVertex(eit->first,k))
                    vertexQueue->insert(eit->first);
            }
            delete a_edgeSet;
        }

        //erase unsatisfied edges
        while(1)
        {
            if(edgeQueue->size() == 0)
                break;
            set<pair<uint32_t,uint32_t>>::iterator edge = edgeQueue->begin();
            cgraph->eraseEdges(edge->first,edge->second);
            edgeQueue->erase(edge);
            if(cgraph->getNeighborNum(edge->first) < k)
                vertexQueue->insert(edge->first);
            if(cgraph->getNeighborNum(edge->second) < k)
                vertexQueue->insert(edge->second);
        }
        //cout << "test" << endl;
        //erase unsatisfied vertices
        while(1)
        {
            if(vertexQueue->size() == 0)
                break;
            set<uint32_t>::iterator vertex = vertexQueue->begin();
            cgraph->eraseVertex(*vertex,vertexQueue,k); // erase *vertex and push its unsaitsfied neighbor into vertexSet
            vertexQueue->erase(vertex);
        }

        delete edgeQueue;
        delete vertexQueue;
        delete vIds;
    }

    delete approximateCoreGraphs;
    for(uint32_t i=0; i<KMAX; i++)
    {
        delete[] a_index[i];
    }
    delete[] a_index;
}

/*
void updateCoreGraphInsert()
{

}*/

int main()
{
    AbstractGraph* graph = buildGraph("/home/netlab/C++/GraphFrame/input.txt");
    //AbstractGraph* subgraph = new AbstractGraph(graph);
    //subgraph->eraseVertex(1);

    //cout << "vertex size: " << graph->getSize() << endl;
    cout << "----- Original Graph -----" << endl;
    graph->printEdges();
    //cout << "----- Subgraph ----" << endl;
    //subgraph->printEdges();


    cout << "----- (k,h)-core Graph -----" << endl;
    //Abstract k-core,h-edge subgraph
    //uint32_t k = 2, h = 2;   //2-neighbor,2-edge;
    vector<AbstractGraph*>* coreGraphs = new vector<AbstractGraph*>;
    uint32_t **index = new uint32_t*[KMAX];
    for(uint32_t i=0; i<KMAX; i++)
    {
        index[i] = new uint32_t[HMAX]();
    }

    treeCoreDecomposition(graph,coreGraphs,index);


    //vector<AbstractGraph*>::iterator it = coreGraphs->begin()+index[2][2];
    //AbstractGraph* core22 = *it;


    //delete subgraph
    AbstractGraph* delSubGraph = buildGraph("/home/netlab/C++/GraphFrame/delete.txt");


    //graph.deleteSubgraph();
    AbstractGraph* neighborGraph = findNeighborGraph(delSubGraph,graph);

    cout << "---- neighbor graph ----" << endl;
    neighborGraph->printEdges();




    return 0;
}
