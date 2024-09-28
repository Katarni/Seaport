#pragma once
#include <iostream>
#include "Ship.h"

enum class TypeOfEvent {
    ArrivalOfShip, StartOfUnloading, FinishOfUnloading
};

class Event {
public:
    int GetIdCrane();

    int GetTime();

    TypeOfEvent GetTypeOfEvent();

    Ship* GetShip();

private:
    int id_crane_, time_;
    TypeOfEvent type_;
    Ship* ship_;
};

