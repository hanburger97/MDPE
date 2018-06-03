//
// Created by Han Xiao on 2018-03-15.
//
#include "Node.h"
#include "Topics.h"
#include <cppkafka/cppkafka.h>
#include <libibc/IB.h>

#ifndef MDPE_STREAMINGNODE_H
#define MDPE_STREAMINGNODE_H


class StreamingNode : public Node {

protected:

private:


    cppkafka::Producer * producer;
    cppkafka::Configuration * conf;


    void configureKafkaNode(std::string broker="0.0.0.0", std::string port="9092");
    void errorExit() override ;




    std::vector<Topic*> topics;
    std::string kafkaBroker;

public:
    StreamingNode(std::string host,
                  std::string ibhost,
                  int ibport
    );
    StreamingNode();
    ~StreamingNode() override ;
    void start() override ;


    //================== Client Methods ==================





    // ================= Virtuals from EWrapper ===========
    void tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute) override ;
    void tickSize(TickerId tickerId, TickType field, int size) override;
    void tickOptionComputation( TickerId tickerId, TickType tickType, double impliedVol, double delta,
                                double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice) override;
    void tickGeneric(TickerId tickerId, TickType tickType, double value) override;
    void tickString(TickerId tickerId, TickType tickType, const std::string& value) override;
    void tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const std::string& formattedBasisPoints,
                 double totalDividends, int holdDays, const std::string& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate) override;
    void orderStatus(OrderId orderId, const std::string& status, double filled,
                     double remaining, double avgFillPrice, int permId, int parentId,
                     double lastFillPrice, int clientId, const std::string& whyHeld) override;
    void openOrder(OrderId orderId, const Contract&, const Order&, const OrderState&) override;
    void openOrderEnd() override;
    void winError(const std::string& str, int lastError) override;
    void connectionClosed() override;
    void updateAccountValue(const std::string& key, const std::string& val,
                            const std::string& currency, const std::string& accountName) override;
    void updatePortfolio(const Contract& contract, double position,
                         double marketPrice, double marketValue, double averageCost,
                         double unrealizedPNL, double realizedPNL, const std::string& accountName) override;
    void updateAccountTime(const std::string& timeStamp) override;
    void accountDownloadEnd(const std::string& accountName) override;
    void nextValidId(OrderId orderId) override;
    void contractDetails(int reqId, const ContractDetails& contractDetails) override;
    void bondContractDetails(int reqId, const ContractDetails& contractDetails) override;
    void contractDetailsEnd(int reqId) override;
    void execDetails(int reqId, const Contract& contract, const Execution& execution) override;
    void execDetailsEnd(int reqId) override;
    void error(const int id, const int errorCode, const std::string errorString) override;
    void updateMktDepth(TickerId id, int position, int operation, int side,
                        double price, int size) override;
    void updateMktDepthL2(TickerId id, int position, std::string marketMaker, int operation,
                          int side, double price, int size) override;
    void updateNewsBulletin(int msgId, int msgType, const std::string& newsMessage, const std::string& originExch) override;
    void managedAccounts(const std::string& accountsList) override;
    void receiveFA(faDataType pFaDataType, const std::string& cxml) override;
    void historicalData(TickerId reqId, const std::string& date, double open, double high,
                        double low, double close, int volume, int barCount, double WAP, int hasGaps) override;
    void scannerParameters(const std::string& xml) override;
    void scannerData(int reqId, int rank, const ContractDetails& contractDetails,
                     const std::string& distance, const std::string& benchmark, const std::string& projection,
                     const std::string& legsStr) override;
    void scannerDataEnd(int reqId) override;
    void realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
                     long volume, double wap, int count) override;
    void currentTime(long time) override;
    void fundamentalData(TickerId reqId, const std::string& data) override;
    void deltaNeutralValidation(int reqId, const UnderComp& underComp) override;
    void tickSnapshotEnd(int reqId) override;
    void marketDataType(TickerId reqId, int marketDataType) override;
    void commissionReport( const CommissionReport& commissionReport) override;
    void position( const std::string& account, const Contract& contract, double position, double avgCost) override;
    void positionEnd() override;
    void accountSummary( int reqId, const std::string& account, const std::string& tag, const std::string& value, const std::string& curency) override;
    void accountSummaryEnd( int reqId) override;
    void verifyMessageAPI( const std::string& apiData) override;
    void verifyCompleted( bool isSuccessful, const std::string& errorText) override;
    void verifyAndAuthMessageAPI( const std::string& apiData, const std::string& xyzChallenge) override;
    void verifyAndAuthCompleted( bool isSuccessful, const std::string& errorText) override;
    void displayGroupList( int reqId, const std::string& groups) override;
    void displayGroupUpdated( int reqId, const std::string& contractInfo) override;
    void connectAck() override;
    void positionMulti( int reqId, const std::string& account,const std::string& modelCode, const Contract& contract, double pos, double avgCost) override;
    void positionMultiEnd( int reqId) override;
    void accountUpdateMulti( int reqId, const std::string& account, const std::string& modelCode, const std::string& key, const std::string& value, const std::string& currency) override;
    void accountUpdateMultiEnd( int reqId) override;
    void securityDefinitionOptionalParameter(int reqId, const std::string& exchange, int underlyingConId, const std::string& tradingClass, const std::string& multiplier, std::set<std::string> expirations, std::set<double> strikes) override;
    void securityDefinitionOptionalParameterEnd(int reqId) override;
    void softDollarTiers(int reqId, const std::vector<SoftDollarTier> &tiers) override;
    void softExit() const override;
    void shutDown() const override;
    STATE getState() const override;
    void handleDisconnection() override;

};


#endif //MDPE_STREAMINGNODE_H
