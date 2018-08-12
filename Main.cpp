
#include <iostream>
#include "StreamingNode.h"
#include <libibc/IB.h>
#include <libibc/ContractSamples.h>
#include <cppkafka/cppkafka.h>

/*
 * TO-DO:
 *      - Refactor the code so that we remove Kafka Streaming Process
 *      - Implement a basic Library from which we can interface and
 * */


int main(int argc, char** argv)
{
   // demoRun(argc, argv);

    StreamingNode * snode = new StreamingNode();
    //snode->connect(snode->getIBHost(), snode->getIBPort());
    snode->start();
    Contract samples = ContractSamples::USOptionContract();
    snode->subscribe((Contract&) samples);



    cppkafka::Configuration config = {{ "metadata.broker.list", "localhost:9092" },
                                      { "group.id", "kafka-consumer-test" },

            // Disable auto commit
                                      { "enable.auto.commit", false },

            // Client group session timeout
                                      { "session.timeout.ms", 300000 }
    };

    cppkafka::Producer producer(config);

    cppkafka::Consumer consumer(config);


    cppkafka::Topic topic = producer.get_topic("testtopic");

    std::string m = "testing";

    producer.produce(cppkafka::MessageBuilder(std::string("testtopic")).partition(0).payload(m));
   // producer.flush();
    std::string cmd;
    while(true){
        std::cin >> cmd;
        consumer.subscribe( { "testtopic" });


        if("q"==cmd){
            break;
        }
        cppkafka::Message msg = consumer.poll();
        if (!msg){
            continue;
        }
        if (msg.get_error()) {
            // rdkafka indicates EOF (end of partition) as errors,
            // which doesn't really mean something went wrong
            if (!msg.is_eof()) {
                // This is an actual error, handle it properly
                std::cerr << msg.get_error();
                break;
            }
            // Regardless, we need to keep going
            continue;
        }
        // We actually received a message, process it
        std::cout<< msg.get_payload() << std::endl;


    }
    delete snode;
    return 0;
}

