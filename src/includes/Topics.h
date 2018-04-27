//
// Created by Han Xiao on 2018-04-27.
//

#ifndef MDPE_TOPICS_H
#define MDPE_TOPICS_H


#include <string>
#include <vector>

class Topic {
public:

    Topic(std::string topicName);
    virtual ~Topic();
    std::string getName();
    void setParent(Topic * topic);
    void addChildren(Topic * topic);
    std::vector<std::string> getChildren() const;

private:
    Topic * parent;
    std::vector<Topic*> children;
    std::string name;

};


#endif //MDPE_TOPICS_H
