//
// Created by Тимур Ахметзянов on 16.10.2024.
//

#pragma once

#include "../header.h"


class UnloadedItem : public kat::Div {
 public:
    explicit UnloadedItem(Ship* ship, sf::RenderWindow* parent) : kat::Div(parent) {
        resize(472, 145);
        setBorderRadius(15);
        setBorderColor(kBlue);
        setBorderBold(2);
        setBackgroundColor(sf::Color::White);

        labels_.resize(4);
        vals_.resize(5);
        std::vector<float> lbls_x = {32, 238, 83, 88},
                lbls_width = {84, 84, 186, 156},
                lbls_y = {44, 44, 75, 107};
        std::vector<std::string> lbls_text = {"arrival", "waiting", "start unloading", "unloading time"};
        vals_[0] = new kat::Button(parent);
        vals_[0]->setFont("../fonts/KodeMono.ttf");
        vals_[0]->setFontSize(18);
        vals_[0]->setY(10);
        vals_[0]->resize(105, 26);
        vals_[0]->setX(183);
        vals_[0]->setBorderBold(2);
        vals_[0]->setBorderColor(kBlue);
        vals_[0]->setBorderRadius(8);
        for (int i = 0; i < 4; ++i) {
            labels_[i] = new kat::Button(parent);
            labels_[i]->setFont("../fonts/KodeMono.ttf");
            labels_[i]->setFontSize(20);
            labels_[i]->setY(lbls_y[i]);
            labels_[i]->setData(lbls_text[i]);
            labels_[i]->setX(lbls_x[i]);
            labels_[i]->resize(lbls_width[i], 26);

            vals_[i + 1] = new kat::Button(parent);
            vals_[i + 1]->setFont("../fonts/KodeMono.ttf");
            vals_[i + 1]->setFontSize(18);
            vals_[i + 1]->setY(labels_[i]->getY());
            vals_[i + 1]->resize(105, 26);
            vals_[i + 1]->setX(labels_[i]->getX() + labels_[i]->getWidth() + 3);
            vals_[i + 1]->setBorderBold(2);
            vals_[i + 1]->setBorderColor(kBlue);
            vals_[i + 1]->setBorderRadius(8);
        }
        vals_[0]->setData(ship->getName());
        vals_[1]->setData(intToTime(ship->getArrival()));
        vals_[2]->setData(intToTime(ship->getWaitingTime()));
        vals_[3]->setData(intToTime(ship->getStartOfUnloading()));
        vals_[4]->setData(intToTime(ship->getUnloadTime()));
    }

    void render() override {
        if (!needRender()) return;
        kat::Div::render();

        for (auto elm : labels_) {
            elm->moveX(getX());
            elm->moveY(getY());
            elm->render();
            elm->moveX(-getX());
            elm->moveY(-getY());
        }

        for (auto elm : vals_) {
            elm->moveX(getX());
            elm->moveY(getY());
            elm->render();
            elm->moveX(-getX());
            elm->moveY(-getY());
        }
    }

 private:
    std::vector<kat::Button*> labels_, vals_;
};
