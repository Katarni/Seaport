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

int Manager::getCountOfShips() const {
    return static_cast<int>(ships_.size());
}

double Manager::getAverageWaitingTime() const {
    if (getCountOfShips() == 0) return 0;
    return total_waiting_time_ / (1.0 * ships_.size());
}

int64_t Manager::getFine() const {
    return fee_;
}

int64_t Manager::getMaxDelay() const {
    return max_delay_;
}

double Manager::getAverageDelay() const {
    if (getCountOfShips() == 0) return 0;
    return total_delay_ / (1.0 * ships_.size());
}

void Manager::setDelayRange(int left, int right) {
    if (left > right) throw std::runtime_error("Ты тёпленький!!!");
    delay_min_ = left;
    delay_max_ = right;
}

void Manager::setLateArrivalRange(int left, int right) {
    if (left > right) throw std::runtime_error("Ты тёпленький!!!");
    late_arrival_min_ = left;
    late_arrival_max_ = right;
}

void Manager::setCountContainerCranes(int count) {
    count_container_cranes_ = count;
}

void Manager::setCountGranularCranes(int count) {
    count_granular_cranes = count;
}

void Manager::setCountLiquidCranes(int count) {
    count_liquid_cranes_ = count;
}

void Manager::ModelingForOneType(std::vector<Ship*> &ships) {
    if (ships.empty()) return;
    std::sort(ships.begin(), ships.end());
    int64_t count_cranes;
    if (ships[0]->getType() == TypeOfCargo::Container) {
        count_cranes = count_container_cranes_;
    } else if (ships[0]->getType() == TypeOfCargo::Granular) {
        count_cranes = count_granular_cranes;
    } else {
        count_cranes = count_liquid_cranes_;
    }
    std::sort(ships.begin(), ships.end(), [](auto lhs, auto rhs) {
        return *lhs < *rhs;
    });
    auto comparator = [](std::pair<int64_t, int> lhs, std::pair<int64_t, int> rhs) {
        if (lhs.first + lhs.second != rhs.first + rhs.second) {
            return lhs.first + lhs.second < rhs.first + rhs.second;
        }
        return lhs.second < rhs.second;
    };
    std::set<std::pair<int64_t, int>, decltype(comparator)> cranes;
    for (int i = 0; i < count_cranes; ++i) {
        cranes.insert({0, i});
    }
    for (auto& ship : ships) {
        events_.emplace_back(ship->getArrival() - kDistArrivalQueue / kSpeedShip,
                             TypeOfEvent::ArrivalOnScreen,
                             ship);
        events_.emplace_back(ship->getArrival(),
                             TypeOfEvent::ArrivalAtPort,
                             ship);
        auto [time, id] = *cranes.begin();
        cranes.erase(cranes.begin());
        int64_t time_to_crane = kDistQueueFirst / kSpeedShip + id * kDistCranes / kSpeedShip;
        time = std::max(ship->getArrival(), time - time_to_crane);
        fee_ += std::max(0ll, time - ship->getArrival()) * kFee;
        total_waiting_time_ += std::max(0ll, time - ship->getArrival());
        events_.emplace_back(id, time,
                             TypeOfEvent::StartMovingToCrane,
                             ship);
        time += time_to_crane;
        time = std::max(time, ship->getArrival());
        events_.emplace_back(id, time,
                             TypeOfEvent::StartOfUnloading,
                             ship);
        time += ship->getUnloadTime();
        events_.emplace_back(id, time,
                             TypeOfEvent::FinishOfUnloading,
                             ship);
        cranes.insert({time, id});
        max_time_ = std::max(max_time_, time);
    }
}

void Manager::modeling() {
    std::vector<Ship*> container_ships, liquid_ships, granular_ships;
    for (auto& ship : ships_) {
        if (ship.getType() == TypeOfCargo::Container) {
            container_ships.push_back(&ship);
        } else if (ship.getType() == TypeOfCargo::Granular) {
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

void Manager::setShips(std::vector<Ship>& ships) {
    for (auto& ship : ships) {
        int64_t time;
        if (ship.getType() == TypeOfCargo::Container) {
            time = kSpeedOfContainerCrane * ship.getWeight();
        } else if (ship.getType() == TypeOfCargo::Granular) {
            time = kSpeedOfGranularCrane * ship.getWeight();
        } else {
            time = kSpeedOfLiquidCrane * ship.getWeight();
        }
        int64_t delay = GetRandomFromRange(delay_min_, delay_max_);
        int64_t late_arrival = GetRandomFromRange(late_arrival_min_,
                                                  late_arrival_max_);
        delay = std::max(-time + 1, delay);
        late_arrival = std::max(-ship.getArrival() + 1, late_arrival);
        time += delay;
        ship.setArrival(late_arrival + ship.getArrival());
        total_delay_ += delay;
        max_delay_ = std::max(max_delay_, delay);
        ship.setUnloadTime(time);
    }
    ships_ = ships;
}

Event Manager::getNext() {
    ++ptr_;
    if (ptr_ >= events_.size()) throw std::runtime_error("!!!!йухан идИ");
    return events_[ptr_];
}

double Manager::getAverageQueue() {
    int balance = 0;
    int64_t total_queue = 0;
    int cur_event = 0;
    for (int i = 1; i <= max_time_; ++i) {
        while (cur_event < events_.size() && events_[cur_event].getTime() == i) {
            if (events_[cur_event].getTypeOfEvent() == TypeOfEvent::ArrivalAtPort) {
                ++balance;
            } else if (events_[cur_event].getTypeOfEvent() == TypeOfEvent::StartMovingToCrane) {
                --balance;
            }
            ++cur_event;
        }
        total_queue += balance;
    }
    return total_queue / (1.0 * max_time_);
}

void Manager::setEventTime(int time) {
    int left = -1, right = static_cast<int>(events_.size()) - 1;
    while (right - left > 1) {
        int mid = (left + right) / 2;
        if (events_[mid].getTime() < time) {
            left = mid;
        } else {
            right = mid;
        }
    }
    if (events_[right].getTime() < time) {
        throw std::runtime_error("Слишком много букв, больше трёх не перевариваю...");
    }
    ptr_ = right - 1;
}