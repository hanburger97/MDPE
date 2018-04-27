//
// Created by Han Xiao on 2018-04-27.
//

#include "Topics.h"

Topic::Topic(std::string topicName) :
name(topicName)
{}

Topic::~Topic() {}

std::string Topic::getName() {
    return name;
}

void Topic::setParent(Topic * tparent) {
    parent = tparent;
    tparent->addChildren(this);
}

void Topic::addChildren(Topic *topic) {
    this->children.push_back(topic);
}

std::vector<std::string> Topic::getChildren() const {
    std::vector<std::string> res;
    for (auto it = children.cbegin(); it != children.cend(); it++){
        res.push_back((*it)->getName());
    }
    return res;
}