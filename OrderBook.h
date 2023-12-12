#pragma once
#include <vector>
#include <string>
#include "OrderBookEntry.h"
#include "CSVReader.h"

class OrderBook
{
    public:
        OrderBook(std::string filename);
        std::vector<std::string> getKnownProducts();
        std::vector<OrderBookEntry> getOrders(OrderBookType type, std::string product, std::string timestamp);
        std::string getEarliestTime();
        std::string getNextTime(std::string timestamp);
        std::string getPreviousTime(std::string timestamp);
        static double getHighPrice(std::vector<OrderBookEntry> &orders);
        static double getLowPrice(std::vector<OrderBookEntry> &orders);
        double getMedianPrice(std::vector<OrderBookEntry> &orders);
        double getChange(OrderBookType type, std::string product, std::string timestamp);
        void insertOrder(OrderBookEntry &order);
        std::vector<OrderBookEntry> matchBidAsk(std::string product, std::string timestamp);

    private:
        std::vector<OrderBookEntry> orders;
};