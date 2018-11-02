//
// Created by baiwen on 5/4/18.
//

#ifndef GRAPHFRAME_ABSTRACTEDGE_H
#define GRAPHFRAME_ABSTRACTEDGE_H

#include <stdint.h>

class AbstractEdge {
public:
    AbstractEdge(uint32_t sourceId, uint32_t destinationId, uint32_t timestamp);
    AbstractEdge(AbstractEdge *edge);
    ~AbstractEdge();
    uint32_t getSourceId();
    uint32_t getDestinationId();
    uint32_t getTimestamp();
private:
    uint32_t sourceId;
    uint32_t destinationId;
    uint32_t timestamp;
};


#endif //GRAPHFRAME_ABSTRACTEDGE_H
