#ifndef _CLIENT_H
#define _CLIENT_H

#include "data_feed_file.h"
#include "trade.h"

class Client {
private:
    DataFeedFile feed;
    bool show_trades{false};

public:
    Client(const std::string &, const std::string &);

    Client(const std::string &, const std::string &, bool show_trades);

    bool hasOrder();

    Order &sendOrder();

    void receiveTrade(std::vector <Trade> &);
};

#endif