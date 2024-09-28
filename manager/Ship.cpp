#include "Ship.h"

std::string GenName() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::string chars = "qwertyuiopasdfghjklzxcvbnm1234567890";
    int sz = static_cast<int>(chars.size());
    std::string res;
    for (int i = 0; i < 6; ++i) res += chars[rng() % sz];
    return res;
}