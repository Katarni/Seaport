#pragma once
#include "includes.h"


enum class TypeOfCargo {
    Container, Liquid, Granular
};

class Ship {
public:
    Ship(TypeOfCargo type, std::string name, int weight, int time, int arrival);

    bool operator<(const Ship& other) const;

    int GetTime();

    int GetArrival();

    std::string GetName();

    TypeOfCargo GetType();

private:
    TypeOfCargo type_;
    std::string name_;
    int weight_, time_, arrival_;
};
