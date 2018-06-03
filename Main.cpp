
#include <iostream>
#include "StreamingNode.h"
#include <libibc/IB.h>
#include <libibc/ContractSamples.h>

int main(int argc, char** argv)
{
   // demoRun(argc, argv);

    StreamingNode * snode = new StreamingNode();
    //snode->connect(snode->getIBHost(), snode->getIBPort());
    snode->start();
    Contract samples = ContractSamples::USOptionContract();
    snode->subscribe((Contract&) samples);


    while(true){

    }

    std::cout<< "Compiled and linked... I hope"<<"\n";
    return 0;
}

