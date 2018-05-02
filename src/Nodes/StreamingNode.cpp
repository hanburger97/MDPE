//
// Created by Han Xiao on 2018-03-15.
//
#include "StreamingNode.h"


/*
 * A StreamingNode will be a Node that will actively listen to Interactive Broker's
 * call backs and publish it with the
 * */
StreamingNode::StreamingNode():
Node("127.0.0.1")
{}

StreamingNode::StreamingNode(std::string host, std::string ibhost, int ibport):
Node(host)
{}

StreamingNode::~StreamingNode() {

    // idk if this is the correct way to destruct a Producer and Conf instance from RdKafka
    delete conf;
    delete producer;
}



void StreamingNode::start() {

    try{
        configureKafkaNode();
        connect(host, ibport);

    }
    catch (int e){
        if (20==e){
            std::cerr<< "Connection Error"<< std::endl;
            //
        }
    }
    this->currentState = RUNNING;

}

//=============== PRIVATE METHODS==========================//

void StreamingNode::errorExit() {
    // Do stuff
    // Mem management


    //probably will cause seg Fault... idk
    delete this->producer;
    delete this->conf;


    exit(20);
}

void StreamingNode::configureKafkaNode(std::string broker, std::string port) {

    try{

        *conf = {{"metadata.broker.list", broker+":"+port}};
        producer = new cppkafka::Producer(*conf);

    }
    catch (std::string& e){
        std::cerr<<e<<std::endl;
        throw 20;
    }

}



void StreamingNode::stream(Message msg, int partition) {

    std::string topicName = msg.getTopicName();
    std::string key = msg.getKey();
    std::string payload = msg.getPayload();
    cppkafka::MessageBuilder builder(topicName);
    if (partition != -1){
        builder.partition(partition);
    }
    std::string fucku = "shut the fuck up";
    builder.key(fucku);
    builder.payload(fucku);
    producer->produce(builder);
}
























