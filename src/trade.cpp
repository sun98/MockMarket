#include <iostream>

#include "trade.h"

void Trade::show() const {
    std::cout << instr_symbol << "\t" << price << "\t" << quantity
              << "\t\t" << ask_order_id << "\t\t" << bid_order_id << std::endl;
}