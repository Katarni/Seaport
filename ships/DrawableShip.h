//
// Created by Тимур Ахметзянов on 07.10.2024.
//

#include "../header.h"
#include "MovingEvent.h"


class DrawableShip : public kat::Image {
 public:
    DrawableShip(int type, sf::RenderWindow* parent) {
        std::vector<std::string> clrs = {"red", "blue", "viol"};
        std::vector<std::string> types = {"container", "granular", "liquid"};
        std::random_device rd;
        std::mt19937 rng(rd());
        loadFromFile("../sprites/" + types[type] + "-ship/" + clrs[rng() % 3] + ".png");

        setParent(parent);
    }

    int updCoorInTime(int64_t time) {
        if (events[0].getCoorInTime(time).first == -1e5) return 1;
        if (events.back().getCoorInTime(time).first == 1e5) return 0;

        for (int i = 0; i < events.size(); ++i) {
            if (std::abs(events[i].getCoorInTime(time).first) != 1e5) {
                setX(events[i].getCoorInTime(time).first);
                setY(events[i].getCoorInTime(time).second);
                return 0;
            } else if (i > 0 && events[i].getCoorInTime(time).first == -events[i - 1].getCoorInTime(time).first &&
                        std::abs(events[i].getCoorInTime(time).first) == 1e5) {
                setX(events[i - 1].getEndPos().first);
                setY(events[i - 1].getEndPos().second);
                return 0;
            }
        }
    }

    void addEvent(const MovingEvent& event) {
        events.push_back(event);
    }

 private:
    std::vector<MovingEvent> events;
};
