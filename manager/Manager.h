#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "Ship.h"
#include "Event.h"

class Manager {
public:
    Manager();

    void AddShip(TypeOfCargo type, const std::string& name,
                 int weight, int arrival);

    void AddShips(const std::vector<Ship>& ships);

    void ModelingForOneType(std::vector<Ship*>& ships);

    void Modeling();

    std::vector<Event> GetEvents(int time);

    int GetCountOfShips() const;

    double GetAverageWaitingTime() const;

    int64_t GetFine() const;

    int64_t GetMaxDelay() const;

    double GetAverageDelay() const;

    double GetAverageQueue() const;

    void SetDelayRange(int left, int right);

    void SetLateArrivalRange(int left, int right);

    void SetCountContainerCranes(int count);

    void SetCountGranularCranes(int count);

    void SetCountLiquidCranes(int count);


private:
    std::vector<Ship> ships_;
    std::map<int, std::vector<Event>> events_;
    int64_t fine_, total_delay_,
            max_delay_, total_waiting_time_;
    int delay_min_, delay_max_,
        late_arrival_min_, late_arrival_max_,
        count_container_cranes_, count_granular_cranes,
        count_liquid_cranes_;
};

