add_library(engine
    src/order.cpp
    src/order_book.cpp
)

target_include_directories(engine PUBLIC include)