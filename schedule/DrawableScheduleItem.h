//
// Created by Тимур Ахметзянов on 14.10.2024.
//

#pragma once

#include "../header.h"


class DrawableScheduleItem : public kat::Div {
 public:
    explicit DrawableScheduleItem(const ScheduleItem& item, sf::RenderWindow* parent) : kat::Div(parent) {
        resize(472, 88);
        setBorderRadius(15);
        setBorderColor(kBlue);
        setBorderBold(2);
        setBackgroundColor(sf::Color::White);

        labels_.resize(5);
        vals_.resize(5);
        std::vector<float> lbls_x = {43, 207, 300, 23, 230},
                            lbls_width = {48, 48, 72, 84, 107},
                            vals_width = {105, 34, 54, 105, 105};
        std::vector<std::string> lbls_text = {"name", "type", "weight", "arrival", "unloading"};
        for (int i = 0; i < 5; ++i) {
            labels_[i] = new kat::Button(parent);
            labels_[i]->setFont("../fonts/KodeMono.ttf");
            labels_[i]->setFontSize(20);
            labels_[i]->setY(i < 3 ? 12 : 53);
            labels_[i]->setData(lbls_text[i]);
            labels_[i]->setX(lbls_x[i]);
            labels_[i]->resize(lbls_width[i], 26);

            vals_[i] = new kat::Button(parent);
            vals_[i]->setFont("../fonts/KodeMono.ttf");
            vals_[i]->setFontSize(18);
            vals_[i]->setY(labels_[i]->getY());
            vals_[i]->resize(vals_width[i], 26);
            vals_[i]->setX(labels_[i]->getX() + labels_[i]->getWidth() + 3);
            vals_[i]->setBorderBold(2);
            vals_[i]->setBorderColor(kBlue);
            vals_[i]->setBorderRadius(8);
        }
        vals_[0]->setData(item.getName());
        vals_[1]->setData(std::to_string(static_cast<int>(item.getType())));
        vals_[2]->setData(std::to_string(item.getWeight()));
        vals_[3]->setData(intToTime(item.getArrival()));
        vals_[4]->setData(intToTime(item.getUnloadTime()));

        unloaded_ = new kat::Label(parent);
        unloaded_->resize(20, 20);
        unloaded_->setX(10);
        unloaded_->setY(34);
        unloaded_->setBorderRadius(7);
        unloaded_->setBorderBold(1);
        unloaded_->setBorderColor(kBlue);
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

        unloaded_->moveX(getX());
        unloaded_->moveY(getY());
        unloaded_->render();
        unloaded_->moveX(-getX());
        unloaded_->moveY(-getY());
    }

    void changeUnload() {
        if (unloaded_->getColor() == kBlue) {
            unloaded_->setBackgroundColor(sf::Color::White);
        } else {
            unloaded_->setBackgroundColor(kBlue);
        }
    }

 private:
    std::vector<kat::Button*> labels_, vals_;
    kat::Label *unloaded_;
};
