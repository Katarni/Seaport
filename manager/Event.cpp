#include "Event.h"

Event::Event() {}

Event::Event(int64_t time,
             TypeOfEvent type,
             Ship *ship) : id_crane_(-1),
                           time_(time),
                           type_(type),
                           ship_(ship) {}

Event::Event(int id_crane,
             int64_t time,
             TypeOfEvent type,
             Ship *ship) : id_crane_(id_crane),
                           time_(time),
                           type_(type),
                           ship_(ship) {}


int Event::GetIdCrane() const {
    return id_crane_;
}

Ship* Event::GetShip() const {
    return ship_;
}

int64_t Event::GetTime() const {
    return time_;
}

TypeOfEvent Event::GetTypeOfEvent() const {
    return type_;
}

bool Event::operator<(const Event &other) const {
    if (other.time_ != time_) {
        return time_ < other.time_;
    }
    if (type_ != other.type_) {
        if (type_ == TypeOfEvent::FinishOfUnloading) return true;
        if (other.type_ == TypeOfEvent::FinishOfUnloading) return false;
        if (type_ == TypeOfEvent::ArrivalOfShip) return true;
        return false;
    }
    return ship_ < other.ship_;
}
