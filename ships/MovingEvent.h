//
// Created by Тимур Ахметзянов on 07.10.2024.
//

#pragma once

#include "../header.h"


class MovingEvent {
 public:
    MovingEvent(float from_x, float from_y,
                float delta_x, float delta_y,
                bool is_y_first, int64_t start_time) : from_x_(from_x), from_y_(from_y),
                                                       delta_x_(delta_x), delta_y_(delta_y),
                                                    is_y_first_(is_y_first), start_time_(start_time) {
        float some_speed = 72;
        end_time_ = static_cast<int64_t>(static_cast<float>(start_time_) + (delta_x + delta_y) / some_speed);
    }

    [[nodiscard]]
    std::pair<float, float> CoorInTime(int64_t time) const {
        if (time < start_time_) return {-1, -1};
        if (time > end_time_) return {-1, -1};

        float some_speed = 72;

        if (!is_y_first_) {
            if (static_cast<float>(time - start_time_) * some_speed <= delta_x_) {
                return {from_x_ + static_cast<float>(time - start_time_) * some_speed, from_y_};
            }

            return {from_x_ + delta_x_,
                    from_y_ + (static_cast<float>(time - start_time_) - (delta_x_ / some_speed)) * some_speed};
        }

        if (static_cast<float>(time - start_time_) * some_speed <= delta_y_) {
            return {from_x_, from_y_ + static_cast<float>(time - start_time_) * some_speed};
        }

        return {from_x_ + (static_cast<float>(time - start_time_) - (delta_y_ / some_speed)) * some_speed,
                from_y_ + delta_y_};
    }

 private:
    float from_x_, from_y_, delta_x_, delta_y_;
    bool is_y_first_;
    int64_t start_time_, end_time_; // all include
};
