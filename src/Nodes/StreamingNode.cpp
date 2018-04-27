//
// Created by Han Xiao on 2018-03-15.
//
#include "StreamingNode.h"


/*
 * A StreamingNode will be a Node that will actively listen to Interactive Broker's
 * call backs and publish it with the
 * */
StreamingNode::StreamingNode():
Node("127.0.0.1"),
ib(new IBInterface("127.0.0.1", 9001))
{}

StreamingNode::StreamingNode(std::string host, std::string ibhost, int ibport):
Node(host),
ib(new IBInterface(ibhost, ibport))
{
}

StreamingNode::~StreamingNode() {
    delete ib;

    // idk if this is the correct way to destruct a Producer and Conf instance from RdKafka
    delete conf;
    delete producer;
}


void StreamingNode::start() {

    try{
        configureKafkaNode();
        ib->connectIB();
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
    delete this->ib;


    //probably will cause seg Fault... idk
    delete this->producer;
    delete this->conf;


    exit(20);
}

void StreamingNode::configureKafkaNode() {

    try{

        *conf = {{"metadata.broker.list", "127.0.0.2:9092"}};


    }
    catch (std::string& e){
        std::cerr<<e<<std::endl;
        throw 20;
    }

}



















