#ifndef _DATA_FEED_FILE_H
#define _DATA_FEED_FILE_H

#include <fstream>
#include <sstream>
#include <vector>

#include "data_feed.h"

class DataFeedFile : public DataFeed {
private:
    std::ifstream insert_fs, cancel_fs;
    /* to simplify the data reading, use vector to store them */
    std::vector <InsertOrder> insert_data;
    std::vector <CancelOrder> cancel_data;
    size_t insert_index, cancel_index;

public:
    DataFeedFile();

    bool initialize(const std::string &, const std::string &) override;

    bool hasNextOrder() const override;

    Order &getNextOrder() override;

private:
    bool disconnect();

    void initData();
};

#endif