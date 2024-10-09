//
// Created by Тимур Ахметзянов on 29.09.2024.
//

#pragma once

#include <KatLib.h>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "manager/Manager.h"

const sf::Color kBlue = sf::Color(31, 184, 193), kLightBlue = sf::Color(16, 204, 224);

int64_t getCurrTime() {
    return std::chrono::duration_cast
            <std::chrono::milliseconds>
            (std::chrono::steady_clock::now().time_since_epoch()).count();
}

std::string intToTime(int64_t time) {
    if (time < 0) return "00:00:00";

    std::string res;
    if (std::to_string(time / (24 * 60)).size() == 1) {
        res = "0" + std::to_string(time / (24 * 60));
    } else {
        res = std::to_string(time / (24 * 60));
    }

    res += ":";

    time %= (24 * 60);

    if (std::to_string(time / 60).size() == 1) {
        res += "0" + std::to_string(time / 60);
    } else {
        res += std::to_string(time / 60);
    }

    time %= 60;

    res += ":";

    if (std::to_string(time).size() == 1) {
        res += "0" + std::to_string(time);
    } else {
        res += std::to_string(time);
    }

    return res;
}
