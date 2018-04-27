//
// Created by Han Xiao on 2018-03-15.
//
#include "includes/Node.h"
#include "includes/IBInterface.h"
#include <cppkafka/cppkafka.h>

#ifndef MDPE_STREAMINGNODE_H
#define MDPE_STREAMINGNODE_H


class StreamingNode : public Node {

public:
    StreamingNode(std::string host,
                  std::string ibhost,
                  int ibport
    );
    StreamingNode();
    ~StreamingNode() override ;
    void start() override ;


private:
    IBInterface * ib;



    cppkafka::Producer * producer;
    cppkafka::Configuration * conf;


    void configureKafkaNode();
    void errorExit() override ;


};


#endif //MDPE_STREAMINGNODE_H
