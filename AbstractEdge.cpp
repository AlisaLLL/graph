//
// Created by baiwen on 5/4/18.
//

#include "AbstractEdge.h"

AbstractEdge::AbstractEdge(uint32_t sourceId, uint32_t destinationId, uint32_t timestamp) {
    this->sourceId=sourceId;
    this->destinationId=destinationId;
    this->timestamp=timestamp;
}

AbstractEdge::AbstractEdge(AbstractEdge *edge) {
    this->sourceId=edge->getSourceId();
    this->destinationId=edge->getDestinationId();
    this->timestamp=edge->getTimestamp();
}

AbstractEdge::~AbstractEdge(){

}

uint32_t AbstractEdge::getDestinationId() {
    return this->destinationId;
}

uint32_t AbstractEdge::getSourceId() {
    return this->sourceId;
}

uint32_t AbstractEdge::getTimestamp() {
    return this->timestamp;
}

