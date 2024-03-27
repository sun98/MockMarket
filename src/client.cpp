#include <iostream>

#include "client.h"

Client::Client(const std::string &insert_f, const std::string &cancel_f) {
    feed.initialize(insert_f, cancel_f);
}

Client::Client(const std::string &insert_f, const std::string &cancel_f,
               bool show_trades) : show_trades(show_trades) {
    feed.initialize(insert_f, cancel_f);
}

bool Client::hasOrder() {
    return feed.hasNextOrder();
}

Order &Client::sendOrder() {
    return feed.getNextOrder();
}

void Client::receiveTrade(std::vector <Trade> &new_trades) {
    if (new_trades.size() > 0 && show_trades) {
        std::cout << "client received trades:" << std::endl;
        std::cout << "instrument\tprice\tquantity\task_order_id\tbid_order_id"
                  << std::endl;
        for (auto &trade: new_trades) {
            trade.show();
        }
    }
}
