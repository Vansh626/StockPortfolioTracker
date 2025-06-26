# Stock Portfolio Tracker (C++)

A command-line tool to manage your stock investments. Features:
- Add, remove, and view stocks
- Calculate CAGR using live market prices
- Fetch prices using Yahoo Finance or fallback to Twelve Data

## Tech Used
- C++
- CPR (C++ Requests)
- nlohmann/json
- vcpkg + CMake

## Build Instructions

```bash
git clone https://github.com/YOUR_USERNAME/StockPortfolioTracker.git
cd StockPortfolioTracker

# Setup vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg install cpr nlohmann-json
cd ..

# Build
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
