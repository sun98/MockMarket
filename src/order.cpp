#include <iostream>

#include "order.h"

void InsertOrder::show() const {
    std::cout << instr_symbol << "," << time << "," << order_id
              << "," << price << "," << quantity << std::endl;
}

void CancelOrder::show() const {
    std::cout << "invalid show for cancel order" << std::endl;
}