//
// Created by Тимур Ахметзянов on 07.10.2024.
//

#include "../header.h"
#include "MovingEvent.h"


class DrawableShip : public kat::Image {
 public:
    DrawableShip(float y, int type, sf::RenderWindow* parent) {
        std::vector<std::string> clrs = {"red", "blue", "viol"};
        std::vector<std::string> types = {"container", "granular", "liquid"};
        std::random_device rd;
        std::mt19937 rng(rd());
        loadFromFile("../sprites/" + types[type] + "-ship/" + clrs[rng() % 3]);

        setParent(parent);
        setY(y);
        setX(-getWidth());
    }

    void updCoorInTime(int64_t time) {
        if (events[0].getCoorInTime(time).first == -1e5) return;
        if (events.back().getCoorInTime(time).first == 1e5) return;

        for (int i = 1; i < events.size(); ++i) {
            if (abs(events[i].getCoorInTime(time).first) != 1e5) {
                setX(events[i].getCoorInTime(time).first);
                setX(events[i].getCoorInTime(time).second);
                return;
            } else if (events[i].getCoorInTime(time).first == -events[i - 1].getCoorInTime(time).first) {
                setX(events[i - 1].getEndPos().first);
                setX(events[i - 1].getEndPos().second);
                return;
            }
        }
    }

    void addEvent(const MovingEvent& event) {
        events.push_back(event);
    }

 private:
    std::vector<MovingEvent> events;
};
