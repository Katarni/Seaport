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
