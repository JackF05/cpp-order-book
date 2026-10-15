add_library(io
    src/order_parser.cpp
)

target_include_directories(io PUBLIC include)

target_link_libraries(io PUBLIC engine)