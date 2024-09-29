#include "Manager.h"

int GetRandomFromRange(int64_t left, int64_t right) {
    std::random_device rd;
    std::mt19937 rng(rd());
    return (int64_t) rng() % (right - left + 1) + left;
}

Manager::Manager() {
    fee_ = total_delay_ = max_delay_ = total_waiting_time_ = 0;
    count_liquid_cranes_ = count_granular_cranes = count_container_cranes_ = 1;
    delay_min_ = 0, delay_max_ = 0;
    late_arrival_min_ = 0;
    late_arrival_max_ = 0;
    max_time_ = 0;
}

int Manager::GetCountOfShips() const {
    return static_cast<int>(ships_.size());
}

double Manager::GetAverageWaitingTime() const {
    if (GetCountOfShips() == 0) return 0;
    return total_waiting_time_ / (1.0 * ships_.size());
}

int64_t Manager::GetFine() const {
    return fee_;
}

int64_t Manager::GetMaxDelay() const {
    return max_delay_;
}

double Manager::GetAverageDelay() const {
    if (GetCountOfShips() == 0) return 0;
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

bool Comp(const Ship* first, const Ship* second) {
    return *first < *second;
}

void Manager::ModelingForOneType(std::vector<Ship*> &ships) {
    if (ships.empty()) return;
    std::sort(ships.begin(), ships.end());
    int64_t count_cranes;
    if (ships[0]->GetType() == TypeOfCargo::Container) {
        count_cranes = count_container_cranes_;
    } else if (ships[0]->GetType() == TypeOfCargo::Granular) {
        count_cranes = count_granular_cranes;
    } else {
        count_cranes = count_liquid_cranes_;
    }
    std::sort(ships.begin(), ships.end(), Comp);
    std::set<std::pair<int64_t, int>> cranes;
    for (int i = 0; i < count_cranes; ++i) {
        cranes.insert({0, i});
    }
    for (auto& ship : ships) {
        events_.emplace_back(ship->GetArrival(),
                             TypeOfEvent::ArrivalOfShip,
                             ship);
        auto [time, id] = *cranes.begin();
        cranes.erase(cranes.begin());
        fee_ += std::max(0ll, time - ship->GetArrival()) * kFee;
        total_waiting_time_ += std::max(0ll, time - ship->GetArrival());
        time = std::max(time, ship->GetArrival());
        events_.emplace_back(id, time,
                             TypeOfEvent::StartOfUnloading,
                             ship);
        time += ship->GetUnloadTime();
        events_.emplace_back(id, time,
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
    std::sort(events_.begin(), events_.end());
    ptr_ = -1;
}

void Manager::AddShips(std::vector<Ship>& ships) {
    for (auto& ship : ships) {
        int64_t time;
        if (ship.GetType() == TypeOfCargo::Container) {
            time = kSpeedOfContainerCrane * ship.GetWeight();
        } else if (ship.GetType() == TypeOfCargo::Granular) {
            time = kSpeedOfGranularCrane * ship.GetWeight();
        } else {
            time = kSpeedOfLiquidCrane * ship.GetWeight();
        }
        int64_t delay = GetRandomFromRange(delay_min_, delay_max_);
        int64_t late_arrival = GetRandomFromRange(late_arrival_min_,
                                                  late_arrival_max_);
        delay = std::max(-time + 1, delay);
        late_arrival = std::max(-ship.GetArrival() + 1, late_arrival);
        time += delay;
        ship.SetArrival(late_arrival + ship.GetArrival());
        total_delay_ += delay;
        max_delay_ = std::max(max_delay_, delay);
        ship.SetUnloadTime(time);
    }
    ships_ = ships;
}

Event Manager::GetNext() {
    ++ptr_;
    if (ptr_ >= events_.size()) throw std::runtime_error("!!!йухан идИ");
    return events_[ptr_];
}

double Manager::GetAverageQueue() {
    int balance = 0;
    int64_t total_queue = 0;
    int cur_event = 0;
    for (int i = 1; i <= max_time_; ++i) {
        while (cur_event < events_.size() && events_[cur_event].GetTime() == i) {
            if (events_[cur_event].GetTypeOfEvent() == TypeOfEvent::ArrivalOfShip) {
                ++balance;
            } else if (events_[cur_event].GetTypeOfEvent() == TypeOfEvent::StartOfUnloading) {
                --balance;
            }
            ++cur_event;
        }
        total_queue += balance;
    }
    return total_queue / (1.0 * max_time_);
}

void Manager::SetEventTime(int time) {
    int left = -1, right = static_cast<int>(events_.size()) - 1;
    while (right - left > 1) {
        int mid = (left + right) / 2;
        if (events_[mid].GetTime() < time) {
            left = mid;
        } else {
            right = mid;
        }
    }
    if (events_[right].GetTime() < time) {
        throw std::runtime_error("Слишком много букв, больше трёх не перевариваю...");
    }
    ptr_ = right - 1;
}