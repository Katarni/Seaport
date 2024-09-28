#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "Ship.h"
#include "Event.h"

class Manager {
public:
    std::vector<Event> GetEvents(int time);

    int GetCountOfShips();

    double GetAverageWaitingTime();

    int64_t GetFine();

    int64_t GetMaxDelay();

    double GetAverageDelay();

    double GetAverageQueue();


private:
    std::vector<Ship> ships_;
    std::map<int, std::vector<Event>> events_;
    int64_t fine_, total_expectation_, total_delay_, max_delay_, total_waiting_time_;
};

