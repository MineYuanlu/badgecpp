#include "badgecpp/badge.hpp"
#include <chrono>
#include <iostream>
namespace x {
    void xxx();
}
int main() {
    x::xxx();
    badge::Badge badge{
            "label",
            std::nullopt,
            "message",
            std::nullopt,
            badge::FLAT,
            std::nullopt,
            std::nullopt,
            "id",
    };
    auto svg = badge.makeBadge();
    std::cout << svg << std::endl;
    // time check
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1; i++) {
        badge.makeBadge();
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "time used: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microseconds" << std::endl;

    return 0;
}