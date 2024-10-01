#pragma once
#include "Ship.h"

enum class TypeOfEvent {
    FinishOfUnloading, ArrivalOfShip, StartOfUnloading
};

class Event {
public:
    Event();

    Event(int64_t time, TypeOfEvent type, Ship* ship);

    Event(int id_crane, int64_t time, TypeOfEvent type, Ship* ship);

    int getIdCrane() const;

    int64_t getTime() const;

    TypeOfEvent getTypeOfEvent() const;

    Ship* getShip() const;

    bool operator<(const Event& other) const;

private:
    int id_crane_;
    int64_t time_;
    TypeOfEvent type_;
    Ship* ship_;
};

