#include "Manager.h"

int GetRandomFromRange(int left, int right) {
    std::random_device rd;
    std::mt19937 rng(rd());
    return rng() % (right - left + 1) + left;
}

Manager::Manager() {
    fine_ = total_delay_ = max_delay_ = total_waiting_time_ = 0;
    count_liquid_cranes_ = count_granular_cranes = count_container_cranes_ = 1;
    delay_min_ = 0, delay_max_ = 0;
    late_arrival_min_ = 0;
    late_arrival_max_ = 0;
    max_time_ = 0;
}

void Manager::AddShip(TypeOfCargo type, const std::string& name,
                      int weight, int arrival) {
    int time;
    if (type == TypeOfCargo::Container) {
        time = kSpeedOfContainerCrane * weight;
    } else if (type == TypeOfCargo::Granular) {
        time = kSpeedOfGranularCrane * weight;
    } else {
        time = kSpeedOfLiquidCrane * weight;
    }
    int delay = GetRandomFromRange(delay_min_, delay_max_);
    int late_arrival = GetRandomFromRange(late_arrival_min_,
                                          late_arrival_max_);
    time += delay;
    arrival += late_arrival;
    total_delay_ += delay;
    max_delay_ = std::max(max_delay_, static_cast<int64_t>(delay));
    ships_.emplace_back(type, name, weight, time, arrival);
}

std::vector<Event> Manager::GetEvents(int time) {
    return events_[time];
}

int Manager::GetCountOfShips() const {
    return static_cast<int>(ships_.size());
}

double Manager::GetAverageWaitingTime() const {
    return total_waiting_time_ / (1.0 * ships_.size());
}

int64_t Manager::GetFine() const {
    return fine_;
}

int64_t Manager::GetMaxDelay() const {
    return max_delay_;
}

double Manager::GetAverageDelay() const {
    return total_delay_ / (1.0 * ships_.size());
}

void Manager::SetDelayRange(int left, int right) {
    if (left > right) throw std::runtime_error("Ты тёпленький!!!");
    delay_min_ = left;
    delay_max_ = right;
}

void Manager::SetLateArrivalRange(int left, int right) {
    if (left > right) throw std::runtime_error("Ты тёпленький!!!");
    late_arrival_min_ = left;
    late_arrival_max_ = right;
}

void Manager::SetCountContainerCranes(int count) {
    count_container_cranes_ = count;
}

void Manager::SetCountGranularCranes(int count) {
    count_granular_cranes = count;
}

void Manager::SetCountLiquidCranes(int count) {
    count_liquid_cranes_ = count;
}

void Manager::ModelingForOneType(std::vector<Ship*> &ships) {
    if (ships.empty()) return;
    std::sort(ships.begin(), ships.end());
    int count;
    if (ships[0]->GetType() == TypeOfCargo::Container) {
        count = count_container_cranes_;
    } else if (ships[0]->GetType() == TypeOfCargo::Granular) {
        count = count_granular_cranes;
    } else {
        count = count_liquid_cranes_;
    }
    std::sort(ships.begin(), ships.end());
    std::set<std::pair<int, int>> cranes;
    for (int i = 0; i < count; ++i) {
        cranes.insert({0, i});
    }
    for (auto& ship : ships) {
        events_[ship->GetArrival()].emplace_back(ship->GetArrival(),
                                                TypeOfEvent::ArrivalOfShip,
                                                ship);
        auto [time, id] = *cranes.begin();
        cranes.erase(cranes.begin());
        fine_ += std::max(0, time - ship->GetArrival()) * kFine;
        total_waiting_time_ += std::max(0, time - ship->GetArrival());
        time = std::max(time, ship->GetArrival());
        events_[time].emplace_back(id, time,
                                   TypeOfEvent::StartOfUnloading,
                                   ship);
        time += ship->GetTime();
        events_[time].emplace_back(id, time,
                                   TypeOfEvent::FinishOfUnloading,
                                   ship);
        cranes.insert({time, id});
        max_time_ = std::max(max_time_, time);
    }
}

void Manager::Modeling() {
    std::vector<Ship*> container_ships, liquid_ships, granular_ships;
    for (auto& ship : ships_) {
        if (ship.GetType() == TypeOfCargo::Container) {
            container_ships.push_back(&ship);
        } else if (ship.GetType() == TypeOfCargo::Granular) {
            granular_ships.push_back(&ship);
        } else {
            liquid_ships.push_back(&ship);
        }
    }
    ModelingForOneType(container_ships);
    ModelingForOneType(liquid_ships);
    ModelingForOneType(granular_ships);
}

void Manager::AddShips(const std::vector<Ship>& ships) {
    for (auto& ship : ships) {
        std::string name = ship.GetName();
        AddShip(ship.GetType(), name, ship.GetWeight(),
                ship.GetArrival());
    }
}

Event Manager::GetNext() {
    while (cur_time_ <= max_time_ && events_[cur_time_].size() == ptr_) cur_time_++, ptr_ = 0;
    if (cur_time_ > max_time_) throw std::runtime_error("!!!йухан идИ");
    return events_[cur_time_][ptr_];
}

double Manager::GetAverageQueue() {
    int balance = 0;
    int64_t total_queue = 0;
    for (int i = 1; i <= max_time_; ++i) {
        for (auto& event : events_[i]) {
            if (event.GetTypeOfEvent() == TypeOfEvent::ArrivalOfShip) {
                ++balance;
            } else if (event.GetTypeOfEvent() == TypeOfEvent::StartOfUnloading) {
                --balance;
            }
        }
        total_queue += balance;
    }
    return total_queue / (1.0 * max_time_);
}

void Manager::SetEventTime(int time) {
    cur_time_ = time;
    ptr_ = 0;
}
