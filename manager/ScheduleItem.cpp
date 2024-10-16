#include "ScheduleItem.h"

std::string generateName() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::string chars = "abcdefghijklmnopqrstuvwxyz1234567890";
    int sz = static_cast<int>(chars.size());
    std::string res;
    for (int i = 0; i < 6; ++i) res += chars[rng() % sz];
    return res;
}

ScheduleItem::ScheduleItem(TypeOfCargo type, const std::string &name,
                           int64_t weight, int64_t arrival)
        : type_(type),
          name_(name),
          weight_(weight), arrival_(arrival) {
    if (type_ == TypeOfCargo::Container) {
        unload_time_ = kSpeedOfContainerCrane * weight_;
    } else if (type_ == TypeOfCargo::Granular) {
        unload_time_ = kSpeedOfGranularCrane * weight_;
    } else {
        unload_time_ = kSpeedOfLiquidCrane * weight_;
    }
    if (name_.empty()) name_ = generateName();
}

int64_t ScheduleItem::getUnloadTime() const {
    return unload_time_;
}

int64_t ScheduleItem::getArrival() const {
    return arrival_;
}

int64_t ScheduleItem::getWeight() const {
    return weight_;
}

std::string ScheduleItem::getName() const {
    return name_;
}

TypeOfCargo ScheduleItem::getType() const {
    return type_;
}
