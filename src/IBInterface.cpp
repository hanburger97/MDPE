//
// Created by Han Xiao on 2018-03-15.
//

#include "IBInterface.h"
#include <librdkafka/rdkafkacpp.h>

IBInterface::IBInterface(std::string host, int port) :
host(host),
port(port)
{}

int IBInterface::getPort() {
    return this->port;
}