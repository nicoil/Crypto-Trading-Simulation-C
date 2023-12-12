#include "OrderBook.h"
#include <map>
#include <algorithm>
#include <iostream>

OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}

std::vector<std::string> OrderBook::getKnownProducts()
{
    std::map<std::string, bool> prodMap;
    for (const OrderBookEntry& e: orders)
    {
        prodMap[e.product] = true;
    }
    std::vector<std::string> products;
    for (const auto& e : prodMap)
    {
        products.push_back(e.first);
    }
    return products;
}

std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, std::string product, std::string timestamp)
{
    std::vector<OrderBookEntry> entries;
    for (const OrderBookEntry& e: orders)
    {
        if(type == e.orderType && product == e.product && timestamp == e.timestamp)
        {
            entries.push_back(e);
        }
    }
    return entries;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry> &orders)
{
    double max = orders[0].price;
    for (const OrderBookEntry& e: orders)
    {
        if (e.price > max)
        {
            max = e.price;
        }
    }
    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry> &orders)
{
    double min = orders[0].price;
    for (const OrderBookEntry& e: orders)
    {
        if (e.price < min)
        {
            min = e.price;
        }
    }
    return min;
}

double OrderBook::getMedianPrice(std::vector<OrderBookEntry> &orders)
{
    double minPrice = getLowPrice(orders);
    double maxPrice = getHighPrice(orders);
    double median = (minPrice + maxPrice) / 2;
    return median;
}

std::string OrderBook::getEarliestTime()
{
    std::string currentTime = orders[0].timestamp;
    for (OrderBookEntry& o: orders)
    {
        if (o.timestamp < currentTime)
        {
            currentTime = o.timestamp;
        }
    }
    return currentTime;
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string nextTime = "";
    for (OrderBookEntry& o: orders)
    {
        if (o.timestamp > timestamp)
        {
            nextTime = o.timestamp;
            break;
        }
    }
    if (nextTime == "")
    {
        nextTime = getEarliestTime();
    }
    return nextTime;
}

std::string OrderBook::getPreviousTime(std::string timestamp)
{
    std::string previousTime = "";
    for (OrderBookEntry &o : orders)
    {
        if (o.timestamp < timestamp)
        {
            previousTime = o.timestamp;
        }
    }
    if (previousTime == "")
    {
        previousTime = getEarliestTime();
    }
    return previousTime;
}

double OrderBook::getChange(OrderBookType type, std::string product, std::string timestamp)
{
    std::vector<OrderBookEntry> currentPrices;
    std::vector<OrderBookEntry> previousPrices;
    for (const OrderBookEntry &e:orders)
    {
        if (type == e.orderType && product == e.product && e.timestamp == timestamp)
        {
            currentPrices.push_back(e);
        }
        if (type == e.orderType && product == e.product && e.timestamp == getPreviousTime(timestamp))
        {
            previousPrices.push_back(e);
        }
    }
    return (getMedianPrice(currentPrices) - getMedianPrice(previousPrices)) / 2;
}

void OrderBook::insertOrder(OrderBookEntry &order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchBidAsk(std::string product, std::string timestamp)
{
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask,
                                                 product,
                                                 timestamp);
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid,
                                                 product,
                                                 timestamp);
    std::vector<OrderBookEntry> sales;
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    for (OrderBookEntry &ask : asks)
    {
        for (OrderBookEntry &bid : bids)
        {
            if (bid.price >= ask.price)
            {
                OrderBookEntry sale{ask.price, 0, timestamp, product, OrderBookType::asksale};
                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::asksale;
                }
                if (bid.amount == ask.amount)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = 0;
                    break;
                }
                if (bid.amount > ask.amount)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = bid.amount - ask.amount;
                    break;
                }
                if (bid.amount < ask.amount && bid.amount > 0)
                {
                    sale.amount = bid.amount;
                    sales.push_back(sale);
                    ask.amount = ask.amount - bid.amount;
                    bid.amount = 0;
                    continue;
                }
            }
        }
    }
    return sales;
}