#pragma once
#include "includes.h"

enum class TypeOfCargo {
    Container, Liquid, Granular
};


class ScheduleItem {
public:
    ScheduleItem(TypeOfCargo type, const std::string& name, int64_t weight, int64_t arrival);

    int64_t getUnloadTime() const;

    int64_t getArrival() const;

    int64_t getWeight() const;

    std::string getName() const;

    TypeOfCargo getType() const;

protected:
    TypeOfCargo type_;
    std::string name_;
    int64_t weight_, unload_time_, arrival_;
};

