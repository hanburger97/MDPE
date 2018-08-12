
#include <stdio.h>
#include <iostream>
#include <thread>
#include <ctime>
#include <atomic>

#include "includes/IB.h"
#include "includes/StdAfx.h"


#include "includes/EClientSocket.h"
#include "includes/EPosixClientSocketPlatform.h"

#include "includes/Contract.h"
#include "includes/Order.h"
#include "includes/OrderState.h"
#include "includes/Execution.h"
#include "includes/CommissionReport.h"

#include "includes/ScannerSubscription.h"
#include "includes/executioncondition.h"
#include "includes/PriceCondition.h"
#include "includes/MarginCondition.h"
#include "includes/PercentChangeCondition.h"
#include "includes/TimeCondition.h"
#include "includes/VolumeCondition.h"

#include "includes/CommonDefs.h"

// static atomic long initialization
std::atomic<long> IB_Client::NEXTID(0);
// Since TickerId is really just a long integer... see IB's typedef in commondef.h

IB_Client::IB_Client(EWrapper * ew):
        sig(2000), // Timeout 2 seconds
        cli(new EClientSocket(ew, &sig)),
        reader(new EReader(cli, &sig))
{
}

IB_Client::~IB_Client() {
    delete reader;
    delete cli;
}

bool IB_Client::connect(std::string host, int port) {
    std::cout << "Connecting to " << host << ":"<<port<<std::endl;
    try {
        bool connection_success = cli->eConnect(host.c_str(), port);
        if (!connection_success){throw 20;}
        std::cout<< "Connection [ CONNECTED ]"<<std::endl;

        //reader = new EReader(cli, &sig); // THIS IS DONE IN THE CONSTRUCTOR'S INTIIALIZER

        reader->start();
        return true;

    }
    catch (int e){
        std::cerr<<"Error Connecting "<< e<< std::endl;
        return false;
    }
}


void IB_Client::disconnect() const {
    cli->eDisconnect();
    std::cout<<"Connection [ DISCONNECTED ]"<<std::endl;
}

//=============== Request Operation==========================================================

long IB_Client::subscribe(Contract& contract) {
    /*
     * Pre: a valid Contract object reference of a reference to a sequence of Contract objects
     * Post: Subscribed data to Contract from IB, returned a unique reqId integer to keep track of tick
     *          returns -1 if error
     * */
    long reqId = this->getTickerId();
    try{
        this->cli->reqMktData(reqId, contract, "", false, TagValueListSPtr());
        return reqId;
    }
    catch (std::exception& e){
        return -1;
    }
}

long IB_Client::subscribeOption(Contract& contract) {

    long reqId = this->getTickerId();
    try{
        this->cli->reqContractDetails(reqId, contract);
        return reqId;
    }
    catch (std::exception& e){
        return -1;
    }
}

bool IB_Client::selectMktDataType(MarketDataType type) {
    try{
        short dataType;
        switch (type){
            case REALTIME:
                dataType=1;
                break;
            case FROZEN:
                dataType=2;
                break;
            case DELAYED:
                dataType=3;
                break;
            case DELAYED_FROZEN:
                dataType=4;
                break;
        }
        this->cli->reqMarketDataType(dataType);
    }
    catch (std::exception& e){
        return false;
    }
}

bool IB_Client::cancelSubscription(int reqId) {
    try{
        this->cli->cancelMktData(reqId);
        return true;
    }
    catch (std::exception& e){
        return false;
    }
}

