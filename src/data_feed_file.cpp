#include <iostream>
#include <utility>

#include "util/Time.h"

#include "data_feed_file.h"

DataFeedFile::DataFeedFile() : insert_index(0), cancel_index(0) {
}

bool DataFeedFile::initialize(const std::string &insert_f, const std::string &cancel_f) {
    std::cout << "file data feed initialized" << std::endl;
    std::cout << "insert file=" << insert_f << std::endl;
    std::cout << "cancel file=" << cancel_f << std::endl;
    /* assume the both files exist */
    insert_fs.open(insert_f);
    cancel_fs.open(cancel_f);
    if (!insert_fs.is_open() || !cancel_fs.is_open()) {
        return false;
    }
    initData();
    std::cout << "total insert orders: " << insert_data.size() << std::endl;
    std::cout << "total cancel orders: " << cancel_data.size() << std::endl;
    disconnect();

    return true;
}

bool DataFeedFile::disconnect() {
    if (insert_fs.is_open()) {
        insert_fs.close();
    }
    if (cancel_fs.is_open()) {
        cancel_fs.close();
    }
    return true;
}

bool DataFeedFile::hasNextOrder() const {
    return insert_index < insert_data.size() || cancel_index < cancel_data.size();
}

Order &DataFeedFile::getNextOrder() {
    if (hasNextOrder()) {
        if (cancel_index == cancel_data.size() ||
            (insert_index < insert_data.size() &&
             insert_data[insert_index].time < cancel_data[cancel_index].time)) {
            return insert_data[insert_index++];
        } else {
            return cancel_data[cancel_index++];
        }
    }
    return insert_data.back();
}

void DataFeedFile::initData() {
    std::string line;
    /* 1. handle insert file */
    /* 1.1 skip csv header */
    std::getline(insert_fs, line);
    /* 1.2 iterate csv file by rows */
    while (std::getline(insert_fs, line)) {
        std::stringstream ss(line);
        std::string field;
        InsertOrder order;
        std::getline(ss, field, ',');
        order.instr_symbol = field;
        std::getline(ss, field, ',');
        order.time = Time::convertTimeStringToUInt(field);
        std::getline(ss, field, ',');
        order.order_id = std::stoi(field);
        std::getline(ss, field, ',');
        order.price = static_cast<unsigned int>(std::stod(field) * 100);
        std::getline(ss, field, ',');
        order.quantity = std::stoi(field);
        std::getline(ss, field, ',');
        switch (field[0]) {
            case 'A':
                order.type = INSERT_ASK;
                break;
            case 'B':
                order.type = INSERT_BID;
                break;
            default:
                throw std::runtime_error("Invalid insert order type");
        }
        insert_data.emplace_back(order);
    }

    /* 2. handle cancel file */
    /* 2.1 skip csv header */
    std::getline(cancel_fs, line);
    /* 2.2 iterate csv file by rows */
    while (std::getline(cancel_fs, line)) {
        std::stringstream ss(line);
        std::string field;
        CancelOrder order;
        std::getline(ss, field, ',');
        order.instr_symbol = field;
        std::getline(ss, field, ',');
        order.order_id = std::stoi(field);
        std::getline(ss, field, ',');
        order.quantity = std::stoi(field);
        std::getline(ss, field, ',');
        order.time = Time::convertTimeStringToUInt(field);
        std::getline(ss, field, ',');
        order.org_req_id = static_cast<unsigned int>(std::stod(field) * 100);
        cancel_data.emplace_back(order);
    }
}