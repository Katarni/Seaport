#pragma once
#include "includes.h"


enum class TypeOfCargo {
    Container, Liquid, Granular
};

class Ship {
public:
    Ship(TypeOfCargo type, const std::string& name, int64_t weight, int64_t time, int64_t arrival);

    bool operator<(const Ship& other) const;

    int64_t GetUnloadTime() const;

    int64_t GetArrival() const;

    int64_t GetWeight() const;

    std::string GetName() const;

    TypeOfCargo GetType() const;

    void SetArrival(int64_t arrival);

    void SetUnloadTime(int64_t unload_time);

private:
    TypeOfCargo type_;
    std::string name_;
    int64_t weight_, unload_time_, arrival_;
};
