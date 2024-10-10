#include "Ship.h"



Ship::Ship(TypeOfCargo type, const std::string& name,
    int64_t weight, int64_t arrival)
    : ScheduleItem(type, name, weight, arrival) {}

bool Ship::operator<(const Ship& other) const {
    if (arrival_ != other.arrival_) {
        return arrival_ < other.arrival_;
    }
    if (unload_time_ != other.unload_time_) {
        return unload_time_ < other.unload_time_;
    }
    return name_ < other.name_;
}

void Ship::setArrival(int64_t arrival) {
    arrival_ = arrival;
}

void Ship::setUnloadTime(int64_t unload_time) {
    unload_time_ = unload_time;
}

int64_t Ship::getFee() const {
    return fee_;
}

Ship::Ship(const ScheduleItem& item) : ScheduleItem(item), fee_(0) {}

void Ship::addToFee(int64_t delta) {
    fee_ += delta;
}
