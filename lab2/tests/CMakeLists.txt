add_executable(transport_tests
    CoreTest.cpp
    StaffTest.cpp
    FleetTest.cpp
    CargoWarehouseTest.cpp
    OrdersTest.cpp
    FinanceTest.cpp
    AnalyticsTest.cpp
    ServiceTest.cpp
)

target_link_libraries(transport_tests
    PRIVATE
        transport_domain
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(transport_tests)
