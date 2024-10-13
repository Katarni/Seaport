//
// Created by Тимур Ахметзянов on 12.10.2024.
//

#pragma once

#include "../header.h"


class ResultWin {
 public:
    explicit ResultWin(Manager *manager) : manager_(manager) {
        width_ = 850, height_ = 600;

        window_ = new sf::RenderWindow(sf::VideoMode(width_, height_), "Results");

        title_lbl_ = new kat::Button;
        title_lbl_->setParent(window_);
        title_lbl_->resize(150, 40);
        title_lbl_->setData("Results");
        title_lbl_->setY(30);
        title_lbl_->setFontSize(30);
        title_lbl_->setFont("../fonts/KodeMono.ttf");
        title_lbl_->setX(static_cast<float>(width_ - 150) / 2);

        results_lbls_.resize(6);
        results_data_.resize(6);
        std::vector<std::string> results_vals_lbls = {"ships: ", "average queue: ",
                                                      "average waiting: ", "max delay:",
                                                      "average delay: ", "fee: "},
                results_vals_data = {format(manager_->getCountOfShips()),
                                     format(manager_->getAverageQueue()),
                                     format(manager_->getAverageWaitingTime()),
                                     format(manager_->getMaxDelay()),
                                     format(manager_->getAverageDelay()),
                                     format(manager_->getFee())};

        for (int i = 0; i < 6; ++i) {
            results_lbls_[i] = new kat::Button;
            results_lbls_[i]->setParent(window_);
            results_lbls_[i]->setFontSize(25);
            results_lbls_[i]->setFont("../fonts/KodeMono.ttf");
            results_lbls_[i]->setX(static_cast<float>(width_ / 2.0 - 150) / 2 + 50);
            results_lbls_[i]->resize(150, 30);
            if (i == 0) {
                results_lbls_[i]->setY(140);
            } else {
                results_lbls_[i]->setY(results_lbls_[i - 1]->getY() + results_lbls_[i - 1]->getHeight() + 20);
            }
            results_lbls_[i]->setData(results_vals_lbls[i]);

            results_data_[i] = new kat::Button;
            results_data_[i]->setParent(window_);
            results_data_[i]->setFontSize(25);
            results_data_[i]->setFont("../fonts/KodeMono.ttf");
            results_data_[i]->setX(static_cast<float>((width_ / 2.0 - 50) / 2 + width_ / 2.0) - 50);
            results_data_[i]->resize(150, 30);
            if (i == 0) {
                results_data_[i]->setY(140);
            } else {
                results_data_[i]->setY(results_lbls_[i - 1]->getY() + results_lbls_[i - 1]->getHeight() + 20);
            }
            results_data_[i]->setData(results_vals_data[i]);

            results_data_[i]->setBorderRadius(15);
            results_data_[i]->setBorderColor(kBlue);
            results_data_[i]->setBorderBold(2);
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

            title_lbl_->render();

            for (auto &elm: results_lbls_) {
                elm->render();
            }
            for (auto &elm: results_data_) {
                elm->render();
            }

            window_->display();
        }
    }

 private:
    int width_, height_;
    sf::RenderWindow *window_;

    Manager *manager_;

    kat::Button *title_lbl_;
    std::vector<kat::Button *> results_lbls_, results_data_;

    static std::string format(int num) {
        return std::to_string(num);
    }

    static std::string format(int64_t num) {
        return std::to_string(num);
    }

    static std::string format(double num) {
        std::string res = std::to_string(num);
        while (res.back() == '0') {
            res.pop_back();
        }

        if (res.back() == '.') {
            res.pop_back();
        }

        return res;
    }
};
