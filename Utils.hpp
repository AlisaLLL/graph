//
// Created by baiwen on 6/11/18.
//

#ifndef GRAPHFRAME_UTILS_H
#define GRAPHFRAME_UTILS_H

#include <cinttypes>
#include <fstream>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using std::ifstream;
using std::ofstream;
using std::pair;
using std::queue;
using std::stack;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::unordered_set;
using std::unique_ptr;
using std::vector;


class Utils{
public:
    static string concatenateString(string s1,string s2)
    {
        stringstream s;
        s<<s1<<s2;
        return s.str();
    }
    template <typename T1,typename T2>
    static T2 findMaxKey(unordered_map<T1,T2> *map)
    {
        T1 maxKey=map->cbegin()->first;
        T2 maxValue=map->cbegin()->second;
        for(auto pair:*map)
        {
            if(pair.second>maxValue)
            {
                maxKey=pair.first;
                maxValue=pair.second;
            }
        }
        return maxKey;
    }
    template <typename T1,typename T2>
    static T2 findMaxValue(unordered_map<T1,T2> *map)
    {
        T2 maxValue=map->cbegin()->second;
        for(auto pair:*map)
        {
            if(pair.second>maxValue)
            {
                maxValue=pair.second;
            }
        }
        return maxValue;
    }

    template <typename T1,typename T2>
    static T2 findMinValue(unordered_map<T1,T2> *map)
    {
        T2 minValue=map->cbegin()->second;
        for(auto pair:*map)
        {
            if(pair.second<minValue)
            {
                minValue=pair.second;
            }
        }
        return minValue;
    }
    template <typename T1,typename T2>
    static T1 findMinKey(unordered_map<T1,T2> *map)
    {
        T1 minKey=map->cbegin()->first;
        T2 minValue=map->cbegin()->second;
        for(auto pair:*map)
        {
            if(pair.second<minValue)
            {
                minKey=pair.first;
                minValue=pair.second;
            }
        }
        return minKey;
    }
    template <typename T1,typename T2>
    static unordered_set<T1>* findMinKeySet(unordered_map<T1,T2> *map,T2 minValue)
    {
        auto result=new unordered_set<T1>;
        for(auto pair:*map)
        {
            if(pair.second==minValue)
            {
                result->insert(pair.first);
            }
        }
        return result;
    }

    template <typename T>
    static bool isSubSet(unordered_set<T > *set, unordered_set<T> *subset)
    {
        for(auto element:*subset)
        {
            if(set->count(element)==0)
            {
                return false;
            }
        }
        return true;
    }
    template <typename T>
    static unordered_set<T>* intersect(unordered_set<T> *set1, unordered_set<T> *set2)
    {
        auto result=new unordered_set<T>;
        for(auto element:*set1)
        {
            if(set2->count(element)==1)
            {
                result->insert(element);
            }
        }
        return result;
    }

    template <typename T1,typename T2>
    static unordered_set<T1>* intersect(unordered_map<T1 ,T2> *map1, unordered_map<T1,T2> *map2)
    {
        auto result=new unordered_set<T1>;
        for(auto pair1:*map1)
        {
            for(auto pair2:*map2)
            {
                if(pair1.first==pair2.first)
                {
                    result->insert(pair1.first);
                }
            }
        }
        return result;
    }
    template<typename T>
    static void releaseMemory(unordered_set<T>* elementSet)
    {
        for(auto element:*elementSet)
        {
            delete element;
        }
        delete elementSet;
    }
    template<typename T1,typename T2>
    static void releaseMemory(unordered_map<T1,T2>* pairMap)
    {
        for(auto pair:*pairMap)
        {
            delete pair.second;
        }
        delete pairMap;
    }
};
#endif //GRAPHFRAME_UTILS_H
