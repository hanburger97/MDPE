
#include <iostream>
#include "includes/StreamingNode.h"

int main(int argc, char** argv)
{
   // demoRun(argc, argv);

    StreamingNode * snode = new StreamingNode();
    snode->connect(snode->getIBHost(), snode->getIBPort());




    std::cout<< "Compiled and linked... I hope"<<"\n";
    return 0;
}

