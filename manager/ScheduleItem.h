#pragma once

#include "includes.h"

enum class TypeOfCargo {
    Container, Liquid, Granular
};


class ScheduleItem {
 public:
    ScheduleItem(TypeOfCargo type, const std::string &name, int64_t weight, int64_t arrival);

    [[nodiscard]]
    int64_t getUnloadTime() const;

    [[nodiscard]]
    int64_t getArrival() const;

    [[nodiscard]]
    int64_t getWeight() const;

    [[nodiscard]]
    std::string getName() const;

    [[nodiscard]]
    TypeOfCargo getType() const;

 protected:
    TypeOfCargo type_;
    std::string name_;
    int64_t weight_, unload_time_, arrival_;
};

