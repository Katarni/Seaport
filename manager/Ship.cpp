#include "Ship.h"

std::string generateName() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::string chars = "abcdefghijklmnopqrstuvwxyz1234567890";
    int sz = static_cast<int>(chars.size());
    std::string res;
    for (int i = 0; i < 6; ++i) res += chars[rng() % sz];
    return res;
}

Ship::Ship(TypeOfCargo type, const std::string& name,
    int64_t weight, int64_t time, int64_t arrival)
    : type_(type),
      name_(name),
      weight_(weight),
      unload_time_(time), arrival_(arrival) {
    if (name_.empty()) {
        name_ = generateName();
    }
}

bool Ship::operator<(const Ship& other) const {
    if (arrival_ != other.arrival_) {
        return arrival_ < other.arrival_;
    }
    if (unload_time_ != other.unload_time_) {
        return unload_time_ < other.unload_time_;
    }
    return name_ < other.name_;
}

std::string Ship::getName() const {
    return name_;
}

int64_t Ship::getUnloadTime() const {
    return unload_time_;
}

int64_t Ship::getArrival() const {
    return arrival_;
}

TypeOfCargo Ship::getType() const {
    return type_;
}

int64_t Ship::getWeight() const {
    return weight_;
}

void Ship::setArrival(int64_t arrival) {
    arrival_ = arrival;
}

void Ship::setUnloadTime(int64_t unload_time) {
    unload_time_ = unload_time;
}
