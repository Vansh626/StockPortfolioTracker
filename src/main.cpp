// stock_portfolio_tracker.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <sstream>
#include <cpr/cpr.h> // Added for HTTP requests
#include <nlohmann/json.hpp> // Requires the JSON library: https://github.com/nlohmann/json

using json = nlohmann::json;

struct Stock {
    std::string ticker;
    int quantity;
    double buy_price;
    std::string buy_date;
};

std::map<std::string, Stock> portfolio;

void loadPortfolio(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) return;

    json j;
    file >> j;
    for (const auto &[key, val] : j.items()) {
        portfolio[key] = Stock{
            key,
            val["quantity"],
            val["buy_price"],
            val["buy_date"]
        };
    }
}


void savePortfolio(const std::string &filename) {
    json j;
    for (const auto &[ticker, stock] : portfolio) {
        j[ticker] = {
            {"quantity", stock.quantity},
            {"buy_price", stock.buy_price},
            {"buy_date", stock.buy_date}
        };
    }
    std::ofstream file(filename);
    file << std::setw(4) << j << std::endl;
}

void addStock() {
    Stock s;
    std::cout << "Enter Ticker Symbol: "; std::cin >> s.ticker;
    std::cout << "Enter Quantity: "; std::cin >> s.quantity;
    std::cout << "Enter Buy Price: "; std::cin >> s.buy_price;
    std::cout << "Enter Buy Date (YYYY-MM-DD): "; std::cin >> s.buy_date;

    portfolio[s.ticker] = s;
    std::cout << "Added " << s.ticker << " to portfolio.\n";
}

void removeStock() {
    std::string ticker;
    std::cout << "Enter Ticker Symbol to remove: "; std::cin >> ticker;
    if (portfolio.erase(ticker)) {
        std::cout << "Removed " << ticker << " from portfolio.\n";
    } else {
        std::cout << "Ticker not found.\n";
    }
}

void viewPortfolio() {
    std::cout << "\nCurrent Portfolio:\n";
    std::cout << std::left << std::setw(10) << "Ticker" << std::setw(10) << "Qty" << std::setw(15) << "Buy Price" << std::setw(15) << "Buy Date" << "\n";
    for (const auto &[ticker, stock] : portfolio) {
        std::cout << std::setw(10) << stock.ticker << std::setw(10) << stock.quantity << std::setw(15) << stock.buy_price << std::setw(15) << stock.buy_date << "\n";
    }
    std::cout << std::endl;
}

double calculateYearsSince(const std::string& buy_date) {
    std::tm tm = {};
    std::istringstream ss(buy_date);
    ss >> std::get_time(&tm, "%Y-%m-%d");

    std::time_t buy_time = std::mktime(&tm);
    std::time_t now = std::time(nullptr);

    double seconds_in_year = 60.0 * 60 * 24 * 365;
    return difftime(now, buy_time) / seconds_in_year;
}

const std::string TWELVE_DATA_API_KEY = "8a72e54082b746adb9b381948aaf8f2b";  

double fetchLivePrice(const std::string& ticker) {
    // 1. Try Yahoo Finance
    std::string yahoo_url = "https://query1.finance.yahoo.com/v7/finance/quote?symbols=" + ticker;
    cpr::Response r = cpr::Get(cpr::Url{yahoo_url});

    if (r.status_code == 200) {
        try {
            auto json_response = json::parse(r.text);
            if (!json_response["quoteResponse"]["result"].empty()) {
                double price = json_response["quoteResponse"]["result"][0]["regularMarketPrice"];
                std::cout << "Yahoo Price of " << ticker << ": " << price << "\n";
                return price;
            }
        } catch (...) {
            std::cerr << "Error parsing Yahoo JSON.\n";
        }
    } else {
        std::cerr << "Yahoo API failed with HTTP code: " << r.status_code << "\n";
    }

    // 2. Fallback: Try Twelve Data
    std::string twelve_url = "https://api.twelvedata.com/price?symbol=" + ticker + "&apikey=" + TWELVE_DATA_API_KEY;
    cpr::Response r2 = cpr::Get(cpr::Url{twelve_url});

    try {
        auto json_response = json::parse(r2.text);
        if (json_response.contains("price")) {
            double price = std::stod(json_response["price"].get<std::string>());
            std::cout << "Twelve Data Price of " << ticker << ": " << price << "\n";
            return price;
        } else {
            std::cerr << "Twelve Data response: " << r2.text << "\n";
        }
    } catch (...) {
        std::cerr << "Error parsing Twelve Data JSON.\n";
    }

    std::cerr << "Unable to fetch price for " << ticker << ". Try again later.\n";
    return -1;
}


void calculateCAGR() {
    std::string ticker;
    std::cout << "Enter Ticker Symbol: ";
    std::cin >> ticker;

    if (portfolio.find(ticker) == portfolio.end()) {
        std::cout << "Ticker not found.\n";
        return;
    }

    const Stock& stock = portfolio[ticker];
    double current_price = fetchLivePrice(ticker);
    if (current_price < 0) {
        std::cout << "Unable to fetch price. Try again later.\n";
        return;
    }

    std::cout << "Live price of " << ticker << ": " << current_price << "\n";

    double years = calculateYearsSince(stock.buy_date);
    if (years <= 0) {
        std::cout << "Invalid buy date or date too recent.\n";
        return;
    }

    double cagr = std::pow(current_price / stock.buy_price, 1.0 / years) - 1;
    std::cout << "CAGR for " << ticker << ": " << std::fixed << std::setprecision(2) << cagr * 100 << "%\n";
}

int main() {
    const std::string filename = "portfolio.json";
    loadPortfolio(filename);

    while (true) {
        std::cout << "\n1. Add Stock\n2. Remove Stock\n3. View Portfolio\n4. Calculate CAGR\n5. Save and Exit\nChoose an option: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: addStock(); break;
            case 2: removeStock(); break;
            case 3: viewPortfolio(); break;
            case 4: calculateCAGR(); break;
            case 5: savePortfolio(filename); return 0;
            default: std::cout << "Invalid option.\n";
        }
    }

    return 0;
}