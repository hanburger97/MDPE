//
// Created by Han Xiao on 2018-03-15.
//
#include "StreamingNode.h"

/*
 * A StreamingNode will be a Node that will actively listen to Interactive Broker's
 * call backs and publish it with the
 * */

StreamingNode::StreamingNode(std::string host, std::string ibhost, int ibport):
Node(host),
ib(new IBInterface(ibhost, ibport))
{}

StreamingNode::~StreamingNode() {
    delete ib;
}


void StreamingNode::start() const {
    ib->connect(this->host, ib->getPort());

}