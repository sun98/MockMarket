#ifndef _DATA_FEED_H
#define _DATA_FEED_H

#include "order.h"

class DataFeed {
public:
    virtual bool initialize(const std::string &, const std::string &) = 0;

    virtual bool hasNextOrder() const = 0;

    virtual Order &getNextOrder() = 0;
};

#endif