#pragma once
#include "includes.h"
#include "ScheduleItem.h"

class Ship : public ScheduleItem {
public:
    Ship(TypeOfCargo type, const std::string& name, int64_t weight, int64_t arrival);

    explicit Ship(const ScheduleItem& item);

    bool operator<(const Ship& other) const;

    [[nodiscard]]
    int64_t getFee() const;

    void setArrival(int64_t arrival);

    void setUnloadTime(int64_t unload_time);

    void addToFee(int64_t delta);

private:
    int64_t fee_;
};
