cmake_minimum_required(VERSION 3.25)
project(OrderBookEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(engine)
add_subdirectory(tests)

add_executable(orderbook_app main.cpp)
target_link_libraries(orderbook_app PRIVATE engine)