//
// Created by Han Xiao on 2018-03-15.
//  X&T All Rights Reserved
//

#include "includes/Node.h"
#include "includes/Topics.h"
#include <cppkafka/cppkafka.h>
#include <libibc/IB.h>

#include "StreamingNode.h"


/*
 * A StreamingNode will be a Node that will actively listen to Interactive Broker's
 * call backs and publish it with the
 * */
StreamingNode::StreamingNode():
Node("127.0.0.1", this)
{}

StreamingNode::StreamingNode(std::string host, std::string ibhost, int ibport):
Node(host, this)
{}

StreamingNode::~StreamingNode() {

    // idk if this is the correct way to destruct a Producer and Conf instance from RdKafka
    delete conf;
    delete producer;
}



void StreamingNode::start() {

    try{
        configureKafkaNode();
        connect(host, ibport);

    }
    catch (int e){
        if (20==e){
            std::cerr<< "Connection Error"<< std::endl;
            //
        }
    }
    this->currentState = RUNNING;

}


//=============== PRIVATE METHODS==========================//

void StreamingNode::errorExit() {
    // Do stuff
    // Mem management


    //probably will cause seg Fault... idk
    delete this->producer;
    delete this->conf;


    exit(20);
}

void StreamingNode::configureKafkaNode(std::string broker, std::string port) {

    try{

        *conf = {{"metadata.broker.list", broker+":"+port}};
        producer = new cppkafka::Producer(*conf);

    }
    catch (std::string& e){
        std::cerr<<e<<std::endl;
        throw 20;
    }

}



//Overriding======================


void StreamingNode::error(const int id, const int errorCode, const std::string errorString)
{
    printf( "Error. Id: %d, Code: %d, Msg: %s\n", id, errorCode, errorString.c_str());
}
//! [error]

//! [tickprice]
void StreamingNode::tickPrice( TickerId tickerId, TickType field, double price, int canAutoExecute) {
    printf( "Tick Price. Ticker Id: %ld, Field: %d, Price: %g, CanAutoExecute: %d\n", tickerId, (int)field, price, canAutoExecute);
}
//! [tickprice]

//! [ticksize]
void StreamingNode::tickSize( TickerId tickerId, TickType field, int size) {
    printf( "Tick Size. Ticker Id: %ld, Field: %d, Size: %d\n", tickerId, (int)field, size);
}
//! [ticksize]

//! [tickoptioncomputation]
void StreamingNode::tickOptionComputation( TickerId tickerId, TickType tickType, double impliedVol, double delta,
                                           double optPrice, double pvDividend,
                                           double gamma, double vega, double theta, double undPrice) {
    printf( "TickOptionComputation. Ticker Id: %ld, Type: %d, ImpliedVolatility: %g, Delta: %g, OptionPrice: %g, pvDividend: %g, Gamma: %g, Vega: %g, Theta: %g, Underlying Price: %g\n", tickerId, (int)tickType, impliedVol, delta, optPrice, pvDividend, gamma, vega, theta, undPrice);
}
//! [tickoptioncomputation]

//! [tickgeneric]
void StreamingNode::tickGeneric(TickerId tickerId, TickType tickType, double value) {
    printf( "Tick Generic. Ticker Id: %ld, Type: %d, Value: %g\n", tickerId, (int)tickType, value);
}
//! [tickgeneric]

//! [tickstring]
void StreamingNode::tickString(TickerId tickerId, TickType tickType, const std::string& value) {
    printf( "Tick String. Ticker Id: %ld, Type: %d, Value: %s\n", tickerId, (int)tickType, value.c_str());
}
//! [tickstring]

void StreamingNode::tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const std::string& formattedBasisPoints,
                            double totalDividends, int holdDays, const std::string& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate) {
    printf( "TickEFP. %ld, Type: %d, BasisPoints: %g, FormattedBasisPoints: %s, Total Dividends: %g, HoldDays: %d, Future Last Trade Date: %s, Dividend Impact: %g, Dividends To Last Trade Date: %g\n", tickerId, (int)tickType, basisPoints, formattedBasisPoints.c_str(), totalDividends, holdDays, futureLastTradeDate.c_str(), dividendImpact, dividendsToLastTradeDate);
}

//! [orderstatus]
void StreamingNode::orderStatus(OrderId orderId, const std::string& status, double filled,
                                double remaining, double avgFillPrice, int permId, int parentId,
                                double lastFillPrice, int clientId, const std::string& whyHeld){
    printf("OrderStatus. Id: %ld, Status: %s, Filled: %g, Remaining: %g, AvgFillPrice: %g, PermId: %d, LastFillPrice: %g, ClientId: %d, WhyHeld: %s\n", orderId, status.c_str(), filled, remaining, avgFillPrice, permId, lastFillPrice, clientId, whyHeld.c_str());
}
//! [orderstatus]

//! [openorder]
void StreamingNode::openOrder( OrderId orderId, const Contract& contract, const Order& order, const OrderState& ostate) {}
//! [openorder]

//! [openorderend]
void StreamingNode::openOrderEnd() {
    printf( "OpenOrderEnd\n");
}
//! [openorderend]

void StreamingNode::winError( const std::string& str, int lastError) {}
void StreamingNode::connectionClosed() {
    printf( "Connection Closed\n");
}

//! [updateaccountvalue]
void StreamingNode::updateAccountValue(const std::string& key, const std::string& val,
                                       const std::string& currency, const std::string& accountName) {
    printf("UpdateAccountValue. Key: %s, Value: %s, Currency: %s, Account Name: %s\n", key.c_str(), val.c_str(), currency.c_str(), accountName.c_str());
}
//! [updateaccountvalue]

//! [updateportfolio]
void StreamingNode::updatePortfolio(const Contract& contract, double position,
                                    double marketPrice, double marketValue, double averageCost,
                                    double unrealizedPNL, double realizedPNL, const std::string& accountName){
    printf("UpdatePortfolio. %s, %s @ %s: Position: %g, MarketPrice: %g, MarketValue: %g, AverageCost: %g, UnrealisedPNL: %g, RealisedPNL: %g, AccountName: %s\n", (contract.symbol).c_str(), (contract.secType).c_str(), (contract.primaryExchange).c_str(), position, marketPrice, marketValue, averageCost, unrealizedPNL, realizedPNL, accountName.c_str());
}
//! [updateportfolio]

//! [updateaccounttime]
void StreamingNode::updateAccountTime(const std::string& timeStamp) {
    printf( "UpdateAccountTime. Time: %s\n", timeStamp.c_str());
}
//! [updateaccounttime]

//! [accountdownloadend]
void StreamingNode::accountDownloadEnd(const std::string& accountName) {
    printf( "Account download finished: %s\n", accountName.c_str());
}
//! [accountdownloadend]

//! [contractdetails]
void StreamingNode::contractDetails( int reqId, const ContractDetails& contractDetails) {
    printf( "ContractDetails. ReqId: %d - %s, %s, ConId: %ld @ %s, Trading Hours: %s, Liquidation Hours: %s\n", reqId, contractDetails.summary.symbol.c_str(), contractDetails.summary.secType.c_str(), contractDetails.summary.conId, contractDetails.summary.exchange.c_str(), contractDetails.tradingHours.c_str(), contractDetails.liquidHours.c_str());
}
//! [contractdetails]

void StreamingNode::bondContractDetails( int reqId, const ContractDetails& contractDetails) {
    printf( "Bond. ReqId: %d, Symbol: %s, Security Type: %s, Currency: %s, Trading Hours: %s, Liquidation Hours: %s\n", reqId, contractDetails.summary.symbol.c_str(), contractDetails.summary.secType.c_str(), contractDetails.summary.currency.c_str(), contractDetails.tradingHours.c_str(), contractDetails.liquidHours.c_str());
}

//! [contractdetailsend]
void StreamingNode::contractDetailsEnd( int reqId) {
    printf( "ContractDetailsEnd. %d\n", reqId);
}
//! [contractdetailsend]

//! [execdetails]
void StreamingNode::execDetails( int reqId, const Contract& contract, const Execution& execution) {}
//! [execdetails]

//! [execdetailsend]
void StreamingNode::execDetailsEnd( int reqId) {
    printf( "ExecDetailsEnd. %d\n", reqId);
}
//! [execdetailsend]

//! [updatemktdepth]
void StreamingNode::updateMktDepth(TickerId id, int position, int operation, int side,
                                   double price, int size) {
    printf( "UpdateMarketDepth. %ld - Position: %d, Operation: %d, Side: %d, Price: %g, Size: %d\n", id, position, operation, side, price, size);
}
//! [updatemktdepth]

void StreamingNode::updateMktDepthL2(TickerId id, int position, std::string marketMaker, int operation,
                                     int side, double price, int size) {
    printf( "UpdateMarketDepthL2. %ld - Position: %d, Operation: %d, Side: %d, Price: %g, Size: %d\n", id, position, operation, side, price, size);
}

//! [updatenewsbulletin]
void StreamingNode::updateNewsBulletin(int msgId, int msgType, const std::string& newsMessage, const std::string& originExch) {
    printf( "News Bulletins. %d - Type: %d, Message: %s, Exchange of Origin: %s\n", msgId, msgType, newsMessage.c_str(), originExch.c_str());
}
//! [updatenewsbulletin]

//! [managedaccounts]
void StreamingNode::managedAccounts( const std::string& accountsList) {
    printf( "Account List: %s\n", accountsList.c_str());
}
//! [managedaccounts]

//! [receivefa]
void StreamingNode::receiveFA(faDataType pFaDataType, const std::string& cxml) {
    std::cout << "Receiving FA: " << (int)pFaDataType << std::endl << cxml << std::endl;
}
//! [receivefa]

//! [historicaldata]
void StreamingNode::historicalData(TickerId reqId, const std::string& date, double open, double high,
                                   double low, double close, int volume, int barCount, double WAP, int hasGaps) {
    printf( "HistoricalData. ReqId: %ld - Date: %s, Open: %g, High: %g, Low: %g, Close: %g, Volume: %d, Count: %d, WAP: %g, HasGaps: %d\n", reqId, date.c_str(), open, high, low, close, volume, barCount, WAP, hasGaps);
}
//! [historicaldata]

//! [scannerparameters]
void StreamingNode::scannerParameters(const std::string& xml) {
    printf( "ScannerParameters. %s\n", xml.c_str());
}
//! [scannerparameters]

//! [scannerdata]
void StreamingNode::scannerData(int reqId, int rank, const ContractDetails& contractDetails,
                                const std::string& distance, const std::string& benchmark, const std::string& projection,
                                const std::string& legsStr) {
    printf( "ScannerData. %d - Rank: %d, Symbol: %s, SecType: %s, Currency: %s, Distance: %s, Benchmark: %s, Projection: %s, Legs String: %s\n", reqId, rank, contractDetails.summary.symbol.c_str(), contractDetails.summary.secType.c_str(), contractDetails.summary.currency.c_str(), distance.c_str(), benchmark.c_str(), projection.c_str(), legsStr.c_str());
}
//! [scannerdata]

//! [scannerdataend]
void StreamingNode::scannerDataEnd(int reqId) {
    printf( "ScannerDataEnd. %d\n", reqId);
}
//! [scannerdataend]

//! [realtimebar]
void StreamingNode::realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
                                long volume, double wap, int count) {
    printf( "RealTimeBars. %ld - Time: %ld, Open: %g, High: %g, Low: %g, Close: %g, Volume: %ld, Count: %d, WAP: %g\n", reqId, time, open, high, low, close, volume, count, wap);
}
//! [realtimebar]

//! [fundamentaldata]
void StreamingNode::fundamentalData(TickerId reqId, const std::string& data) {
    printf( "FundamentalData. ReqId: %ld, %s\n", reqId, data.c_str());
}
//! [fundamentaldata]

void StreamingNode::deltaNeutralValidation(int reqId, const UnderComp& underComp) {
    printf( "DeltaNeutralValidation. %d, ConId: %ld, Delta: %g, Price: %g\n", reqId, underComp.conId, underComp.delta, underComp.price);
}

//! [ticksnapshotend]
void StreamingNode::tickSnapshotEnd(int reqId) {
    printf( "TickSnapshotEnd: %d\n", reqId);
}
//! [ticksnapshotend]

//! [marketdatatype]
void StreamingNode::marketDataType(TickerId reqId, int marketDataType) {
    printf( "MarketDataType. ReqId: %ld, Type: %d\n", reqId, marketDataType);
}
//! [marketdatatype]

//! [commissionreport]
void StreamingNode::commissionReport( const CommissionReport& commissionReport) {}
//! [commissionreport]

//! [position]
void StreamingNode::position( const std::string& account, const Contract& contract, double position, double avgCost) {
    printf( "Position. %s - Symbol: %s, SecType: %s, Currency: %s, Position: %g, Avg Cost: %g\n", account.c_str(), contract.symbol.c_str(), contract.secType.c_str(), contract.currency.c_str(), position, avgCost);
}
//! [position]

//! [positionend]
void StreamingNode::positionEnd() {
    printf( "PositionEnd\n");
}
//! [positionend]

//! [accountsummary]
void StreamingNode::accountSummary( int reqId, const std::string& account, const std::string& tag, const std::string& value, const std::string& currency) {
    printf( "Acct Summary. ReqId: %d, Account: %s, Tag: %s, Value: %s, Currency: %s\n", reqId, account.c_str(), tag.c_str(), value.c_str(), currency.c_str());
}
//! [accountsummary]

//! [accountsummaryend]
void StreamingNode::accountSummaryEnd( int reqId) {
    printf( "AccountSummaryEnd. Req Id: %d\n", reqId);
}
//! [accountsummaryend]

void StreamingNode::verifyMessageAPI( const std::string& apiData) {
    printf("verifyMessageAPI: %s\b", apiData.c_str());
}

void StreamingNode::verifyCompleted( bool isSuccessful, const std::string& errorText) {
    printf("verifyCompleted. IsSuccessfule: %d - Error: %s\n", isSuccessful, errorText.c_str());
}

void StreamingNode::verifyAndAuthMessageAPI( const std::string& apiDatai, const std::string& xyzChallenge) {
    printf("verifyAndAuthMessageAPI: %s %s\n", apiDatai.c_str(), xyzChallenge.c_str());
}

void StreamingNode::verifyAndAuthCompleted( bool isSuccessful, const std::string& errorText) {}


//! [displaygrouplist]
void StreamingNode::displayGroupList( int reqId, const std::string& groups) {
    printf("Display Group List. ReqId: %d, Groups: %s\n", reqId, groups.c_str());
}
//! [displaygrouplist]

//! [displaygroupupdated]
void StreamingNode::displayGroupUpdated( int reqId, const std::string& contractInfo) {
    std::cout << "Display Group Updated. ReqId: " << reqId << ", Contract Info: " << contractInfo << std::endl;
}
//! [displaygroupupdated]

//! [positionmulti]
void StreamingNode::positionMulti( int reqId, const std::string& account,const std::string& modelCode, const Contract& contract, double pos, double avgCost) {
    printf("Position Multi. Request: %d, Account: %s, ModelCode: %s, Symbol: %s, SecType: %s, Currency: %s, Position: %g, Avg Cost: %g\n", reqId, account.c_str(), modelCode.c_str(), contract.symbol.c_str(), contract.secType.c_str(), contract.currency.c_str(), pos, avgCost);
}
//! [positionmulti]

//! [positionmultiend]
void StreamingNode::positionMultiEnd( int reqId) {
    printf("Position Multi End. Request: %d\n", reqId);
}
//! [positionmultiend]

//! [accountupdatemulti]
void StreamingNode::accountUpdateMulti( int reqId, const std::string& account, const std::string& modelCode, const std::string& key, const std::string& value, const std::string& currency) {
    printf("AccountUpdate Multi. Request: %d, Account: %s, ModelCode: %s, Key, %s, Value: %s, Currency: %s\n", reqId, account.c_str(), modelCode.c_str(), key.c_str(), value.c_str(), currency.c_str());
}
//! [accountupdatemulti]

//! [accountupdatemultiend]
void StreamingNode::accountUpdateMultiEnd( int reqId) {
    printf("Account Update Multi End. Request: %d\n", reqId);
}
//! [accountupdatemultiend]

//! [securityDefinitionOptionParameter]
void StreamingNode::securityDefinitionOptionalParameter(int reqId, const std::string& exchange, int underlyingConId, const std::string& tradingClass, const std::string& multiplier, std::set<std::string> expirations, std::set<double> strikes) {
    printf("Security Definition Optional Parameter. Request: %d, Trading Class: %s, Multiplier: %s\n", reqId, tradingClass.c_str(), multiplier.c_str());
}
//! [securityDefinitionOptionParameter]

//! [securityDefinitionOptionParameterEnd]
void StreamingNode::securityDefinitionOptionalParameterEnd(int reqId) {
    printf("Security Definition Optional Parameter End. Request: %d\n", reqId);
}
//! [securityDefinitionOptionParameterEnd]

//! [softDollarTiers]
void StreamingNode::softDollarTiers(int reqId, const std::vector<SoftDollarTier> &tiers) {
    printf("Soft dollar tiers (%d):", tiers.size());

    for (int i = 0; i < tiers.size(); i++) {
        printf("%s \n", tiers[0].displayName().c_str());
        //std::cout<<tiers[0].di
    }

}

void StreamingNode::connectAck() {}

void StreamingNode::currentTime(long time) {}

void StreamingNode::nextValidId(OrderId orderId) {}

void StreamingNode::softExit() const {

}

void StreamingNode::shutDown() const{}

STATE StreamingNode::getState() const{}

void StreamingNode::handleDisconnection(){}



















