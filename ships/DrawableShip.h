//
// Created by Тимур Ахметзянов on 07.10.2024.
//

#include "MovingEvent.h"


class DrawableShip : public kat::Image {
 public:
    DrawableShip(int type, sf::RenderWindow* parent, Ship* ship) : ship_(ship), is_selected_(false) {
        std::vector<std::string> clrs = {"red", "blue", "viol"};
        std::vector<std::string> types = {"container", "liquid", "granular"};
        std::random_device rd;
        std::mt19937 rng(rd());
        std::string path = "../sprites/" + types[type] + "-ship/" + clrs[rng() % 3];
        if (type == 1) {
            path += rng() % 2 ? "-green" : "-blue";
        } else if (type == 2) {
            path += rng() % 2 ? "-yellow" : "-grey";
        }
        loadFromFile(path + ".png");

        setParent(parent);

        delta_x_ = 0;
    }

    int updCoorInTime(int64_t time) {
        if (events_[0].getCoorInTime(time).first == -1e5) return 1;
        if (events_.back().getCoorInTime(time).first == 1e5) {
            setX(events_.back().getEndPos().first + delta_x_);
            setY(events_.back().getEndPos().second);
            return 0;
        }

        for (int i = 0; i < events_.size(); ++i) {
            if (std::abs(events_[i].getCoorInTime(time).first) != 1e5) {
                setX(events_[i].getCoorInTime(time).first + delta_x_);
                setY(events_[i].getCoorInTime(time).second);
                return 0;
            } else if (i > 0 && events_[i].getCoorInTime(time).first == -events_[i - 1].getCoorInTime(time).first &&
                       std::abs(events_[i].getCoorInTime(time).first) == 1e5) {
                setX(events_[i - 1].getEndPos().first + delta_x_);
                setY(events_[i - 1].getEndPos().second);
                return 0;
            }
        }

        return 0;
    }

    void addEvent(const MovingEvent& event) {
        events_.push_back(event);
    }

    void popBack() {
        events_.pop_back();
    }

    void moveX(float delta) override {
        delta_x_ += delta;
        kat::Image::moveX(delta);
    }

    void isSelected(float x, float y) {
        is_selected_ = isHovered(x, y);
    }

 private:
    std::vector<MovingEvent> events_;
    float delta_x_;
    Ship* ship_;
    bool is_selected_;
};
