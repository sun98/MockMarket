#ifndef _ORDER_H
#define _ORDER_H

#include <string>

enum ORDER_TYPE {
    ORDER_INSERT,
    ORDER_CANCEL,
    ORDER_TYPE_MAX,
};

enum INSERT_ORDER_TYPE {
    INSERT_BID,
    INSERT_ASK,
    INSERT_TYPE_MAX,
};

class Order {
public:
    std::string instr_symbol;
    unsigned int time;
    unsigned int order_id;
    unsigned int quantity;

    [[nodiscard]] virtual ORDER_TYPE getType() const = 0;

    virtual void show() const = 0;
};

class InsertOrder : public Order {
public:
    unsigned int price{};
    INSERT_ORDER_TYPE type{INSERT_TYPE_MAX};

    [[nodiscard]] ORDER_TYPE getType() const override {
        return ORDER_INSERT;
    };

    void show() const override;
};

class CancelOrder : public Order {
public:
    unsigned int org_req_id{};

    [[nodiscard]] ORDER_TYPE getType() const override {
        return ORDER_CANCEL;
    };

    void show() const override;
};

#endif // _ORDER_H
