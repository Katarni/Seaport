//
// Created by Тимур Ахметзянов on 29.09.2024.
//

#pragma once

#include "ShipInput.h"


class GetDataWin {
 public:
    GetDataWin() {
        width_ = 850;
        height_ = 600;

        last_ship_y = 226;

        reg_font_.loadFromFile("../fonts/KodeMono.ttf");

        window_ = new sf::RenderWindow(sf::VideoMode(width_, height_), "Gat Data");

        random_limits_lbl_.resize(2);
        for (int i = 0; i < 2; ++i) {
            random_limits_lbl_[i] = new kat::Label;
            random_limits_lbl_[i]->setParent(window_);
            random_limits_lbl_[i]->resize(276, 49);
            random_limits_lbl_[i]->setX(83);
            random_limits_lbl_[i]->setY(43 + static_cast<float>(i) * (random_limits_lbl_[i]->getHeight()  + 16));
            random_limits_lbl_[i]->setFontSize(20);
            random_limits_lbl_[i]->setFont(reg_font_);
        }
        random_limits_lbl_[0]->setData("Unloading delay\nlimits");
        random_limits_lbl_[1]->setData("The limits of deviation\nfrom the schedule");

        random_limits_input_.resize(4);
        for (int i = 0; i < 4; ++i) {
            random_limits_input_[i] = new kat::TextInput;
            random_limits_input_[i]->setParent(window_);
            random_limits_input_[i]->resize(130, 32);
            random_limits_input_[i]->setFontSize(20);
            random_limits_input_[i]->setFont(reg_font_);
            random_limits_input_[i]->setPlaceHolder(i % 2 ? "max" : "min");
            random_limits_input_[i]->setBorderRadius(15);
            random_limits_input_[i]->setBorderColor(sf::Color(31, 184, 193));
            random_limits_input_[i]->setBorderBold(2);
            random_limits_input_[i]->setX(425 + static_cast<float>(i % 2) * (55 + random_limits_input_[i]->getWidth()));
            random_limits_input_[i]->setY(52 + static_cast<float>(i > 1) * (33 + random_limits_input_[i]->getHeight()));
        }

        counters_lbl_.resize(4);
        std::vector<float> counters_lbl_width = {156, 127, 145, 57}, counters_lbl_x = {83, 328, 568, 83};
        for (int i = 0; i < 4; ++i) {
            counters_lbl_[i] = new kat::Label;
            counters_lbl_[i]->setParent(window_);
            counters_lbl_[i]->resize(counters_lbl_width[i], 17);
            counters_lbl_[i]->setY(i < 3 ? 170 : 197);
            counters_lbl_[i]->setX(counters_lbl_x[i]);
            counters_lbl_[i]->setFontSize(15);
            counters_lbl_[i]->setFont(reg_font_);
        }
        counters_lbl_[0]->setData("Container cranes:");
        counters_lbl_[1]->setData("Liquid cranes:");
        counters_lbl_[2]->setData("Granular cranes:");
        counters_lbl_[3]->setData("Ships:");

        counters_input_.resize(4);
        for (int i = 0; i < 4; ++i) {
            counters_input_[i] = new kat::TextInput;
            counters_input_[i]->setParent(window_);
            counters_input_[i]->resize(43, 19);
            counters_input_[i]->setY(counters_lbl_[i]->getY() + 1);
            counters_input_[i]->setX(counters_lbl_[i]->getX() + counters_lbl_[i]->getWidth() + 5);
            counters_input_[i]->setFontSize(15);
            counters_input_[i]->setFont(reg_font_);
            counters_input_[i]->setPlaceHolder("1");
            counters_input_[i]->setBorderRadius(5);
            counters_input_[i]->setBorderColor(sf::Color(31, 184, 193));
            counters_input_[i]->setBorderBold(2);
        }

        ships_scroll_area_ = new kat::VerScrollArea(83, 226, 657, 360, window_);
    }

    ~GetDataWin() {
        for (auto& elm : random_limits_lbl_) {
            delete elm;
        }
        for (auto& elm : random_limits_input_) {
            delete elm;
        }
        for (auto& elm : counters_lbl_) {
            delete elm;
        }
        for (auto& elm : counters_input_) {
            delete elm;
        }

        delete ships_scroll_area_;
    }

    void open() {
        while (window_->isOpen()) {
            sf::Event event{};
            while (window_->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window_->close();
                }

                if (event.type == sf::Event::MouseWheelMoved) {
                    if (ships_scroll_area_->isHovered(static_cast<float>(event.mouseButton.x),
                                                      static_cast<float>(event.mouseButton.y))) {
                        ships_scroll_area_->moveY(static_cast<float>(kat::sign(event.mouseWheel.delta) * 5));
                    }
                }

                if (event.type == sf::Event::MouseButtonPressed &&
                    event.mouseButton.button == sf::Mouse::Button::Left) {
                    for (auto& elm : random_limits_input_) {
                        elm->isPressed(static_cast<float>(event.mouseButton.x),
                                       static_cast<float>(event.mouseButton.y));
                    }
                    for (auto& elm : counters_input_) {
                        elm->isPressed(static_cast<float>(event.mouseButton.x),
                                       static_cast<float>(event.mouseButton.y));
                    }
                    ships_scroll_area_->isPressed(static_cast<float>(event.mouseButton.x),
                                                  static_cast<float>(event.mouseButton.y));
                }

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::RShift ||
                        event.key.code == sf::Keyboard::LShift) {
                        is_shift_ = true;
                    }
                }

                if (event.type == sf::Event::KeyReleased) {
                    mouseButtonRealised(event);
                }
            }

            window_->clear(sf::Color::White);

            for (auto& elm : random_limits_lbl_) {
                elm->render();
            }
            for (auto& elm : random_limits_input_) {
                elm->render();
            }
            for (auto& elm : counters_lbl_) {
                elm->render();
            }
            for (auto& elm : counters_input_) {
                elm->render();
            }

            ships_scroll_area_->render();

            window_->display();
        }
    }

 private:
    bool is_shift_ = false;
    int width_, height_;
    float last_ship_y;

    sf::Font reg_font_;

    sf::RenderWindow* window_;

    std::vector<kat::TextInput*> random_limits_input_;
    std::vector<kat::Label*> random_limits_lbl_;
    std::vector<kat::Label*> counters_lbl_;
    std::vector<kat::TextInput*> counters_input_;

    kat::VerScrollArea* ships_scroll_area_;

    void mouseButtonRealised(const sf::Event& event) {
        if (event.key.code == sf::Keyboard::RShift ||
            event.key.code == sf::Keyboard::LShift) {
            is_shift_ = false;
        }

        if (event.key.code == sf::Keyboard::BackSpace) {
            for (auto& elm : random_limits_input_) {
                if (elm->isSelected()) {
                    elm->delCharacter();
                }
            }
            for (auto& elm : counters_input_) {
                if (elm->isSelected()) {
                    elm->delCharacter();
                }
            }
            for (auto& elm : ships_scroll_area_->getElms()) {
                if (dynamic_cast<ShipInput*>(elm)->isSelected()) {
                    dynamic_cast<ShipInput*>(elm)->delCharacter();
                }
            }
        }

        if (event.key.code == sf::Keyboard::Space) {
            return;
        }

        if (event.key.code == sf::Keyboard::Enter) {
            if (counters_input_[3]->isSelected()) {
                int count = std::stoi(counters_input_[3]->getData().empty() ? "0" : counters_input_[3]->getData());
                while (ships_scroll_area_->getElms().size() < count) {
                    ships_scroll_area_->addElm(new ShipInput(last_ship_y, reg_font_, window_));
                    last_ship_y += 73;
                }

                while (ships_scroll_area_->getElms().size() > count) {
                    ships_scroll_area_->removeElm(static_cast<int>(ships_scroll_area_->getElms().size() - 1));
                    last_ship_y -= 73;
                }
            }
        }

        char event_char = kat::getCharFromEvent(event, is_shift_);
        for (auto& elm : random_limits_input_) {
            if (!elm->isSelected()) continue;
            if (elm->getData().size() -
                (!elm->getData().empty() &&
                 elm->getData()[0] == '-') >= 2) {
                continue;
            }

            if ('0' <= event_char && event_char <= '9' ||
                event_char == '-' && elm->getData().empty()) {
                elm->addCharacter(event_char);
            }
        }

        for (auto& elm : counters_input_) {
            if (!elm->isSelected()) continue;
            if (elm->getData().size() >= 2) continue;

            if ('0' <= event_char && event_char <= '9') {
                elm->addCharacter(event_char);
            }
        }

        for (auto& elm : ships_scroll_area_->getElms()) {
            if (dynamic_cast<ShipInput*>(elm)->isSelected()) {
                dynamic_cast<ShipInput*>(elm)->addCharacter(event_char);
            }
        }
    }
};
