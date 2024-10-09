//
// Created by Тимур Ахметзянов on 07.10.2024.
//

#include "../header.h"
#include "MovingEvent.h"


class DrawableShip : public kat::Image {
 public:
    DrawableShip(int type, sf::RenderWindow* parent) {
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
        if (events[0].getCoorInTime(time).first == -1e5) return 1;
        if (events.back().getCoorInTime(time).first == 1e5) return 0;

        for (int i = 0; i < events.size(); ++i) {
            if (std::abs(events[i].getCoorInTime(time).first) != 1e5) {
                setX(events[i].getCoorInTime(time).first + delta_x_);
                setY(events[i].getCoorInTime(time).second);
                return 0;
            } else if (i > 0 && events[i].getCoorInTime(time).first == -events[i - 1].getCoorInTime(time).first &&
                        std::abs(events[i].getCoorInTime(time).first) == 1e5) {
                setX(events[i - 1].getEndPos().first + delta_x_);
                setY(events[i - 1].getEndPos().second);
                return 0;
            }
        }

        return 1;
    }

    void addEvent(const MovingEvent& event) {
        events.push_back(event);
    }

    void moveX(float delta) override {
        delta_x_ += delta;
        kat::Image::moveX(delta);
    }

 private:
    std::vector<MovingEvent> events;
    float delta_x_;
};
