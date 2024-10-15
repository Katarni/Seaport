#pragma once
#include "Ship.h"
#include "Event.h"

class Manager {
public:
    Manager();

    void setShips(std::vector<ScheduleItem*>& ships);

    void modeling();

    [[nodiscard]]
    int getCountOfShips() const;

    [[nodiscard]]
    double getAverageWaitingTime() const;

    [[nodiscard]]
    int64_t getFee() const;

    [[nodiscard]]
    int64_t getMaxDelay() const;

    [[nodiscard]]
    double getAverageDelay() const;

    double getAverageQueue();

    void setDelayRange(int left, int right);

    void setLateArrivalRange(int left, int right);

    void setCountContainerCranes(int count);

    void setCountGranularCranes(int count);

    void setCountLiquidCranes(int count);

    Event getCur();

    int goNext();
    int goPrev();

    void setEventTime(int time);

    std::vector<ScheduleItem*>& getSchedule();


private:
    void ModelingForOneType(std::vector<Ship*>& ships);

    std::vector<Ship> ships_;
    std::vector<Event> events_;
    std::vector<ScheduleItem*> schedule_;
    int64_t fee_, total_delay_,
            max_delay_, total_waiting_time_,
            delay_min_, delay_max_,
            late_arrival_min_, late_arrival_max_,
            count_container_cranes_, count_granular_cranes,
            count_liquid_cranes_,
            ptr_, max_time_;
};

