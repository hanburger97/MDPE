//
// Created by Han Xiao on 2018-02-23.
//

#pragma once
#ifndef PIPELINE_IB_H
#define PIPELINE_IB_H

#include "EWrapper.h"
#include "EReaderOSSignal.h"
#include "EReader.h"

#include <memory>
#include <vector>

class EClientSocket;


class IB_Client : EWrapper{

public:
    IB_Client();
    ~IB_Client();

    bool connect(std::string host, int port);

    void disconnect() const;

    bool subscribeFutureOptions(
        std::string symbol,
        std::string	expiration,
        double		strike,
        std::string	multiplier,
        std::string	exchange,
        std::string	currency,
        std::string	localSymbol,
        std::string	tradingClass,
        std::string	secIdType,		// CUSIP;SEDOL;ISIN;RIC
        std::string	secId
    );

    void tickDataOperation();
    void marketDepthOperations();
    void realTimeBars();
    void marketDataType();
    void historicalDataRequests();
    void optionsOperations();
    void accountOperations();
    void orderOperations();
    void ocaSamples();
    void conditionSamples();
    void bracketSample();
    void hedgeSample();
    void contractOperations();
    void marketScanners();
    void reutersFundamentals();
    void bulletins();
    void testAlgoSamples();
    void financialAdvisorOrderSamples();
    void financialAdvisorOperations();
    void testDisplayGroups();
    void miscelaneous();

    void reqCurrentTime();

private:

    EReaderOSSignal sig;
    EClientSocket * const cli;

    EReader * reader;


};

#endif //PIPELINE_IB_H





