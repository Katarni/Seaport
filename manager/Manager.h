#pragma once
#include "Ship.h"
#include "Event.h"

class Manager {
public:
    Manager();

    void setShips(std::vector<Ship>& ships);

    void modeling();

    int getCountOfShips() const;

    double getAverageWaitingTime() const;

    int64_t getFine() const;

    int64_t getMaxDelay() const;

    double getAverageDelay() const;

    double getAverageQueue();

    void setDelayRange(int left, int right);

    void setLateArrivalRange(int left, int right);

    void setCountContainerCranes(int count);

    void setCountGranularCranes(int count);

    void setCountLiquidCranes(int count);

    Event getNext();

    void setEventTime(int time);


private:
    void ModelingForOneType(std::vector<Ship*>& ships);

    std::vector<Ship> ships_;
    std::vector<Event> events_;
    int64_t fee_, total_delay_,
            max_delay_, total_waiting_time_,
            delay_min_, delay_max_,
            late_arrival_min_, late_arrival_max_,
            count_container_cranes_, count_granular_cranes,
            count_liquid_cranes_,
            ptr_, max_time_;
};

