cmake_minimum_required(VERSION 3.10)
project(StockPortfolioTracker)

set(CMAKE_CXX_STANDARD 17)

# Include json.hpp (this assumes json.hpp is in nlohmann/ under project root)
include_directories(${CMAKE_SOURCE_DIR})

# Let vcpkg handle CPR
find_package(cpr REQUIRED)

add_executable(tracker src/main.cpp)

# Link CPR
target_link_libraries(tracker PRIVATE cpr::cpr)
