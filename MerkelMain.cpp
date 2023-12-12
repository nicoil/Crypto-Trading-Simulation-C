#include "MerkelMain.h"
#include "CSVReader.h"
#include <iostream>
#include <vector>

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();
    wallet.insertCurrency("BTC", 10);
    wallet.insertCurrency("DOGE", 69420);
    while (true)
    {
        printMenu();
        int userOption = getUserOption();
        processUserOption(userOption);
    }
}

void MerkelMain::printMenu()
{
    std::cout << "1: Print help" << std::endl;
    std::cout << "2: Print exchange stats" << std::endl;
    std::cout << "3: Place an ask" << std::endl;
    std::cout << "4: Place a bid" << std::endl;
    std::cout << "5: Print wallet" << std::endl;
    std::cout << "6. Continue" << std::endl;

    std::cout << "==========" << std::endl;
    std::cout << "Current time is: " << currentTime << std::endl;
    std::cout << "==========" << std::endl;
    std::cout << "Type in 1-6" << std::endl;
}

int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch(const std::exception& e)
    {

    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - choose options from the menu" << std::endl;
    std::cout << "and follow the on screen instructions." << std::endl;
}

void MerkelMain::printStats()
{
    for (const std::string& p: orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entriesAsk = orderBook.getOrders(OrderBookType::ask, p, currentTime);
        std::vector<OrderBookEntry> entriesBid = orderBook.getOrders(OrderBookType::bid, p, currentTime);
        std::vector<OrderBookEntry> entries;
        for (OrderBookEntry e: entriesAsk)
        {
            entries.push_back(e);
        }
        for (OrderBookEntry e: entriesBid)
        {
            entries.push_back(e);
        }
        std::cout << "Asks seen: " << entriesAsk.size() << std::endl;
        std::cout << "Bids seen: " << entriesBid.size() << std::endl;
        std::cout << "Max price: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min price: " << OrderBook::getLowPrice(entries) << std::endl;
    }
}

void MerkelMain::placeAsk()
{
    std::cout << "Please enter your ask value: product, price, amount. Ex. ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "Bad Line" << std::endl;
    }
    else
    {
        OrderBookEntry obe = CSVReader::stringstoOBE(tokens[1],
                                                     tokens[2],
                                                     currentTime,
                                                     tokens[0],
                                                     OrderBookType::ask);
        obe.username = "simuser";
        if (wallet.checkOrderStatus(obe))
        {
            std::cout << "You can fulfill the order." << std::endl;
            orderBook.insertOrder(obe);
        }
        else
        {
            std::cout << "Insufficient funds." << std::endl;
        }
    }
}

void MerkelMain::placeBid()
{
    std::cout << "Please enter your bid value: product, price, amount. Ex. ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "Bad Line" << std::endl;
    }
    else
    {
        OrderBookEntry obe = CSVReader::stringstoOBE(tokens[1],
                                                     tokens[2],
                                                     currentTime,
                                                     tokens[0],
                                                     OrderBookType::bid);
        obe.username = "simuser";
        if (wallet.checkOrderStatus(obe))
        {
            std::cout << "You can fulfill the order." << std::endl;
            orderBook.insertOrder(obe);
        }
        else
        {
            std::cout << "Insufficient funds." << std::endl;
        }
    }
}

void MerkelMain::printWallet()
{
    std::cout << "Print wallet" << std::endl;
    std::cout << wallet << std::endl;
}

void MerkelMain::goNext()
{
    std::cout << "Continue to the next time frame" << std::endl;
    for (std::string &p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales = orderBook.matchBidAsk(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry &sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            if (sale.username == "simuser")
            {
                wallet.processSale(sale);
            }
        }
    }
    currentTime = orderBook.getNextTime(currentTime);
}

void MerkelMain::badValue()
{
    std::cout << "You entered an invalid value." << std::endl;
    std::cout << "PLease choose 1-6" << std::endl;
}

void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 1)
    {
        printHelp();
    }

    if (userOption == 2)
    {
        printStats();
    }

    if (userOption == 3)
    {
        placeAsk();
    }

    if (userOption == 4)
    {
        placeBid();
    }

    if (userOption == 5)
    {
        printWallet();
    }

    if (userOption == 6)
    {
        goNext();
    }

    if (userOption < 1 || userOption > 6)
    {
        badValue();
    }
}