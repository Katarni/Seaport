#pragma once

#include "Ship.h"

enum class TypeOfEvent {
    FinishOfUnloading, ArrivalOnScreen, ArrivalAtPort, StartMovingToCrane, StartOfUnloading
};

class Event {
 public:
    Event();

    Event(int64_t time, TypeOfEvent type, Ship *ship);

    Event(int id_crane, int64_t time, TypeOfEvent type, Ship *ship);

    [[nodiscard]]
    int getIdCrane() const;

    [[nodiscard]]
    int64_t getTime() const;

    [[nodiscard]]
    TypeOfEvent getTypeOfEvent() const;

    [[nodiscard]]
    Ship *getShip() const;

    bool operator<(const Event &other) const;

 private:
    int id_crane_;
    int64_t time_;
    TypeOfEvent type_;
    Ship *ship_;
};

