//
// Created by Тимур Ахметзянов on 29.09.2024.
//

#pragma once

#include "header.h"
#include "input-data/GetDataWin.h"
#include "manager/Manager.h"


class PortManagerWin {
 public:
    PortManagerWin() {
        width_ = 900, height_ = 700;

        window_ = new sf::RenderWindow(sf::VideoMode(width_, height_), "Modeling");

        manager_ = new Manager;
    }

    void loadData(GetDataWin* data_win) {
        manager_->setDelayRange(data_win->getDelayLimits().first,
                                data_win->getDelayLimits().second);
        manager_->setLateArrivalRange(data_win->getDeviationLimits().first,
                                        data_win->getDeviationLimits().second);

        auto cranes_counters = data_win->getCranesCounts();
        manager_->setCountContainerCranes(cranes_counters[0]);
        manager_->setCountLiquidCranes(cranes_counters[1]);
        manager_->setCountGranularCranes(cranes_counters[2]);

        std::vector<Ship> ships_;
        auto ships_data = data_win->getShipsData();
        for (auto data : ships_data) {
            ships_.emplace_back(TypeOfCargo(get<1>(data)), get<0>(data), get<2>(data), -1, get<3>(data));
        }
    }

    void modeling() {
        while (window_->isOpen()) {
            sf::Event event{};
            while (window_->pollEvent(event)) {

            }

            window_->clear(sf::Color::White);

            window_->display();
        }
    }

 private:
    int width_, height_;
    sf::RenderWindow *window_;
    Manager* manager_;
};
