#ifndef _MARKET_H
#define _MARKET_H

#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "trade.h"
#include "order.h"
#include "client.h"

struct OrderCompare {
    bool operator()(const InsertOrder &a, const InsertOrder &b);
};

enum STATUS_LEVEL {
    STATUS_MIN,
    STATUS_FULL,
};

class Market {
private:
    typedef std::multimap<int, InsertOrder> Book;

    Book bid_book;
    Book ask_book;
    std::unordered_map<int, InsertOrder &> id_book;
    int trade_num{};

    void handleInsertOrder(Order &, std::vector <Trade> &);

    void handleBidOrder(InsertOrder &, std::vector <Trade> &);

    void handleAskOrder(InsertOrder &, std::vector <Trade> &);

    void insertBook(InsertOrder &);

    void handleCancelOrder(Order &);

    void eraseBook(int);

    void eraseByOrderId(Book &, InsertOrder &);

public:
    std::shared_ptr <std::vector<Trade>> handleOrder(Order &);

    void showStatus(STATUS_LEVEL);

    void notifyClient(Client &, std::vector <Trade> &);
};

#endif // _MARKET_H