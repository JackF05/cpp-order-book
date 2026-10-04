cmake_minimum_required(VERSION 3.25)
project(OrderBookEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(engine)

add_executable(orderbook_app main.cpp)
target_link_libraries(orderbook_app PRIVATE engine)