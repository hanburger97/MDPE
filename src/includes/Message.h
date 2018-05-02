//
// Created by Han Xiao on 2018-04-27.
//

#ifndef MDPE_MESSAGE_H
#define MDPE_MESSAGE_H

#include <string>
#include "includes/Topics.h"
class Message {
public:
    Message();
    ~Message();
    std::string getKey();
    std::string getPayload();
    Topic * getTopic();
    std::string getTopicName();

private:
    int id;
    std::string key;
    std::string payload;
    Topic * topic;
};


#endif //MDPE_MESSAGE_H
