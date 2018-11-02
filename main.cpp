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

AbstractGraph* findCoreGraph(uint32_t k, uint32_t h, AbstractGraph* fgraph, vector<AbstractGraph*>* coreGraphs, uint32_t** index)
{
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

    //erase unsatisfied vertices
    while(1)
    {
        if(vertexSet->size() == 0)
            break;
        set<uint32_t>::iterator vertex = vertexSet->begin();
        cgraph->eraseVertex(*vertex,vertexSet,k);
        vertexSet->erase(vertex);
    }
    delete vertexSet;
    delete edgeSet;

    coreGraphs->push_back(cgraph);
    index[k][h] = uint32_t(coreGraphs->size()-1); //-1?
    return cgraph;
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


    cout << "----- (k,h)-core Graph -----" << endl;
    //Abstract k-core,h-edge subgraph
    //uint32_t k = 2, h = 2;   //2-neighbor,2-edge;
    vector<AbstractGraph*>* coreGraphs = new vector<AbstractGraph*>;
    uint32_t **index = new uint32_t*[KMAX];
    for(uint32_t i=0; i<KMAX; i++)
    {
        index[i] = new uint32_t[HMAX];
    }
    // k_max now? h_max now?
    /*for(uint32_t k = 1; k < k_max; k++)
    {
        for(uint32_t h = 1; h < h_max; h++)
        {
            if(k==1 && h==1)
                findCoreGraph(k,h,graph,coreGraphs,index);
            else if(h==1 && k>1)
                findCoreGraph(k,h,coreGraphs[index[k-1][h]],coreGraphs,index);
            else
                findCoreGraph(k,h,coreGraphs[index[k][h-1]],coreGraphs,index);
        }

    }*/
    AbstractGraph* core22 = findCoreGraph(2, 2, graph, coreGraphs, index);


    //delete subgraph
    //AbstractGraph* delSubGraph = buildGraph("/home/netlab/C++/GraphFrame/delete.txt");

    //graph.deleteSubgraph();

    cout << endl;
    //cout << "------ " << k << " core " << h << " edge -----" << endl;
    cout << "vertex size: " << graph->getSize() << endl;
    core22->printEdges();




    return 0;
}
