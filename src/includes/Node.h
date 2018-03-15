//
// Created by Han Xiao on 2018-03-15.
//
#include "includes/IBInterface.h"

#ifndef MDPE_NODE_H
#define MDPE_NODE_H

enum STATE{
    RUNNING,
    SLEEPING,
    SHUTDOWN,
    ERROR
};

class Node {

protected:
    Node(std::string host);
    virtual ~Node() = 0;
    std::string host;


public:
    virtual void start() const = 0;
    virtual void shutDown() const = 0;

private:
    STATE currentState;

    virtual void errorExit() const =0;
    virtual void softExit() const = 0;
};


#endif //MDPE_NODE_H
