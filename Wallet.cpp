#include "Wallet.h"
#include "CSVReader.h"
#include <iostream>

Wallet::Wallet()
{

}

void Wallet::insertCurrency(std::string type, double amount)
{
    double balance;
    if (amount < 0)
    {
        throw std::exception{};
    }
    if (currencies.count(type) == 0)
    {
        balance = 0;
    }
    else
    {
        balance = currencies[type];
    }
    balance = balance + amount;
    currencies[type] = balance;
}

bool Wallet::removeCurrency(std::string type, double amount)
{
    if (amount < 0)
    {
        return false;
    }
    if (currencies.count(type) == 0)
    {
        return false;
    }
    if (checkWallet(type, amount))
    {
        currencies[type] -= amount;
        return true;
    }
    else
    {
        return false;
    }
}

bool Wallet::checkWallet(std::string type, double amount)
{
    if (currencies.count(type) == 0)
    {
        return false;
    }
    else
    {
        return currencies[type] >= amount;
    }
}

bool Wallet::checkOrderStatus(OrderBookEntry order)
{
    std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');
    if (order.orderType == OrderBookType::ask)
    {
        return checkWallet(currs[0], order.amount);
    }
    if (order.orderType == OrderBookType::bid)
    {
        double amount = order.amount * order.price;
        return checkWallet(currs[1], amount);
    }
    else
    {
        return false;
    }
}

void Wallet::processSale(OrderBookEntry &sale)
{
    std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');
    if (sale.orderType == OrderBookType::asksale)
    {
        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[0];
        double incomingAmount = sale.amount * sale.price;
        std::string incomingCurrency = currs[1];
        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
    if (sale.orderType == OrderBookType::bidsale)
    {
        double outgoingAmount = sale.amount * sale.price;
        std::string outgoingCurrency = currs[1];
        double incomingAmount = sale.amount;
        std::string incomingCurrency = currs[0];
        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
}

std::string Wallet::toString()
{
    std::string s;
    for (std::pair<std::string, double> pair: currencies)
    {
        std::string currency = pair.first;
        double amount = pair.second;
        s = s + currency + ": " + std::to_string(amount) + "\n";
    }
    return s;
}

std::ostream &operator<<(std::ostream &os, Wallet &wallet)
{
    os << wallet.toString();
    return os;
}