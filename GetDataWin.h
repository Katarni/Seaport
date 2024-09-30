//
// Created by Тимур Ахметзянов on 29.09.2024.
//

#pragma once

#include "header.h"


class GetDataWin {
 public:
    GetDataWin() {
        width_ = 850;
        height_ = 500;

        window_ = new sf::RenderWindow(sf::VideoMode(width_, height_), "Gat Data");

        random_limits_lbl_.resize(2);
        for (int i = 0; i < 2; ++i) {
            random_limits_lbl_[i] = new kat::Label;
            random_limits_lbl_[i]->setParent(window_);
            random_limits_lbl_[i]->resize(276, 49);
            random_limits_lbl_[i]->setX(83);
            random_limits_lbl_[i]->setY(43 + static_cast<float>(i) * (random_limits_lbl_[i]->getHeight()  + 16));
            random_limits_lbl_[i]->setFontSize(20);
            random_limits_lbl_[i]->setFont("../fonts/KodeMono.ttf");
        }
        random_limits_lbl_[0]->setData("Unloading delay\nlimits");
        random_limits_lbl_[1]->setData("The limits of deviation\nfrom the schedule");

        random_limits_input_.resize(4);
        for (int i = 0; i < 4; ++i) {
            random_limits_input_[i] = new kat::TextInput;
            random_limits_input_[i]->setParent(window_);
            random_limits_input_[i]->resize(130, 32);
            random_limits_input_[i]->setFontSize(20);
            random_limits_input_[i]->setFont("../fonts/KodeMono.ttf");
            random_limits_input_[i]->setPlaceHolder(i % 2 ? "max" : "min");
            random_limits_input_[i]->setBorderRadius(15);
            random_limits_input_[i]->setBorderColor(sf::Color(31, 184, 193));
            random_limits_input_[i]->setBorderBold(2);
            random_limits_input_[i]->setX(425 + static_cast<float>(i % 2) * (55 + random_limits_input_[i]->getWidth()));
            random_limits_input_[i]->setY(52 + static_cast<float>(i > 1) * (33 + random_limits_input_[i]->getHeight()));
        }

        cranes_counters_lbl_.resize(3);
        std::vector<float> cranes_counters_lbl_width = {156, 127, 145}, cranes_counters_lbl_x = {83, 328, 568};
        for (int i = 0; i < 3; ++i) {
            cranes_counters_lbl_[i] = new kat::Label;
            cranes_counters_lbl_[i]->setParent(window_);
            cranes_counters_lbl_[i]->resize(cranes_counters_lbl_width[i], 17);
            cranes_counters_lbl_[i]->setY(170);
            cranes_counters_lbl_[i]->setX(cranes_counters_lbl_x[i]);
            cranes_counters_lbl_[i]->setFontSize(15);
            cranes_counters_lbl_[i]->setFont("../fonts/KodeMono.ttf");
        }
        cranes_counters_lbl_[0]->setData("Container cranes:");
        cranes_counters_lbl_[1]->setData("Liquid cranes:");
        cranes_counters_lbl_[2]->setData("Granular cranes:");

        cranes_counters_input_.resize(3);
        for (int i = 0; i < 3; ++i) {
            cranes_counters_input_[i] = new kat::TextInput;
            cranes_counters_input_[i]->setParent(window_);
            cranes_counters_input_[i]->resize(43, 19);
            cranes_counters_input_[i]->setY(171);
            cranes_counters_input_[i]->setX(cranes_counters_lbl_[i]->getX() + cranes_counters_lbl_[i]->getWidth() + 5);
            cranes_counters_input_[i]->setFontSize(15);
            cranes_counters_input_[i]->setFont("../fonts/KodeMono.ttf");
            cranes_counters_input_[i]->setPlaceHolder("1");
            cranes_counters_input_[i]->setBorderRadius(5);
            cranes_counters_input_[i]->setBorderColor(sf::Color(31, 184, 193));
            cranes_counters_input_[i]->setBorderBold(2);
        }




//        input_scroll_area_ = new kat::VerScrollArea();


    }

    ~GetDataWin() {
        for (auto& elm : random_limits_lbl_) {
            delete elm;
        }
        for (auto& elm : random_limits_input_) {
            delete elm;
        }
        for (auto& elm : cranes_counters_lbl_) {
            delete elm;
        }
        for (auto& elm : cranes_counters_input_) {
            delete elm;
        }

//        delete input_scroll_area_;
    }

    void open() {
        bool is_shift = false;

        while (window_->isOpen()) {
            sf::Event event{};
            while (window_->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window_->close();
                }

                if (event.type == sf::Event::MouseButtonPressed &&
                    event.mouseButton.button == sf::Mouse::Button::Left) {
                    for (auto& elm : random_limits_input_) {
                        elm->isPressed(static_cast<float>(event.mouseButton.x),
                                       static_cast<float>(event.mouseButton.y));
                    }
                    for (auto& elm : cranes_counters_input_) {
                        elm->isPressed(static_cast<float>(event.mouseButton.x),
                                       static_cast<float>(event.mouseButton.y));
                    }
                }

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::RShift ||
                        event.key.code == sf::Keyboard::LShift) {
                        is_shift = true;
                    }
                }

                if (event.type == sf::Event::KeyReleased) {
                    if (event.key.code == sf::Keyboard::RShift ||
                        event.key.code == sf::Keyboard::LShift) {
                        is_shift = false;
                    }

                    if (event.key.code == sf::Keyboard::BackSpace) {
                        for (auto& elm : random_limits_input_) {
                            if (elm->isSelected()) {
                                elm->delCharacter();
                            }
                        }
                        for (auto& elm : cranes_counters_input_) {
                            if (elm->isSelected()) {
                                elm->delCharacter();
                            }
                        }
                    }

                    if (event.key.code == sf::Keyboard::Space ||
                        event.key.code == sf::Keyboard::Enter) {
                        continue;
                    }

                    char event_char = kat::getCharFromEvent(event, is_shift);
                    for (auto& elm : random_limits_input_) {
                        if (!elm->isSelected()) continue;
                        if (elm->getData().size() -
                            (!elm->getData().empty() &&
                                    elm->getData()[0] == '-') >= 2)
                            continue;

                        if ('0' <= event_char && event_char <= '9' ||
                            event_char == '-' && elm->getData().empty()) {
                            elm->addCharacter(event_char);
                        }
                    }

                    for (auto& elm : cranes_counters_input_) {
                        if (!elm->isSelected()) continue;
                        if (elm->getData().size() >= 2) continue;

                        if ('0' <= event_char && event_char <= '9') {
                            elm->addCharacter(event_char);
                        }
                    }
                }
            }

            window_->clear(sf::Color::White);

            for (auto& elm : random_limits_lbl_) {
                elm->render();
            }
            for (auto& elm : random_limits_input_) {
                elm->render();
            }
            for (auto& elm : cranes_counters_lbl_) {
                elm->render();
            }
            for (auto& elm : cranes_counters_input_) {
                elm->render();
            }

            window_->display();
        }
    }

 private:
    int width_, height_;

    sf::RenderWindow* window_;

    std::vector<kat::TextInput*> random_limits_input_;
    std::vector<kat::Label*> random_limits_lbl_;
    std::vector<kat::Label*> cranes_counters_lbl_;
    std::vector<kat::TextInput*> cranes_counters_input_;

//    kat::VerScrollArea* input_scroll_area_;
};
