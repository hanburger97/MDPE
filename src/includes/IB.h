//
// Created by Han Xiao on 2018-02-23.
//

#ifndef MARKET_DATA_TYPE_ENUM
#define MARKET_DATA_TYPE_ENUM
enum MarketDataType {
    REALTIME = 1,
    FROZEN = 2,
    DELAYED = 3,
    DELAYED_FROZEN = 4
};
#endif




#pragma once
#ifndef PIPELINE_IB_H
#define PIPELINE_IB_H

#include "includes/EWrapper.h"
#include "includes/EReaderOSSignal.h"
#include "includes/EReader.h"
#include <atomic>
#include <memory>
#include <vector>






class EClientSocket;

class IB_Client : EWrapper{


private:

    static std::atomic<long> NEXTID;

    EReaderOSSignal sig;
    EClientSocket * const cli;
    EReader * reader;


    // Own Methods


    long getTickerId(){
        return NEXTID ++;
    }


public:
    IB_Client();
    ~IB_Client();

    bool connect(std::string host, int port);

    void disconnect() const;


    //================== Client Methods ==================

    long subscribe(Contract& contract);

    long subscribeOption(Contract& contract);

    bool selectMktDataType(MarketDataType type);

    bool cancelSubscription(int reqId);

    // ================= Virtuals from EWrapper ===========
    void tickPrice( TickerId tickerId, TickType field, double price, int canAutoExecute);



    void openOrderEnd(){};
    void connectionClosed(){};
    void positionEnd(){};
    void accountSummary(int reqId, const std::string& account, const std::string& tag, const std::string& value, const std::string& curency){};
    void accountSummaryEnd( int reqId){};
    void connectAck(){};
    void positionMultiEnd( int reqId){};
    void accountUpdateMultiEnd( int reqId) {};
    void reqCurrentTime(){};
    void tickSize( TickerId tickerId, TickType field, int size) {};
    void tickOptionComputation( TickerId tickerId, TickType tickType, double impliedVol, double delta,
                                        double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice) {};
    void tickGeneric(TickerId tickerId, TickType tickType, double value) {};
    void tickString(TickerId tickerId, TickType tickType, const std::string& value) {};
     void tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const std::string& formattedBasisPoints,
                         double totalDividends, int holdDays, const std::string& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate) {};
     void orderStatus( OrderId orderId, const std::string& status, double filled,
                              double remaining, double avgFillPrice, int permId, int parentId,
                              double lastFillPrice, int clientId, const std::string& whyHeld) {};
     void openOrder( OrderId orderId, const Contract&, const Order&, const OrderState&){};
     void winError( const std::string& str, int lastError) {};
     void updateAccountValue(const std::string& key, const std::string& val,
                                    const std::string& currency, const std::string& accountName) {};
     void updatePortfolio( const Contract& contract, double position,
                                  double marketPrice, double marketValue, double averageCost,
                                  double unrealizedPNL, double realizedPNL, const std::string& accountName) {};
     void updateAccountTime(const std::string& timeStamp) {};
     void accountDownloadEnd(const std::string& accountName) {};
     void nextValidId( OrderId orderId){};
     void contractDetails( int reqId, const ContractDetails& contractDetails) {};
     void bondContractDetails( int reqId, const ContractDetails& contractDetails) {};
     void contractDetailsEnd( int reqId){};
     void execDetails( int reqId, const Contract& contract, const Execution& execution) {};
     void execDetailsEnd( int reqId){};
     void error(const int id, const int errorCode, const std::string errorString){};
     void updateMktDepth(TickerId id, int position, int operation, int side,
                                double price, int size) {};
     void updateMktDepthL2(TickerId id, int position, std::string marketMaker, int operation,
                                  int side, double price, int size){};
     void updateNewsBulletin(int msgId, int msgType, const std::string& newsMessage, const std::string& originExch){};
     void managedAccounts( const std::string& accountsList){};
     void receiveFA(faDataType pFaDataType, const std::string& cxml) {};
     void historicalData(TickerId reqId, const std::string& date, double open, double high,
                                double low, double close, int volume, int barCount, double WAP, int hasGaps) {};
     void scannerParameters(const std::string& xml) {};
     void scannerData(int reqId, int rank, const ContractDetails& contractDetails,
                             const std::string& distance, const std::string& benchmark, const std::string& projection,
                             const std::string& legsStr) {};
     void scannerDataEnd(int reqId) {};
     void realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
                             long volume, double wap, int count) {};
     void currentTime(long time) {};
     void fundamentalData(TickerId reqId, const std::string& data) {};
     void deltaNeutralValidation(int reqId, const UnderComp& underComp) {};
     void tickSnapshotEnd( int reqId) {};
     void marketDataType( TickerId reqId, int marketDataType) {};
     void commissionReport( const CommissionReport& commissionReport) {};
     void position( const std::string& account, const Contract& contract, double position, double avgCost) {};

     void verifyMessageAPI( const std::string& apiData){};
     void verifyCompleted( bool isSuccessful, const std::string& errorText){};
     void displayGroupList( int reqId, const std::string& groups){};
     void displayGroupUpdated( int reqId, const std::string& contractInfo){};
     void verifyAndAuthMessageAPI( const std::string& apiData, const std::string& xyzChallange){};
     void verifyAndAuthCompleted( bool isSuccessful, const std::string& errorText){};
     void positionMulti( int reqId, const std::string& account,const std::string& modelCode, const Contract& contract, double pos, double avgCost){};
     void accountUpdateMulti( int reqId, const std::string& account, const std::string& modelCode, const std::string& key, const std::string& value, const std::string& currency) {};
     void securityDefinitionOptionalParameter(int reqId, const std::string& exchange, int underlyingConId, const std::string& tradingClass, const std::string& multiplier, std::set<std::string> expirations, std::set<double> strikes) {};
     void securityDefinitionOptionalParameterEnd(int reqId) {};
     void softDollarTiers(int reqId, const std::vector<SoftDollarTier> &tiers) {};



};

#endif



