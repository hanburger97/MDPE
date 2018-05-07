//
// Created by Han Xiao on 2018-03-15.
//

#include "Node.h"
#include <iostream>
#include <string>

Node::Node(std::string host, EWrapper * ew) :
IB_Client(ew), host(host), khost(host), kport(9002), ibhost(host), ibport(9001),
currentState(SLEEPING)
{}

Node::~Node() {}


STATE Node::getState() const {
    return this->currentState;
}

int Node::getIBPort() const {
    return ibport;
}

std::string Node::getIBHost() const {
    return ibhost;
}

void Node::setIBHost(std::string host, int port) {
    if (""!=ibhost && ibport) {
        ibhost = host;
        ibport = port;
    }
}

int Node::getKPort() const {
    return kport;
}

std::string Node::getKHost() const {
    return khost;
}

void Node::setKHost(std::string host, int port) {
    if (""!=host && port){
        khost = host;
        kport = port;
    }

}


