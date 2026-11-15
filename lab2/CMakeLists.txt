cmake_minimum_required(VERSION 3.16)
project(lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(transport_domain STATIC
    src/Enums.cpp
    src/core/Address.cpp
    src/core/GPSPosition.cpp
    src/core/Money.cpp
    src/core/TransportDate.cpp
    src/staff/Salary.cpp
    src/staff/DrivingLicense.cpp
    src/staff/Employee.cpp
    src/staff/Driver.cpp
    src/staff/Dispatcher.cpp
    src/staff/Manager.cpp
    src/staff/HRManager.cpp
    src/staff/WarehouseManager.cpp
    src/staff/Accountant.cpp
    src/staff/Mechanic.cpp
    src/fleet/Machine.cpp
    src/fleet/Vehicle.cpp
    src/fleet/Trailer.cpp
    src/fleet/Truck.cpp
    src/fleet/Van.cpp
    src/fleet/MaintenanceRecord.cpp
    src/fleet/Insurance.cpp
    src/cargo/Cargo.cpp
    src/cargo/FragileCargo.cpp
    src/cargo/PerishableCargo.cpp
    src/warehouse/Rack.cpp
    src/warehouse/StorageUnit.cpp
    src/warehouse/StorageZone.cpp
    src/warehouse/Warehouse.cpp
    src/orders/Client.cpp
    src/orders/RoutePoint.cpp
    src/orders/Route.cpp
    src/orders/Order.cpp
    src/orders/ExpressDelivery.cpp
    src/orders/Delivery.cpp
    src/orders/TrackingSystem.cpp
    src/orders/LogisticsManager.cpp
    src/finance/BankAccount.cpp
    src/finance/Invoice.cpp
    src/finance/Payment.cpp
    src/finance/ExpenseTracker.cpp
    src/finance/ProfitAnalyzer.cpp
    src/finance/TaxCalculator.cpp
    src/finance/CompanyFinance.cpp
    src/analytics/PerformanceMetric.cpp
    src/analytics/FuelConsumption.cpp
    src/analytics/TrafficInfo.cpp
    src/analytics/Metrics.cpp
    src/analytics/CustomerFeedback.cpp
    src/analytics/ServiceQualityMonitor.cpp
    src/service/LoyaltyProgram.cpp
    src/service/CustomerService.cpp
    src/service/ServiceManagement.cpp
    src/service/TransportCompany.cpp
)

target_include_directories(transport_domain
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(transport_domain PRIVATE -Wall -Wextra -Wpedantic)

add_executable(transport_cli
    cli/main.cpp
    cli/CliApplication.cpp
)

target_link_libraries(transport_cli PRIVATE transport_domain)

add_subdirectory(tests)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(transport_domain PRIVATE --coverage -O0 -g)
    target_link_options(transport_domain PUBLIC --coverage)
endif()
