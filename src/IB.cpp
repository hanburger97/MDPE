
#include <stdio.h>
#include <iostream>
#include <thread>
#include <ctime>

#include "IB.h"
#include "StdAfx.h"


#include "EClientSocket.h"
#include "EPosixClientSocketPlatform.h"

#include "Contract.h"
#include "Order.h"
#include "OrderState.h"
#include "Execution.h"
#include "CommissionReport.h"
#include "ContractSamples.h"
#include "OrderSamples.h"
#include "ScannerSubscription.h"
#include "ScannerSubscriptionSamples.h"
#include "executioncondition.h"
#include "PriceCondition.h"
#include "MarginCondition.h"
#include "PercentChangeCondition.h"
#include "TimeCondition.h"
#include "VolumeCondition.h"
#include "AvailableAlgoParams.h"
#include "FAMethodSamples.h"
#include "CommonDefs.h"
#include "AccountSummaryTags.h"


IB_Client::IB_Client():
        sig(2000), // Timeout 2 seconds
        cli(new EClientSocket(this, &sig))
{
}

// Destructor... careful w/ mem management
IB_Client::~IB_Client() {
    if (reader){
        delete reader;
    }
    delete cli;
}

bool IB_Client::connect(std::string host, int port) {
    std::cout << "Connecting to " << host << ":"<<port<<std::endl;
    try {
        bool connection_success = cli->eConnect(host.c_str(), port, false);
        if (!connection_success){throw 20}
        std::cout<< "Connection [ CONNECTED ]"<<std::endl;

        reader = new EReader(cli, &sig);
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

int IB_Client::subscribeFutureOptions(
                                 int id,
                                 double strike,
                                 char * const type,
                                 std::string symbol,
                                 std::string expiration,
                                 std::string exchange,
                                 std::string multiplier,
                                 std::string currency
                                 )
{

    id = (id && id >= 0) ? id : 1000;
    // Initializing Contract Instance
    Contract contract;
    contract.symbol = symbol;
    contract.secType = "FOP";
    contract.exchange = exchange;
    contract.currency = currency;
    contract.strike = strike;
    contract.right = std::string(type);
    contract.multiplier = multiplier;

    try{
        cli->reqContractDetails(id, contract);
        return id;
    }
    catch (int e){
        std::cerr << "Error "<< e << std::endl;
    }

}

int IB_Client::subscribeWholeFutureOptions(
        int id,
        std::string symbol,
        std::string exchange,
        std::string currency
){
        Contract contract;
        contract.symbol = symbol;
        contract.primaryExchange = exchange;
        contract.currency = currency;
    
        try{
            cli->reqContractDetails(id, contract);
            return id;
        }
        catch (int e){
            std::cerr << e << std::endl;
        }
}

// BUilding binding for Kafka streaming on EReader and EWrapper


