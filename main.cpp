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

void findCoreGraph(uint32_t k, uint32_t h, AbstractGraph* fgraph, vector<AbstractGraph*>* coreGraphs, uint32_t** index)
{
    cout << "find " << k << " " << h << "core" << endl;
    AbstractGraph *cgraph = new AbstractGraph(fgraph);
    set<uint32_t> *vertexSet = new set<uint32_t>;
    set<pair<uint32_t,uint32_t>> *edgeSet = new set<pair<uint32_t,uint32_t>>;
    cgraph->unSatisfiedVertexAndEdge(k,h,vertexSet,edgeSet);

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
        cout << "test" << endl;
    }
    delete vertexSet;
    delete edgeSet;
    coreGraphs->push_back(cgraph);
    index[k][h] = uint32_t(coreGraphs->size()-1);
    cout << "find " << k << " " << h << "core finished!" << endl;
}

AbstractGraph* findNeighborGraph(AbstractGraph* subgraph, AbstractGraph* fgraph)
{
    AbstractGraph* ngraph = new AbstractGraph(subgraph);
    set<uint32_t>* vIds = ngraph->getVertexIds();

    set<uint32_t>::iterator it;
    for(it = vIds->begin(); it!= vIds->end(); it++)
    {
        set<pair<uint32_t,uint32_t>>* edgeSet = new set<pair<uint32_t,uint32_t>>;
        fgraph->getNeighborEdgeSet(*it,edgeSet);

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


AbstractGraph* findAproximateCoreGraph(uint32_t k, uint32_t h, AbstractGraph* subgraph, AbstractGraph* fgraph, vector<AbstractGraph*>* approximateCoreGraphs, uint32_t** index)
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
        ngraph->eraseVertex(*vertex); // erase *vertex and push its unsaitsfied neighbor into vertexSet
        agraph->eraseVertex(*vertex,vertexSet,k);
        vertexSet->erase(vertex);
        cout << "test" << endl;
    }

    delete vertexSet;
    delete edgeSet;
    delete ngraph;  //?

    return agraph;
}


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

    /*
    cout << "----- (k,h)-core Graph -----" << endl;
    //Abstract k-core,h-edge subgraph
    //uint32_t k = 2, h = 2;   //2-neighbor,2-edge;
    vector<AbstractGraph*>* coreGraphs = new vector<AbstractGraph*>;
    uint32_t **index = new uint32_t*[KMAX];
    for(uint32_t i=0; i<KMAX; i++)
    {
        index[i] = new uint32_t[HMAX];
    }

    // LCD
    uint32_t k_max = 2, h_max = 2; //th maximal degree of original graph
    uint32_t k = 1;
    findCoreGraph(1,1,graph,coreGraphs,index);
    while(k <= k_max)
    {
        if(k != 1)
        {
            vector<AbstractGraph*>::iterator it = coreGraphs->begin()+index[k-1][1];
            findCoreGraph(k,1,*it,coreGraphs,index);
        }
        uint32_t h = 1;
        while(h < h_max)
        {
            vector<AbstractGraph*>::iterator it2 = coreGraphs->begin()+index[k][h];
            findCoreGraph(k,h+1,*it2,coreGraphs,index);
            it2 = coreGraphs->begin()+index[k][h+1];
            if(*it2 == nullptr)
                break;
            h++;
        }
        k++;
    }*/

    // TCD

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
