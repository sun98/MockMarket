#include <iostream>

#include "market.h"

bool OrderCompare::operator()(const InsertOrder &a, const InsertOrder &b) {
    return a.price < b.price;
}

std::shared_ptr <std::vector<Trade>> Market::handleOrder(Order &order) {
    auto new_trades = std::make_shared < std::vector < Trade >> ();
    if (order.getType() == ORDER_INSERT) {
        handleInsertOrder(order, *new_trades);
    } else if (order.getType() == ORDER_CANCEL) {
        handleCancelOrder(order);
    } else {
        std::cout << "invalid order: " << order.order_id << std::endl;
    }
    trade_num += new_trades->size();
    return new_trades;
}

void Market::handleInsertOrder(Order &order,
                               std::vector <Trade> &new_trades) {
    auto &i_order = dynamic_cast<InsertOrder &>(order);
    if (i_order.type == INSERT_BID) {
        handleBidOrder(i_order, new_trades);
    } else if (i_order.type == INSERT_ASK) {
        handleAskOrder(i_order, new_trades);
    } else {
        std::cout << "invalid insert order: " << order.order_id << std::endl;
    }
}

void Market::handleCancelOrder(Order &order) {
    /* NOTE: do not care about the order quantity */
    eraseBook(order.order_id);
}

void Market::handleBidOrder(InsertOrder &bid_order,
                            std::vector <Trade> &new_trades) {
    if (ask_book.size() == 0) {
        /* there is no ask order */
        insertBook(bid_order);
        return;
    }
    /* find the lowest ask order that is lower than bid price to buy */
    while (bid_order.quantity > 0) {
        auto best_ask_order = ask_book.begin();
        if (best_ask_order == ask_book.end() || best_ask_order->first > bid_order.price) {
            /* there is no ask order meet the requirement */
            break;
        }
        if (best_ask_order->second.quantity > bid_order.quantity) {
            /* the bid order is fully consumed */
            new_trades.emplace_back(bid_order.instr_symbol,
                                    best_ask_order->second.price,
                                    bid_order.quantity,
                                    best_ask_order->second.order_id,
                                    bid_order.order_id);
            best_ask_order->second.quantity -= bid_order.quantity;
            return;
        } else {
            /* the ask order is fully consumed, find next ask order */
            new_trades.emplace_back(bid_order.instr_symbol,
                                    best_ask_order->second.price,
                                    best_ask_order->second.quantity,
                                    best_ask_order->second.order_id,
                                    bid_order.order_id);
            bid_order.quantity -= best_ask_order->second.quantity;
            eraseBook(best_ask_order->second.order_id);
        }
    }
    if (bid_order.quantity > 0) {
        insertBook(bid_order);
    }
}

void Market::handleAskOrder(InsertOrder &ask_order,
                            std::vector <Trade> &new_trades) {
    if (bid_book.size() == 0) {
        /* there is no bid order */
        insertBook(ask_order);
        return;
    }
    /* find the highest bid order that is bigger than ask price to sell */
    while (ask_order.quantity > 0) {
        auto best_bid_order = bid_book.rbegin();
        if (best_bid_order == bid_book.rend() || best_bid_order->first < ask_order.price) {
            /* there is no bid order meet the requirement */
            break;
        }
        if (best_bid_order->second.quantity > ask_order.quantity) {
            /* the ask order is fully consumed */
            new_trades.emplace_back(ask_order.instr_symbol,
                                    best_bid_order->second.price,
                                    ask_order.quantity,
                                    ask_order.order_id,
                                    best_bid_order->second.order_id);
            best_bid_order->second.quantity -= ask_order.quantity;
        } else {
            /* the bid order is fully consumed, find next bid order */
            new_trades.emplace_back(ask_order.instr_symbol,
                                    best_bid_order->second.price,
                                    best_bid_order->second.quantity,
                                    ask_order.order_id,
                                    best_bid_order->second.order_id);
            ask_order.quantity -= best_bid_order->second.quantity;
            eraseBook(best_bid_order->second.order_id);
        }
    }
    if (ask_order.quantity > 0) {
        insertBook(ask_order);
    }
}

void Market::insertBook(InsertOrder &order) {
    Book::const_iterator it;

    switch (order.type) {
        case INSERT_ASK:
            it = ask_book.emplace(order.price, order);
            break;
        case INSERT_BID:
            it = bid_book.emplace(order.price, order);
            break;
        case INSERT_TYPE_MAX:
            std::cout << "invalid insert order:" << order.order_id << std::endl;
            return;
        default:
            break;
    }
    id_book.emplace(order.order_id, order);
}

void Market::eraseBook(int order_id) {
    if (id_book.count(order_id) == 0) {
        return;
    }
    InsertOrder &order = id_book.at(order_id);
    switch (order.type) {
        case INSERT_ASK:
            eraseByOrderId(ask_book, order);
            break;
        case INSERT_BID:
            eraseByOrderId(bid_book, order);
            break;
        case INSERT_TYPE_MAX:
            std::cout << "invalid insert order:" << order_id << std::endl;
            return;
        default:
            break;
    }
    id_book.erase(order_id);
}

void Market::eraseByOrderId(Book &book, InsertOrder &order) {
    auto range = book.equal_range(order.price);
    for (auto i = range.first; i != range.second; ++i) {
        if (i->second.order_id == order.order_id) {
            book.erase(i);
            break;
        }
    }
}

void Market::showStatus(STATUS_LEVEL level) {
    std::cout << "========" << std::endl;
    std::cout << "all orders executed" << std::endl;
    std::cout << "total trades generated: " << trade_num << std::endl;
    std::cout << "remaining ask orders: " << ask_book.size() << std::endl;
    std::cout << "remaining bid orders: " << bid_book.size() << std::endl;
    if (level < STATUS_FULL) {
        return;
    }
    std::cout << "ask orders are:" << std::endl;
    for (auto it: ask_book) {
        it.second.show();
    }
    std::cout << std::endl
              << "bid orders are:" << std::endl;
    for (auto it: bid_book) {
        it.second.show();
    }
    std::cout << "========" << std::endl;
}

void Market::notifyClient(Client &client, std::vector <Trade> &new_trades) {
    client.receiveTrade(new_trades);
}
