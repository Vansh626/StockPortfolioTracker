# StockPortfolioTracker

A C++ command-line stock portfolio tracker with live price fetching and CAGR (Compound Annual Growth Rate) calculator.

---

## Features

- Add and remove stocks
- View your portfolio
- Calculate CAGR for any stock
- Save and load from JSON
- Fetch real-time stock prices using Yahoo Finance (fallback to Twelve Data API)

---

## Technologies Used

- **C++17**
- **[nlohmann/json](https://github.com/nlohmann/json)** for JSON parsing
- **[CPR](https://github.com/libcpr/cpr)** for HTTP requests
- **CMake** for build configuration

---

## Getting Started

### Prerequisites

- C++ compiler (MSVC / MinGW / GCC)
- [CMake](https://cmake.org/)
- Git
- vcpkg (for dependency management)

### Setup Instructions

```bash
# Clone the repository
git clone https://github.com/Vansh626/StockPortfolioTracker.git
cd StockPortfolioTracker

# Install CPR using vcpkg (if not already installed)
./vcpkg install cpr

# Create build directory and compile
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
