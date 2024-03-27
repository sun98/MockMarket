#ifndef _TRADE_H
#define _TRADE_H

#include <string>

class Trade {
private:
    std::string instr_symbol;
    unsigned int price{};
    unsigned int quantity{};
    unsigned int ask_order_id{};
    unsigned int bid_order_id{};

public:
    Trade(const std::string &instr_symbol, unsigned int price,
          unsigned int quantity, unsigned int ask_order_id,
          unsigned int bid_order_id) : instr_symbol(instr_symbol),
                                       price(price), quantity(quantity), ask_order_id(ask_order_id),
                                       bid_order_id(bid_order_id) {};

    void show() const;
};

#endif
