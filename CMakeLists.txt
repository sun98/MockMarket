cmake_minimum_required(VERSION 3.22)
project(MockMarket)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

set(SOURCES
        src/data_feed_file.cpp
        src/order.cpp
        src/market.cpp
        src/trade.cpp
        src/client.cpp
        src/util/Time.cpp
        src/main.cpp
        )

add_executable(MockMarket ${SOURCES})

target_link_libraries(MockMarket)