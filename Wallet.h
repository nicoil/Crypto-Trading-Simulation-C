#pragma once
#include <string>
#include <map>
#include "OrderBookEntry.h"

class Wallet
{
    public:
        Wallet();
        void insertCurrency(std::string type, double amount);
        bool removeCurrency(std::string type, double amount);
        bool checkWallet(std::string type, double amount);
        bool checkOrderStatus(OrderBookEntry order);
        void processSale(OrderBookEntry &sale);
        std::string toString();
        friend std::ostream &operator<<(std::ostream &os, Wallet &wallet);

    private:
        std::map<std::string, double> currencies;
};