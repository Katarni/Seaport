//
// Created by Тимур Ахметзянов on 29.09.2024.
//

#pragma once

#include "header.h"
#include "input-data/GetDataWin.h"
#include "manager/Manager.h"


class PortManagerWin {
 public:
    explicit PortManagerWin(GetDataWin *data_win) {
        width_ = 900, height_ = 700;
        window_ = new sf::RenderWindow(sf::VideoMode(width_, height_), "Modeling");

        max_crane_x_.resize(3);

        manager_ = new Manager;
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
        for (auto data: ships_data) {
            ships_.emplace_back(TypeOfCargo(get<1>(data)), get<0>(data), get<2>(data), get<3>(data));
        }

        cranes_scroll_ = new kat::HorScrollArea(0, 0,
                                                static_cast<float>(width_),
                                                static_cast<float>(height_), window_);
        cranes_scroll_->setBackgroundColor(kLightBlue);
        float last_crane_x = 200;

        auto buoy = new kat::Image("../sprites/buoy.png", window_);
        cranes_scroll_->addElm(buoy);

        buoy = new kat::Image("../sprites/buoy.png", window_);
        buoy->setY(static_cast<float>(height_ - 10));
        cranes_scroll_->addElm(buoy);

        auto border = new kat::Image("../sprites/borders/top-start.png", window_);
        border->setX(last_crane_x);
        last_crane_x += border->getWidth();
        cranes_scroll_->addElm(border);

        for (int i = 0; i < cranes_counters[0]; ++i) {
            auto temp_crane = new kat::Image("../sprites/cranes/top.png", window_);
            temp_crane->setX(last_crane_x);
            last_crane_x += temp_crane->getWidth();
            cranes_scroll_->addElm(temp_crane);
        }

        max_crane_x_[0] = {200 + border->getWidth(), last_crane_x};

        border = new kat::Image("../sprites/borders/top-end.png", window_);
        border->setX(last_crane_x);
        cranes_scroll_->addElm(border);

        last_crane_x = 200;
        border = new kat::Image("../sprites/borders/central-start.png", window_);
        border->setX(last_crane_x);
        border->setY(256);
        last_crane_x += border->getWidth();
        cranes_scroll_->addElm(border);

        for (int i = 0; i < cranes_counters[1]; ++i) {
            auto temp_crane = new kat::Image("../sprites/cranes/central.png", window_);
            temp_crane->setX(last_crane_x);
            temp_crane->setY(250);
            last_crane_x += temp_crane->getWidth();
            cranes_scroll_->addElm(temp_crane);
        }
        max_crane_x_[1] = {200 + border->getWidth(), last_crane_x};

        border = new kat::Image("../sprites/borders/central-end.png", window_);
        border->setX(last_crane_x);
        border->setY(256);
        cranes_scroll_->addElm(border);

        last_crane_x = 200;
        border = new kat::Image("../sprites/borders/bottom-start.png", window_);
        border->setX(last_crane_x);
        border->setY(572);
        last_crane_x += border->getWidth();
        cranes_scroll_->addElm(border);
        for (int i = 0; i < cranes_counters[2]; ++i) {
            auto temp_crane = new kat::Image("../sprites/cranes/bottom.png", window_);
            temp_crane->setX(last_crane_x);
            temp_crane->setY(556);
            last_crane_x += temp_crane->getWidth();
            cranes_scroll_->addElm(temp_crane);
        }

        max_crane_x_[2] = {200 + border->getWidth(), last_crane_x};

        border = new kat::Image("../sprites/borders/bottom-end.png", window_);
        border->setX(last_crane_x);
        border->setY(572);
        cranes_scroll_->addElm(border);

        buoy = new kat::Image("../sprites/buoy.png", window_);
        buoy->setX(std::max(std::max({max_crane_x_[0].second,
                             max_crane_x_[1].second,
                             max_crane_x_[2].second}) + border->getWidth() + 200,
                            static_cast<float>(width_ - 10)));
        cranes_scroll_->addElm(buoy);

        buoy = new kat::Image("../sprites/buoy.png", window_);
        buoy->setX(std::max(std::max({max_crane_x_[0].second,
                                      max_crane_x_[1].second,
                                      max_crane_x_[2].second}) + border->getWidth() + 200,
                            static_cast<float>(width_ - 10)));
        buoy->setY(static_cast<float>(height_ - 10));
        cranes_scroll_->addElm(buoy);

        buoy = new kat::Image("../sprites/buoy.png", window_);
        buoy->setY(static_cast<float>(height_ - 10));
        cranes_scroll_->addElm(buoy);
    }

    void modeling() {
        while (window_->isOpen()) {
            sf::Event event{};
            while (window_->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window_->close();
                }

                if (event.type == sf::Event::MouseWheelScrolled) {
                    if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
                        cranes_scroll_->moveAllX(event.mouseWheelScroll.delta);
                    }
                }
            }

            window_->clear(kLightBlue);

            cranes_scroll_->render();

            window_->display();
        }
    }

 private:
    int width_, height_;
    sf::RenderWindow *window_;
    Manager *manager_;

    kat::HorScrollArea *cranes_scroll_;

    std::vector<std::pair<float, float>> max_crane_x_;
};
