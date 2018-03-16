//
// Created by Han Xiao on 2018-03-15.
//
#include "includes/Node.h"
#include "includes/IBInterface.h"
#include <librdkafka/rdkafkacpp.h>

#ifndef MDPE_STREAMINGNODE_H
#define MDPE_STREAMINGNODE_H


class StreamingNode : Node {

public:
    StreamingNode(std::string host,
                  std::string ibhost,
                  int ibport
    );
    ~StreamingNode() override ;
    void start();


private:
    IBInterface * ib;
    RdKafka::Conf * conf;
    RdKafka::Producer * producer;


    void configureKafkaNode();
    void errorExit() override ;
};


#endif //MDPE_STREAMINGNODE_H
