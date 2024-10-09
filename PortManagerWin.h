//
// Created by Тимур Ахметзянов on 29.09.2024.
//

#pragma once

#include "header.h"
#include "input-data/GetDataWin.h"
#include "ships/DrawableShip.h"


class PortManagerWin {
 public:
    explicit PortManagerWin(GetDataWin *data_win) {
        width_ = 900, height_ = 700;
        window_ = new sf::RenderWindow(sf::VideoMode(width_, height_), "Modeling");
        time_ = -1e18;

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

        std::vector<Ship> ships;
        auto ships_data = data_win->getShipsData();
        for (auto data: ships_data) {
            ships.emplace_back(TypeOfCargo(get<1>(data)), get<0>(data), get<2>(data), get<3>(data));
        }
        manager_->setShips(ships);

        cranes_scroll_ = new kat::HorScrollArea(0, 0,
                                                static_cast<float>(width_),
                                                static_cast<float>(height_), window_);
        cranes_scroll_->setBackgroundColor(sf::Color::Transparent);
        float last_crane_x = 176;

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

        max_crane_x_[0] = {176 + border->getWidth(), last_crane_x};

        border = new kat::Image("../sprites/borders/top-end.png", window_);
        border->setX(last_crane_x);
        cranes_scroll_->addElm(border);

        last_crane_x = 176;
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
        max_crane_x_[1] = {176 + border->getWidth(), last_crane_x};

        border = new kat::Image("../sprites/borders/central-end.png", window_);
        border->setX(last_crane_x);
        border->setY(256);
        cranes_scroll_->addElm(border);

        last_crane_x = 176;
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

        max_crane_x_[2] = {176 + border->getWidth(), last_crane_x};

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

        clock_truck_ = new kat::Image("../sprites/clock-truck.png", window_);
        clock_truck_->setX(240);
        clock_truck_->setY(24);
        clock_truck_->scale(2.5);

        clock_lbl_= new kat::Label(249, 29,
                                   clock_truck_->getScaledWidth(),
                                   clock_truck_->getScaledHeight(),
                                   "", "../fonts/KodeMono.ttf", window_);
        clock_lbl_->setBackgroundColor(sf::Color::Transparent);
        clock_lbl_->setColor(sf::Color::White);
        clock_lbl_->setFontSize(14);
    }

    void modeling() {
        manager_->modeling();

        int64_t last_time = getCurrTime();

        pollManagerEvent();

        bool pause_ = false;

        while (window_->isOpen()) {
            sf::Event event{};
            while (window_->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window_->close();
                }

                if (event.type == sf::Event::MouseWheelScrolled) {
                    if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
                        if (cranes_scroll_->moveAllX(event.mouseWheelScroll.delta)) {
                            for (auto & [key, val] : ships_) {
                                val->moveX(event.mouseWheelScroll.delta);
                            }
                        }
                    }
                }

                if (event.type == sf::Event::KeyReleased) {
                    if (event.key.code == sf::Keyboard::Space) {
                        pause_ = !pause_;
                    }
                }
            }

            if (last_time + 1000 / 12 < getCurrTime() && !pause_) {
                last_time = getCurrTime();
                ++time_;
                pollManagerEvent();
                clock_lbl_->setData(intToTime(time_));
            }

            window_->clear(kLightBlue);

            for (auto & [key, val] : ships_) {
                if (val->updCoorInTime(time_)) continue;
                val->render();
            }

            cranes_scroll_->render();

            clock_truck_->render();
            clock_lbl_->render();

            window_->display();
        }
    }

 private:
    int64_t time_;
    int width_, height_;
    sf::RenderWindow *window_;
    Manager *manager_;

    std::map<Ship*, DrawableShip*> ships_;

    kat::Label *clock_lbl_;
    kat::Image *clock_truck_;

    kat::HorScrollArea *cranes_scroll_;

    std::vector<std::pair<float, float>> max_crane_x_;

    void pollManagerEvent() {
        if (time_ == static_cast<int64_t>(-1e18)) {
            time_ = manager_->getCur().getTime();

            convertEventToShip(manager_->getCur());
            manager_->goNext();
        }

        while (time_ == manager_->getCur().getTime()) {
            convertEventToShip(manager_->getCur());
            if (manager_->goNext()) break;
        }
    }

    void convertEventToShip(const Event& event) {
        if (ships_[event.getShip()] == nullptr) {
            ships_[event.getShip()] = new DrawableShip(static_cast<int>(event.getShip()->getType()), window_);
        }

        float from_y, from_x, dis_x, dis_y;
        bool is_y_first;

        if (event.getTypeOfEvent() == TypeOfEvent::ArrivalOnScreen) {
            dis_y = 0;
            dis_x = 144;
            from_x = -77;
            is_y_first = false;

            if (event.getShip()->getType() == TypeOfCargo::Container) {
                from_y = 139;
            } else if (event.getShip()->getType() == TypeOfCargo::Granular) {
                from_y = static_cast<float>(height_) - 162;
            } else {
                from_y = 222;
            }

            ships_[event.getShip()]->addEvent({from_x, from_y, dis_x, dis_y, is_y_first, event.getTime()});
        } else if (event.getTypeOfEvent() == TypeOfEvent::StartMovingToCrane) {
            is_y_first = false;
            from_x = 77;
            dis_x = static_cast<float>(40 + 144 * (event.getIdCrane() + 1));
            dis_y = 22;

            if (event.getShip()->getType() == TypeOfCargo::Container) {
                from_y = 139;
                dis_y *= -1;
            } else if (event.getShip()->getType() == TypeOfCargo::Granular) {
                from_y = static_cast<float>(height_) - 162;
            } else {
                from_y = 222;
            }

            ships_[event.getShip()]->addEvent({from_x, from_y, dis_x, dis_y, is_y_first, event.getTime()});
        } else if (event.getTypeOfEvent() == TypeOfEvent::FinishOfUnloading) {
            from_x = static_cast<float>(127 + 144 * (event.getIdCrane() + 1));
            is_y_first = true;
            dis_y = -22;

            if (event.getShip()->getType() == TypeOfCargo::Container) {
                from_y = 117;
                dis_y *= -1;
            } else if (event.getShip()->getType() == TypeOfCargo::Granular) {
                from_y = 0;
            } else {
                from_y = 244;
            }

            dis_x = std::max(std::max({max_crane_x_[0].second,
                                       max_crane_x_[1].second,
                                       max_crane_x_[2].second}) + 280,
                             static_cast<float>(width_));

            ships_[event.getShip()]->addEvent({from_x, from_y, dis_x, dis_y, is_y_first, event.getTime()});
        }
    }
};
