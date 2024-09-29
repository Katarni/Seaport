#pragma once
#include <iostream>
#include "Ship.h"

enum class TypeOfEvent {
    ArrivalOfShip, StartOfUnloading, FinishOfUnloading
};

class Event {
public:
    Event();

    Event(int time, TypeOfEvent type, Ship* ship);

    Event(int id_crane, int time, TypeOfEvent type, Ship* ship);

    int GetIdCrane();

    int GetTime();

    TypeOfEvent GetTypeOfEvent();

    Ship* GetShip();

private:
    int id_crane_, time_;
    TypeOfEvent type_;
    Ship* ship_;
};

