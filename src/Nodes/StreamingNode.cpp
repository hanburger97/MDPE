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
{
}

StreamingNode::~StreamingNode() {
    delete ib;

    // idk if this is the correct way to destruct a Producer and Conf instance from RdKafka
    delete conf;
    delete producer;
}


void StreamingNode::start(){

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
        using namespace RdKafka;
        this->conf = Conf::create(Conf::CONF_GLOBAL);
        std::string errstr  = "";

        this->producer = Producer::create(this->conf, errstr);
        if (NULL == producer){
            throw errstr;
        }

    }
    catch (std::string& e){
        std::cerr<<e<<std::endl;
        throw 20;
    }

}