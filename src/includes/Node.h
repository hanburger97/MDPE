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
    virtual ~Node();
    std::string host;
    STATE currentState;


public:
    virtual void start() = 0;
    virtual void shutDown() const = 0;
    virtual STATE getState() const = 0;

private:

    virtual void errorExit() =0;
    virtual void softExit() const = 0;
    virtual void handleDisconnection() = 0;

};


#endif //MDPE_NODE_H
