#pragma once
#include "Ship.h"

enum class TypeOfEvent {
    ArrivalOfShip, StartOfUnloading, FinishOfUnloading
};

class Event {
public:
    Event();

    Event(int64_t time, TypeOfEvent type, Ship* ship);

    Event(int id_crane, int64_t time, TypeOfEvent type, Ship* ship);

    int GetIdCrane() const;

    int64_t GetTime() const;

    TypeOfEvent GetTypeOfEvent() const;

    Ship* GetShip() const;

    bool operator<(const Event& other) const;

private:
    int id_crane_;
    int64_t time_;
    TypeOfEvent type_;
    Ship* ship_;
};

