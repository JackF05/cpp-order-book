add_executable(engine_tests
    test_order.cpp
    test_engine_order.cpp
)

add_executable(io_tests
    test_order_parser.cpp
)

target_link_libraries(engine_tests PRIVATE
    engine
    GTest::gtest_main
)

target_link_libraries(io_tests PRIVATE
    io
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(engine_tests)
gtest_discover_tests(io_tests)