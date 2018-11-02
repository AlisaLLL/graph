cmake_minimum_required(VERSION 2.8)

project(GraphFrame)
add_executable(${PROJECT_NAME} "main.cpp" "AbstractVertex.h" "AbstractVertex.cpp" "AbstractEdge.h"
    "AbstractEdge.cpp" "Utils.hpp" "AbstractGraph.cpp" "AbstractGraph.h")
