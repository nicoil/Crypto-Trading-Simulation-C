#pragma once
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

class MerkelMain
{
    public:
        MerkelMain() = default;
        void init();
    private:
        void printMenu();
        int getUserOption();
        void printHelp();
        void printStats();
        void placeAsk();
        void placeBid();
        void printWallet();
        void goNext();
        void badValue();
        void processUserOption(int userOption);

        std::string currentTime;
        OrderBook orderBook{"BookEntriesFile.csv"};
        Wallet wallet;
};