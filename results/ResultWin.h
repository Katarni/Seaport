//
// Created by Тимур Ахметзянов on 12.10.2024.
//

#pragma once

#include "../header.h"


class ResultWin {
 public:
    explicit ResultWin(Manager *manager) : manager_(manager) {
        width_ = 850;
        height_ = 600;

        window_ = new sf::RenderWindow(sf::VideoMode(width_, height_), "Results");

        results_.resize(6);
        std::vector<std::string> results_vals = {"ships: " + std::to_string(manager_->getCountOfShips()),
                                                 "average queue: " + std::to_string(manager_->getAverageQueue()),
                                                 "average waiting: " +
                                                        std::to_string(manager_->getAverageWaitingTime()),
                                                 "max delay:" + std::to_string(manager_->getMaxDelay()),
                                                 "average delay: " + std::to_string(manager_->getAverageDelay()),
                                                 "fee: " + std::to_string(manager_->getFee())};
        for (int i = 0; i < 6; ++i) {
            results_[i] = new kat::Label;
            results_[i]->setParent(window_);
            results_[i]->setFontSize(20);
            results_[i]->setFont("../fonts/KodeMono.ttf");
            results_[i]->setX(30);
            results_[i]->resize(200, 25);
            if (i == 0) {
                results_[i]->setY(30);
            } else {
                results_[i]->setY(results_[i - 1]->getY() + 45);
            }
            results_[i]->setData(results_vals[i]);
        }
    }

    void open() {
        while (window_->isOpen()) {
            sf::Event event{};
            while (window_->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window_->close();
                }
            }

            window_->clear(sf::Color::White);

            for (auto& elm : results_) {
                elm->render();
            }

            window_->display();
        }
    }

 private:
    int width_, height_;
    sf::RenderWindow *window_;

    Manager *manager_;

    std::vector<kat::Label*> results_;
};
