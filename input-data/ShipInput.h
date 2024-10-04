//
// Created by Тимур Ахметзянов on 30.09.2024.
//

#pragma once

#include "../header.h"


class ShipInput : public kat::TextInput {
 public:
    ShipInput(float y, const sf::Font& font, sf::RenderWindow* parent) :
            TextInput(99, y, 626, 58, font, parent) {
        labels_.resize(4);
        inputs_.resize(4);

        setBorderBold(2);
        setBorderColor(sf::Color(31, 184, 193));
        setBorderRadius(15);

        std::vector<float> lbls_width = {48, 48, 72, 84}, lbls_x = {18, 182, 275, 412},
                            inputs_width = {105, 34, 54, 105};
        std::vector<std::string> lbls_text = {"name", "type", "weight", "arrival"},
                                placeholders_text = {"random", "1", "1", "dd:hh:mm"};
        for (int i = 0; i < 4; ++i) {
            labels_[i] = new kat::Label;
            labels_[i]->setParent(getParent());
            labels_[i]->resize(lbls_width[i], 26);
            labels_[i]->setY(16);
            labels_[i]->setX(lbls_x[i]);
            labels_[i]->setData(lbls_text[i]);
            labels_[i]->setFontSize(20);
            labels_[i]->setFont(font);

            inputs_[i] = new kat::TextInput;
            inputs_[i]->setParent(getParent());
            inputs_[i]->setBorderRadius(8);
            inputs_[i]->setBorderColor(sf::Color(31, 184, 193));
            inputs_[i]->setBorderBold(2);
            inputs_[i]->setPlaceHolder(placeholders_text[i]);
            inputs_[i]->setX(labels_[i]->getX() + labels_[i]->getWidth() + 3);
            inputs_[i]->setY(16);
            inputs_[i]->setFontSize(18);
            inputs_[i]->setFont(font);
            inputs_[i]->resize(inputs_width[i], 26);
        }
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

        for (auto elm : inputs_) {
            elm->moveX(getX());
            elm->moveY(getY());
            elm->render();
            elm->moveX(-getX());
            elm->moveY(-getY());
        }
    }

    inline bool isPressed(float x, float y) override {
        x -= getX();
        y -= getY();

        bool pressed = false;
        for (auto elm : inputs_) {
            pressed |= elm->isPressed(x, y);
        }
        return pressed;
    }

    bool isSelected() const override {
        bool selected = false;
        for (auto elm : inputs_) {
            selected |= elm->isSelected();
        }
        return selected;
    }

    void moveCursorRight() override {
        for (auto elm : inputs_) {
            if (elm->isSelected()) {
                elm->moveCursorRight();
            }
        }
    }

    void moveCursorLeft() override {
        for (auto elm : inputs_) {
            if (elm->isSelected()) {
                elm->moveCursorLeft();
            }
        }
    }

    void delCharacter() override {
        for (auto elm : inputs_) {
            if (elm->isSelected()) {
                elm->delCharacter();
            }
        }
    }

    void addCharacter(char character) override {
        if (inputs_[0]->isSelected()) {
            if (inputs_[0]->getData().size() >= 6) return;
            if ('a' <= character && character <= 'z' || 'A' <= character && character <= 'Z' ||
                '0' <= character && character <= '9') {
                inputs_[0]->addCharacter(character);
            }
        } else if (inputs_[1]->isSelected()) {
            if (!inputs_[1]->getData().empty()) return;
            if (character != '0' && character != '1' && character != '2') return;
            inputs_[1]->addCharacter(character);
        } else if (inputs_[2]->isSelected()) {
            if (inputs_[2]->getData().size() >= 3) return;
            if ('0' <= character && character <= '9') {
                inputs_[2]->addCharacter(character);
            }
        } else if (inputs_[3]->isSelected()) {
            bool add = false;
            auto data = inputs_[3]->getData();
            switch (data.size()) {
                case 0:
                    add = '0' <= character && character <= '3';
                    break;
                case 1:
                    add = data[0] == '3' && (character == '0' || character == '1') ||
                            data[0] != '3' && '0' <= character && character <= '9';
                    break;
                case 2:
                    add = character == ':';
                    break;
                case 3:
                    add = '0' <= character && character <= '2';
                    break;
                case 4:
                    add = data[3] == '2' && '0' <= character && character <= '3' ||
                            data[3] != '2' && '0' <= character && character <= '9';
                    break;
                case 5:
                    add = character == ':';
                    break;
                case 6:
                    add = '0' <= character && character <= '5';
                    break;
                case 7:
                    add = '0' <= character && character <= '9';
                    break;
                default:
                    add = false;
            }

            if (add) {
                inputs_[3]->addCharacter(character);
            }
        }
    }

    std::string getName() {
        return labels_[0]->getData();
    }

    int64_t getTime() {
        std::string time_s;
        if (labels_[3]->getData().size() < 8) {
            time_s = "00:00:00";
        }
        time_s = labels_[3]->getData();
        time_s.push_back(':');

        int64_t time = 0, mul = 60 * 24, cur = 0;
        int i = 0;
        for (char c : time_s) {
            if (c == ':') {
                time += cur * mul;
                mul /= (i == 0) ? 24 : 60;
                ++i;
                cur = 0;
                continue;
            }

            cur = cur * 10 + c - '0';
        }

        return time;
    }

    int64_t getType() {
        if (labels_[1]->getData().empty()) {
            return 1;
        }
        return std::stoll(labels_[1]->getData());
    }

    int64_t getWeight() {
        if (labels_[2]->getData().empty()) {
            return 1;
        }
        return std::stoll(labels_[2]->getData());
    }

 private:
    std::vector<kat::Label*> labels_;
    std::vector<kat::TextInput*> inputs_;
};
