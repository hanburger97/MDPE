//
// Created by Han Xiao on 2018-03-15.
//
#include "includes/Node.h"
#include "includes/Topics.h"
#include "includes/Message.h"
#include <cppkafka/cppkafka.h>
#include <libibc/IB.h>

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

protected:
    void stream(Message msg, int partition = -1);

private:


    cppkafka::Producer * producer;
    cppkafka::Configuration * conf;


    void configureKafkaNode(std::string broker="127.0.0.2", std::string port="9092");
    void errorExit() override ;




    std::vector<Topic*> topics;
    std::string kafkaBroker;
};


#endif //MDPE_STREAMINGNODE_H
