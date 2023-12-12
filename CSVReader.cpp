#include "CSVReader.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader()
{

}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename)
{
    std::string line;
    std::vector<OrderBookEntry> entries;
    std::ifstream csvFile{csvFilename};
    while(getline(csvFile, line))
    {
        std::vector<std::string> tokens = tokenise(line, ',');
        try
        {
            entries.push_back(stringstoOBE(tokens));
        }
        catch(std::exception& e)
        {
            continue;
        }
    }
    return entries;
}

std::vector<std::string> CSVReader::tokenise(std::string s, char separator)
{
    std::vector<std::string> tokens;
    signed int start, end;
    start = s.find_first_not_of(separator, 0);
    do
    {
        std::string token;
        end = s.find_first_of(separator, start);
        if (end == s.length() || start == end)
            break;
        if (end >= 0)
        {
            token = s.substr(start, end - start);
        }
        else
        {
            token = s.substr(start, s.length() - start);
        }
        tokens.push_back(token);
        start = end + 1;
    } while (end != std::string::npos);
    return tokens;
}

OrderBookEntry CSVReader::stringstoOBE(std::vector<std::string> tokens)
{
    double price, amount;
    if (tokens.size() != 5)
    {
        std::cout << "Bad Line" << std::endl;
        throw std::exception{};
    }
    try
    {
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    }
    catch (std::exception &e)
    {
        std::cout << "Bad float! " << tokens[3] << std::endl;
        std::cout << "Bad float! " << tokens[4] << std::endl;
        throw;
    }
    OrderBookEntry obe{
        price,
        amount,
        tokens[0],
        tokens[1],
        OrderBookEntry::stringtoOrderBookType(tokens[2])
    };
    return obe;
}

OrderBookEntry CSVReader::stringstoOBE(std::string priceS,
                                   std::string amountS,
                                   std::string timestamp,
                                   std::string product,
                                   OrderBookType OrderBookType)
{
    double price, amount;
    try
    {
        price = std::stod(priceS);
        amount = std::stod(amountS);
    }
    catch (std::exception &e)
    {
        std::cout << "Bad float! " << priceS << std::endl;
        std::cout << "Bad float! " << amountS << std::endl;
        throw;
    }
    OrderBookEntry obe{
        price,
        amount,
        timestamp,
        product,
        OrderBookType};
    return obe;
}