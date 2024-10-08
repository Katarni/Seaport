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
        end_time_ = static_cast<int64_t>(static_cast<float>(start_time_) + (std::abs(delta_x) +
                                                                            std::abs(delta_y)) / kSpeedShip);
    }

    [[nodiscard]]
    std::pair<float, float> getCoorInTime(int64_t time) const {
        if (time < start_time_) return {-1e5, -1e5};
        if (time > end_time_) return {1e5, 1e5};

        if (!is_y_first_) {
            if (static_cast<float>(time - start_time_) * kSpeedShip <= delta_x_) {
                return {from_x_ + static_cast<float>(time - start_time_)
                                * kSpeedShip * (delta_x_ > 0 ? 1.0 : -1.0), from_y_};
            }

            return {from_x_ + delta_x_,
                    from_y_ + (static_cast<float>(time - start_time_) -
                                (std::abs(delta_x_) / kSpeedShip))
                                * kSpeedShip * (delta_y_ > 0 ? 1.0 : -1.0)};
        }

        if (static_cast<float>(time - start_time_) * kSpeedShip <= delta_y_) {
            return {from_x_, from_y_ + static_cast<float>(time - start_time_) *
                                        kSpeedShip * (delta_y_ > 0 ? 1.0 : -1.0)};
        }

        return {from_x_ + (static_cast<float>(time - start_time_) -
                            (std::abs(delta_y_) / kSpeedShip)) *
                            kSpeedShip * (delta_x_ > 0 ? 1.0 : -1.0),
                from_y_ + delta_y_};
    }

    [[nodiscard]]
    std::pair<float, float> getEndPos() const {
        return {from_x_ + delta_x_, from_y_ + delta_y_};
    }

 private:
    float from_x_, from_y_, delta_x_, delta_y_;
    bool is_y_first_;
    int64_t start_time_, end_time_; // all include
};
