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

        std::vector<ScheduleItem> ships;
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

        clock_truck_ = new kat::Image("../sprites/trucks/normal.png", window_);
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

        btns_truck_ = new kat::Image("../sprites/trucks/reversed.png", window_);
        btns_truck_->setX(280);
        btns_truck_->setY(331);
        btns_truck_->scale(2.5);

        ships_at_queue_counters_.resize(3);
        ships_at_queue_lbls_.resize(3);
        for (int i = 0; i < 3; ++i) {
            ships_at_queue_lbls_[i] = new kat::Label;
            ships_at_queue_lbls_[i]->setParent(window_);
            ships_at_queue_lbls_[i]->setX(77);
            ships_at_queue_lbls_[i]->setWidth(77);
            ships_at_queue_lbls_[i]->setHeight(22);

            if (i == 0) {
                ships_at_queue_lbls_[i]->setY(105);
            } else if (i == 1) {
                ships_at_queue_lbls_[i]->setY(188);
            } else {
                ships_at_queue_lbls_[i]->setY(static_cast<float>(height_) - 196);
            }

            ships_at_queue_lbls_[i]->setFontSize(18);
            ships_at_queue_lbls_[i]->setFont("../fonts/KodeMono.ttf");
            ships_at_queue_lbls_[i]->setBackgroundColor(sf::Color::Transparent);
            ships_at_queue_lbls_[i]->setColor(sf::Color::White);
        }

        control_btns_imgs_.resize(3);
        control_btns_.resize(3);

        control_btns_[0] = new kat::Button;
        control_btns_imgs_[0] = new kat::Image;
        control_btns_[1] = new kat::Button;
        control_btns_imgs_[1] = new kat::Image;
        control_btns_[2] = new kat::Button;
        control_btns_imgs_[2] = new kat::Image;

        control_btns_[0]->setX(btns_truck_->getX() + 25);
        control_btns_[1]->setX(btns_truck_->getX() + 51);
        control_btns_[2]->setX(btns_truck_->getX() + 76);
        control_btns_imgs_[0]->setX(btns_truck_->getX() + 25);
        control_btns_imgs_[1]->setX(btns_truck_->getX() + 51);
        control_btns_imgs_[2]->setX(btns_truck_->getX() + 76);

        control_btns_imgs_[0]->loadFromFile("../sprites/btns/left-arrow.png");
        control_btns_imgs_[1]->loadFromFile("../sprites/btns/pause.png");
        control_btns_imgs_[2]->loadFromFile("../sprites/btns/right-arrow.png");

        for (int i = 0; i < 3; ++i) {
            control_btns_imgs_[i]->setParent(window_);
            control_btns_imgs_[i]->scale(2);
            control_btns_imgs_[i]->setY(btns_truck_->getY() +
                                        (btns_truck_->getScaledHeight()
                                         - control_btns_imgs_[i]->getScaledHeight()) / 2);

            control_btns_[i]->setParent(window_);
            control_btns_[i]->setY(btns_truck_->getY() +
                                    (btns_truck_->getScaledHeight()
                                        - control_btns_imgs_[i]->getScaledHeight()) / 2);
            control_btns_[i]->setHeight(control_btns_imgs_[i]->getScaledHeight());
            control_btns_[i]->setWidth(control_btns_imgs_[i]->getScaledWidth());
        }
    }

    void modeling() {
        manager_->modeling();

        int64_t last_time = getCurrTime();

        forwardEvents();

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
                        if (pause_) {
                            control_btns_imgs_[1]->loadFromFile("../sprites/btns/pause.png");
                        } else {
                            control_btns_imgs_[1]->loadFromFile("../sprites/btns/play.png");
                        }
                        pause_ = !pause_;
                    }
                }

                if (event.type == sf::Event::MouseButtonReleased) {
                    if (pause_) {
                        for (auto & [key, val] : ships_) {
                            val->isSelected(static_cast<float>(event.mouseButton.x),
                                            static_cast<float>(event.mouseButton.y));
                        }
                    }

                    if (control_btns_[0]->isPressed(static_cast<float>(event.mouseButton.x),
                                                    static_cast<float>(event.mouseButton.y))) {
                        if (time_ % (24 * 60) == 0) {
                            time_ -= 24*60;
                        } else {
                            time_ -= time_ % (24 * 60);
                        }
                        backwardEvent();
                    }

                    if (control_btns_[1]->isPressed(static_cast<float>(event.mouseButton.x),
                                                    static_cast<float>(event.mouseButton.y))) {
                        if (pause_) {
                            control_btns_imgs_[1]->loadFromFile("../sprites/btns/pause.png");
                        } else {
                            control_btns_imgs_[1]->loadFromFile("../sprites/btns/play.png");
                        }
                        pause_ = !pause_;
                    }

                    if (control_btns_[2]->isPressed(static_cast<float>(event.mouseButton.x),
                                                    static_cast<float>(event.mouseButton.y))) {

                        time_ += 24 * 60 - time_ % (24 * 60);
                        forwardEvents();
                    }
                }
            }

            if (last_time + 1000 / 24 < getCurrTime()) {
                clock_lbl_->setData(intToTime(time_));

                for (int i = 0; i < 3; ++i) {
                    if (ships_at_queue_counters_[i] == 0) {
                        ships_at_queue_lbls_[i]->setData("");
                    } else {
                        if (ships_at_queue_counters_[i] < 0) {
                            std::cout << 0;
                        }
                        ships_at_queue_lbls_[i]->setData(std::to_string(ships_at_queue_counters_[i]));
                    }
                }
            }

            if (last_time + 1000 / 24 < getCurrTime() && !pause_) {
                last_time = getCurrTime();
                ++time_;
                forwardEvents();
            }

            window_->clear(kLightBlue);

            for (auto & [key, val] : ships_) {
                if (val->updCoorInTime(time_)) continue;
                val->render();
            }

            cranes_scroll_->render();

            clock_truck_->render();
            clock_lbl_->render();

            btns_truck_->render();

            for (int i = 0; i < 3; ++i) {
                ships_at_queue_lbls_[i]->render();
                control_btns_imgs_[i]->render();
            }

            window_->display();
        }
    }

 private:
    int64_t time_;
    int width_, height_;
    sf::RenderWindow *window_;
    Manager *manager_;

    std::vector<int> ships_at_queue_counters_;
    std::vector<kat::Label*> ships_at_queue_lbls_;

    std::map<Ship*, DrawableShip*> ships_;

    kat::Label *clock_lbl_;
    kat::Image *clock_truck_, *btns_truck_;

    std::vector<kat::Image*> control_btns_imgs_;
    std::vector<kat::Button*> control_btns_;

    kat::HorScrollArea *cranes_scroll_;

    std::vector<std::pair<float, float>> max_crane_x_;

    void forwardEvents() {
        if (time_ == static_cast<int64_t>(-1e18)) {
            time_ = manager_->getCur().getTime();

            convertEventToShip(manager_->getCur());
            manager_->goNext();
        }

        while (time_ >= manager_->getCur().getTime()) {
            convertEventToShip(manager_->getCur());
            if (manager_->goNext()) break;
        }
    }

    void backwardEvent() {
        if (time_ < manager_->getCur().getTime() && manager_->goPrev()) return;

        while (time_ < manager_->getCur().getTime()) {
            rollbackEvent(manager_->getCur());
            if (manager_->goPrev()) break;
        }
    }

    void convertEventToShip(const Event& event) {
        if (ships_[event.getShip()] == nullptr) {
            ships_[event.getShip()] = new DrawableShip(static_cast<int>(event.getShip()->getType()),
                                                       window_, event.getShip());
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
            ships_at_queue_counters_[static_cast<int64_t>(event.getShip()->getType())] -= 1;

            is_y_first = false;
            from_x = 77;
            dis_x = static_cast<float>(56 + 128 * (event.getIdCrane() + 1));
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
                from_y = static_cast<float>(height_) - 140;
            } else {
                from_y = 244;
            }

            dis_x = std::max(std::max({max_crane_x_[0].second,
                                       max_crane_x_[1].second,
                                       max_crane_x_[2].second}) + 280,
                             static_cast<float>(width_));

            ships_[event.getShip()]->addEvent({from_x, from_y, dis_x, dis_y, is_y_first, event.getTime()});
        } else if (event.getTypeOfEvent() == TypeOfEvent::ArrivalAtPort) {
            ships_at_queue_counters_[static_cast<int64_t>(event.getShip()->getType())] += 1;
        }
    }

    void rollbackEvent(const Event& event) {
        if (event.getTypeOfEvent() == TypeOfEvent::ArrivalAtPort) {
            ships_at_queue_counters_[static_cast<int64_t>(event.getShip()->getType())] -= 1;
        } else if (event.getTypeOfEvent() == TypeOfEvent::FinishOfUnloading ||
                event.getTypeOfEvent() == TypeOfEvent::ArrivalOnScreen) {
            ships_[event.getShip()]->popBack();
        } else if (event.getTypeOfEvent() == TypeOfEvent::StartMovingToCrane) {
            ships_[event.getShip()]->popBack();
            ships_at_queue_counters_[static_cast<int64_t>(event.getShip()->getType())] += 1;
        }
    }
};
