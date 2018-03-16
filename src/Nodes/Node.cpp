//
// Created by Han Xiao on 2018-03-15.
//

#include "Node.h"


Node::Node(std::string host) :
host(host),
currentState(SLEEPING)
{}

Node::~Node() {}


STATE Node::getState() const {
    return this->currentState;
}

