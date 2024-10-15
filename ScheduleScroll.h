//
// Created by Тимур Ахметзянов on 15.10.2024.
//

#pragma once

#include "DrawableScheduleItem.h"


class ScheduleScroll : public kat::VerScrollArea {
 public:
    explicit ScheduleScroll(sf::RenderWindow* parent) : kat::VerScrollArea(parent) {}

    void changeUnload(size_t idx) {
        if (dynamic_cast<DrawableScheduleItem*>(getElms()[idx]) == nullptr) {
            return;
        }

        dynamic_cast<DrawableScheduleItem*>(getElms()[idx])->changeUnload();
    }
};
