//
// Created by Тимур Ахметзянов on 29.09.2024.
//

#pragma once

#include "input-data/GetDataWin.h"
#include "ships/DrawableShip.h"
#include "schedule/ScheduleScroll.h"
#include "schedule/UnloadedItem.h"


class PortManagerWin {
 public:
    explicit PortManagerWin(GetDataWin *data_win) {
        ended_ = false;
        width_ = 900, height_ = 700;
        window_ = new sf::RenderWindow(sf::VideoMode(width_, height_), "Modeling");
        time_ = -1e18;

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

        std::vector<float> max_crane_x(3);
        max_crane_x[0] = last_crane_x;

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
        max_crane_x[1] = last_crane_x;

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

        max_crane_x[2] = last_crane_x;

        max_crane_x_ = std::max({max_crane_x[0], max_crane_x[1], max_crane_x[2]});

        while (max_crane_x[0] < max_crane_x_) {
            std::random_device rd;
            std::mt19937 rng(rd());
            auto temp_crane = new kat::Image("../sprites/spaces/top/sprite-0"
                    + std::to_string(rng() % 2 + 1) + ".png",
                                             window_);
            temp_crane->setX(max_crane_x[0]);
            max_crane_x[0] += temp_crane->getWidth();
            cranes_scroll_->addElm(temp_crane);
        }

        while (max_crane_x[1] < max_crane_x_) {
            std::random_device rd;
            std::mt19937 rng(rd());
            auto temp_crane = new kat::Image("../sprites/spaces/central/sprite-0" +
                                                std::to_string(rng() % 2 + 1) + ".png",
                                             window_);
            temp_crane->setX(max_crane_x[1]);
            temp_crane->setY(256);
            max_crane_x[1] += temp_crane->getWidth();
            cranes_scroll_->addElm(temp_crane);
        }

        while (max_crane_x[2] < max_crane_x_) {
            std::random_device rd;
            std::mt19937 rng(rd());
            auto temp_crane = new kat::Image("../sprites/spaces/bottom/sprite-0" +
                                                std::to_string(rng() % 2 + 1) + ".png",
                                             window_);
            temp_crane->setX(max_crane_x[2]);
            temp_crane->setY(572);
            max_crane_x[2] += temp_crane->getWidth();
            cranes_scroll_->addElm(temp_crane);
        }

        border = new kat::Image("../sprites/borders/top-end.png", window_);
        border->setX(max_crane_x[0]);
        cranes_scroll_->addElm(border);

        border = new kat::Image("../sprites/borders/central-end.png", window_);
        border->setX(max_crane_x[1]);
        border->setY(256);
        cranes_scroll_->addElm(border);

        border = new kat::Image("../sprites/borders/bottom-end.png", window_);
        border->setX(max_crane_x[2]);
        border->setY(572);
        cranes_scroll_->addElm(border);

        buoy = new kat::Image("../sprites/buoy.png", window_);
        buoy->setX(std::max(max_crane_x_ + border->getWidth() + 200, static_cast<float>(width_ - 10)));
        cranes_scroll_->addElm(buoy);

        buoy = new kat::Image("../sprites/buoy.png", window_);
        buoy->setX(std::max(max_crane_x_ + border->getWidth() + 200,static_cast<float>(width_ - 10)));
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

        unloaded_btn_ = new kat::Button(window_);
        unloaded_btn_->setBackgroundColor(sf::Color(245, 208, 49));
        unloaded_btn_->setData("u");
        unloaded_btn_->setFont("../fonts/KodeMono.ttf");
        unloaded_btn_->setFontSize(16);
        unloaded_btn_->resize(20, 20);
        unloaded_btn_->setColor(sf::Color::White);
        unloaded_btn_->setX(15);
        unloaded_btn_->setY(5);
        unloaded_btn_->setBorderRadius(2);

        schedule_btn_ = new kat::Button(window_);
        schedule_btn_->setBackgroundColor(sf::Color(245, 208, 49));
        schedule_btn_->setFont("../fonts/KodeMono.ttf");
        schedule_btn_->setFontSize(16);
        schedule_btn_->setData("s");
        schedule_btn_->resize(20, 20);
        schedule_btn_->setColor(sf::Color::White);
        schedule_btn_->setX(45);
        schedule_btn_->setY(5);
        schedule_btn_->setBorderRadius(2);

        results_btn_ = new kat::Button(window_);
        results_btn_->setBackgroundColor(sf::Color(245, 208, 49));
        results_btn_->setData("results");
        results_btn_->setFont("../fonts/KodeMono.ttf");
        results_btn_->setFontSize(16);
        results_btn_->resize(80, 20);
        results_btn_->setColor(sf::Color::White);
        results_btn_->setX(75);
        results_btn_->setY(5);
        results_btn_->setBorderRadius(2);

        scroll_areas_title_ = new kat::Button(window_);
        scroll_areas_title_->resize(554, 43);
        scroll_areas_title_->setData("Schedule");
        scroll_areas_title_->setFont("../fonts/KodeMono.ttf");
        scroll_areas_title_->setFontSize(24);
        scroll_areas_title_->setX(346);

        schedule_area_ = new ScheduleScroll(window_);
        schedule_area_->resize(554, static_cast<float>(height_) - scroll_areas_title_->getHeight());
        schedule_area_->setX(346);
        schedule_area_->setY(43);
        schedule_area_->setCropBorders(true);

        unloaded_ships_scroll_ = new kat::VerScrollArea(window_);
        unloaded_ships_scroll_->resize(554, static_cast<float>(height_) - scroll_areas_title_->getHeight());
        unloaded_ships_scroll_->setX(346);
        unloaded_ships_scroll_->setY(43);
        unloaded_ships_scroll_->setCropBorders(true);

        float last_y = 43;
        for (const auto& item : manager_->getSchedule()) {
            schedule_idxs_[item.getName()] = schedule_area_->getElms().size();
            auto elm = new DrawableScheduleItem(item, window_);
            elm->setY(last_y);
            last_y += elm->getHeight() + 15;
            elm->setX(378);
            schedule_area_->addElm(elm);
        }

        last_unloaded_ship_y_ = 43;
    }

    void modeling() {
        manager_->modeling();

        int64_t last_time = getCurrTime();

        forwardEvents();

        bool pause = false, draw_schedule = false, draw_unloaded = false;

        while (window_->isOpen()) {
            sf::Event event{};
            while (window_->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window_->close();
                }

                if (event.type == sf::Event::MouseWheelScrolled) {
                    if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
                        if (!draw_schedule ||
                            !schedule_area_->isHovered(static_cast<float>(event.mouseWheelScroll.x),
                                                     static_cast<float>(event.mouseWheelScroll.y))) {
                            if (cranes_scroll_->moveAllX(event.mouseWheelScroll.delta)) {
                                for (auto & [key, val] : ships_) {
                                    val->moveX(event.mouseWheelScroll.delta);
                                }
                                for (auto& elm : ships_at_queue_lbls_) {
                                    elm->moveX(event.mouseWheelScroll.delta);
                                }
                            }
                        }
                    } else if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                        if (draw_schedule && schedule_area_->isHovered(static_cast<float>(event.mouseWheelScroll.x),
                                                          static_cast<float>(event.mouseWheelScroll.y))) {
                            schedule_area_->moveAllY(static_cast<float>(kat::sign(event.mouseWheelScroll.delta) * 5));
                        }
                    }
                }

                if (event.type == sf::Event::KeyReleased) {
                    if (event.key.code == sf::Keyboard::Space) {
                        if (pause) {
                            control_btns_imgs_[1]->loadFromFile("../sprites/btns/pause.png");
                        } else {
                            control_btns_imgs_[1]->loadFromFile("../sprites/btns/play.png");
                        }
                        pause = !pause;
                    }
                }

                if (event.type == sf::Event::MouseButtonReleased) {
                    if (pause) {
                        for (auto & [key, val] : ships_) {
                            val->isSelected(static_cast<float>(event.mouseButton.x),
                                            static_cast<float>(event.mouseButton.y));
                        }
                    }

                    if (results_btn_->isPressed(static_cast<float>(event.mouseButton.x),
                                                static_cast<float>(event.mouseButton.y))) {
                        window_->close();
                    }

                    if (schedule_btn_->isPressed(static_cast<float>(event.mouseButton.x),
                                                static_cast<float>(event.mouseButton.y))) {
                        draw_schedule = !draw_schedule;
                        draw_unloaded = false;
                        scroll_areas_title_->setData("Schedule");
                    }

                    if (unloaded_btn_->isPressed(static_cast<float>(event.mouseButton.x),
                                                 static_cast<float>(event.mouseButton.y))) {
                        draw_unloaded = !draw_unloaded;
                        draw_schedule = false;
                        scroll_areas_title_->setData("Unloaded");
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
                        if (pause) {
                            control_btns_imgs_[1]->loadFromFile("../sprites/btns/pause.png");
                        } else {
                            control_btns_imgs_[1]->loadFromFile("../sprites/btns/play.png");
                        }
                        pause = !pause;
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

            if (last_time + 1000 / 24 < getCurrTime() && !pause) {
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

            unloaded_btn_->render();
            results_btn_->render();
            schedule_btn_->render();

            if (draw_schedule || draw_unloaded) {
                scroll_areas_title_->render();
            }
            if (draw_schedule) {
                schedule_area_->render();
            }
            if (draw_unloaded) {
                unloaded_ships_scroll_->render();
            }

            window_->display();
        }
    }

    [[nodiscard]]
    Manager *getManager() const {
        return manager_;
    }

 private:
    int64_t time_;
    bool ended_;
    int width_, height_;
    float last_unloaded_ship_y_;
    sf::RenderWindow *window_;
    Manager *manager_;

    ScheduleScroll *schedule_area_;
    kat::HorScrollArea *cranes_scroll_;
    kat::VerScrollArea *unloaded_ships_scroll_;

    std::vector<int> ships_at_queue_counters_;
    std::vector<kat::Label*> ships_at_queue_lbls_;

    std::map<Ship*, DrawableShip*> ships_;
    std::map<std::string, size_t> schedule_idxs_;

    kat::Label *clock_lbl_;
    kat::Image *clock_truck_, *btns_truck_;

    std::vector<kat::Image*> control_btns_imgs_;
    std::vector<kat::Button*> control_btns_;

    kat::Button *results_btn_, *schedule_btn_, *scroll_areas_title_, *unloaded_btn_;

    float max_crane_x_;

    void forwardEvents() {
        if (time_ == static_cast<int64_t>(-1e18)) {
            time_ = manager_->getCur().getTime();

            convertEventToShip(manager_->getCur());
            manager_->goNext();
        }

        while (!ended_ && time_ >= manager_->getCur().getTime()) {
            convertEventToShip(manager_->getCur());
            ended_ = manager_->goNext();
        }
    }

    void backwardEvent() {
        while (time_ < manager_->getCur().getTime()) {
            rollbackEvent(manager_->getCur());
            ended_ = false;
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

                ships_[event.getShip()]->addEvent({ from_x, from_y, dis_x, dis_y, is_y_first, event.getTime() });
        } else if (event.getTypeOfEvent() == TypeOfEvent::FinishOfUnloading) {
            schedule_area_->changeUnload(schedule_idxs_[event.getShip()->getName()]);

            from_x = static_cast<float>(143 + 128 * (event.getIdCrane() + 1));
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

            auto unloaded_item = new UnloadedItem(event.getShip(), window_);
            unloaded_item->setY(last_unloaded_ship_y_);
            unloaded_item->setX(378);
            last_unloaded_ship_y_ += unloaded_item->getHeight() + 15;
            unloaded_ships_scroll_->addElm(unloaded_item);

            dis_x = std::max(max_crane_x_ + 280, static_cast<float>(width_));

            ships_[event.getShip()]->addEvent({from_x, from_y, dis_x, dis_y, is_y_first, event.getTime()});
        }
        else if (event.getTypeOfEvent() == TypeOfEvent::ArrivalAtPort) {
            ships_at_queue_counters_[static_cast<int64_t>(event.getShip()->getType())] += 1;
        }
    }

    void rollbackEvent(const Event& event) {
        if (event.getTypeOfEvent() == TypeOfEvent::ArrivalAtPort) {
            ships_at_queue_counters_[static_cast<int64_t>(event.getShip()->getType())] -= 1;
        } else if (event.getTypeOfEvent() == TypeOfEvent::ArrivalOnScreen) {
            ships_[event.getShip()]->popBack();
        } else if (event.getTypeOfEvent() == TypeOfEvent::StartMovingToCrane) {
            ships_[event.getShip()]->popBack();
            ships_at_queue_counters_[static_cast<int64_t>(event.getShip()->getType())] += 1;
        } else if (event.getTypeOfEvent() == TypeOfEvent::FinishOfUnloading) {
            schedule_area_->changeUnload(schedule_idxs_[event.getShip()->getName()]);
            unloaded_ships_scroll_->popBack();
            ships_[event.getShip()]->popBack();
        }
    }
};
