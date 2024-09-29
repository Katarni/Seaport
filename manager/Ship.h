#pragma once
#include "includes.h"


enum class TypeOfCargo {
    Container, Liquid, Granular
};

class Ship {
public:
    Ship(TypeOfCargo type, const std::string& name, int weight, int time, int arrival);

    bool operator<(const Ship& other) const;

    int GetUnloadTime() const;

    int GetArrival() const;

    int GetWeight() const;

    std::string GetName() const;

    TypeOfCargo GetType() const;

private:
    TypeOfCargo type_;
    std::string name_;
    int weight_, unload_time_, arrival_;
};
