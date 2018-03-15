//
// Created by Han Xiao on 2018-03-15.
//
#include "includes/Node.h"
#include "includes/IBInterface.h"


#ifndef MDPE_STREAMINGNODE_H
#define MDPE_STREAMINGNODE_H


class StreamingNode : Node {

public:
    StreamingNode(std::string host,
                  std::string ibhost,
                  int ibport
    );
    ~StreamingNode();
    void start() const;

private:
    IBInterface * ib;
};


#endif //MDPE_STREAMINGNODE_H
