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

    [[nodiscard]]
    int64_t getWaitingTime() const;

    [[nodiscard]]
    int64_t getStartOfUnloading() const;

    void setArrival(int64_t arrival);

    void setUnloadTime(int64_t unload_time);

    void setStartOfUnloading(int64_t time);

    void setWaitingTime(int64_t time);

    void addToFee(int64_t delta);

private:
    int64_t fee_, start_of_unloading_, waiting_time_;
};
