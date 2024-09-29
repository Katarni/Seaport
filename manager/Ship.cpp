#include "Ship.h"

#include <utility>

std::string GenName() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::string chars = "qwertyuiopasdfghjklzxcvbnm1234567890";
    int sz = static_cast<int>(chars.size());
    std::string res;
    for (int i = 0; i < 6; ++i) res += chars[rng() % sz];
    return res;
}

Ship::Ship(TypeOfCargo type, const std::string& name,
    int weight, int time, int arrival)
    : type_(type),
      name_(name),
      weight_(weight),
      time_(time), arrival_(arrival) {
    if (name_.empty()) name_ = GetName();
}

bool Ship::operator<(const Ship& other) const {
    if (arrival_ != other.arrival_) {
        return arrival_ < other.arrival_;
    }
    if (time_ != other.time_) {
        return time_ < other.time_;
    }
    return name_ < other.name_;
}

std::string Ship::GetName() const {
    return name_;
}

int Ship::GetTime() const {
    return time_;
}

int Ship::GetArrival() const {
    return arrival_;
}

TypeOfCargo Ship::GetType() const {
    return type_;
}

int Ship::GetWeight() const {
    return weight_;
}
