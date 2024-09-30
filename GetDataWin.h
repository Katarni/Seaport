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
            random_limits_lbl_[i]->setY(43 + static_cast<float>(i) * (random_limits_lbl_[i]->getHeight()  + 26));
            random_limits_lbl_[i]->setFontSize(20);
            random_limits_lbl_[i]->setFont("../fonts/SyneMono.ttf");
        }
        random_limits_lbl_[0]->setData("Unloading delay\nlimits");
        random_limits_lbl_[1]->setData("The limits of deviation\nfrom the schedule");

        random_limits_input_.resize(4);
        for (int i = 0; i < 4; ++i) {
            random_limits_input_[i] = new kat::TextInput;
            random_limits_input_[i]->setParent(window_);
            random_limits_input_[i]->resize(130, 32);
            random_limits_input_[i]->setFontSize(20);
            random_limits_input_[i]->setFont("../fonts/SyneMono.ttf");
            random_limits_input_[i]->setPlaceHolder(i % 2 ? "max" : "min");
            random_limits_input_[i]->setBorderRadius(15);
            random_limits_input_[i]->setBorderColor(sf::Color(31, 184, 193));
            random_limits_input_[i]->setBorderBold(2);
            random_limits_input_[i]->setX(425 + static_cast<float>(i % 2) * (55 + random_limits_input_[i]->getWidth()));
            random_limits_input_[i]->setY(52 + static_cast<float>(i > 1) * (33 + random_limits_input_[i]->getHeight()));
        }
    }

    ~GetDataWin() {
        for (int i = 0; i < 2; ++i) {
            delete random_limits_lbl_[i];
        }

        for (int i = 0; i < 4; ++i) {
            delete random_limits_input_[i];
        }
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

                    for (int i = 0; i < 4; ++i) {
                        random_limits_input_[i]->isPressed(static_cast<float>(event.mouseButton.x),
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
                        for (int i = 0; i < 4; ++i) {
                            if (random_limits_input_[i]->isSelected()) {
                                random_limits_input_[i]->delCharacter();
                            }
                        }
                    }

                    if (event.key.code == sf::Keyboard::Space ||
                        event.key.code == sf::Keyboard::Enter) {
                        continue;
                    }

                    char event_char = kat::getCharFromEvent(event, is_shift);
                    for (int i = 0; i < 4; ++i) {
                        if (!random_limits_input_[i]->isSelected()) continue;
                        if (random_limits_input_[i]->getData().size() -
                            (!random_limits_input_[i]->getData().empty() &&
                            random_limits_input_[i]->getData()[0] == '-') >= 2)
                            continue;

                        if ('0' <= event_char && event_char <= '9' ||
                            event_char == '-' && random_limits_input_[i]->getData().empty()) {
                            random_limits_input_[i]->addCharacter(event_char);
                        }
                    }
                }
            }

            window_->clear(sf::Color::White);

            for (int i = 0; i < 2; ++i) {
                random_limits_lbl_[i]->render();
            }
            for (int i = 0; i < 4; ++i) {
                random_limits_input_[i]->render();
            }

            window_->display();
        }


    }

 private:
    int width_, height_;

    sf::RenderWindow* window_;

    std::vector<kat::TextInput*> random_limits_input_;
    std::vector<kat::Label*> random_limits_lbl_;
};
