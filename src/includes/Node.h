//
// Created by Han Xiao on 2018-03-15.
//
#include <libibc/IB.h>
#ifndef MDPE_NODE_H
#define MDPE_NODE_H

enum STATE{
    RUNNING,
    SLEEPING,
    SHUTDOWN,
    ERROR
};

class Node : public IB_Client {

protected:

    Node(std::string host);
    virtual ~Node();
    STATE currentState;

    // For our current host
    std::string host;

    // For our IB Host
    std::string ibhost;
    int ibport;

    // For Kafka Brokerage Host
    std::string khost;
    int kport;

public:

    virtual void start() = 0;
    virtual void shutDown() const = 0;

    virtual STATE getState() const = 0;

    // For IB host
    std::string getIBHost() const;
    int getIBPort() const;
    void setIBHost(std::string host=ibhost, int port = ibport);

    // For Kafka Host
    std::string getKHost() const;
    int getKPort() const;
    void setKHost(std::string host=khost, int port = kport);



private:

    virtual void errorExit() =0;
    virtual void softExit() const = 0;
    virtual void handleDisconnection() = 0;




};


#endif //MDPE_NODE_H
