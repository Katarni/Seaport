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


int Event::getIdCrane() const {
    return id_crane_;
}

Ship *Event::getShip() const {
    return ship_;
}

int64_t Event::getTime() const {
    return time_;
}

TypeOfEvent Event::getTypeOfEvent() const {
    return type_;
}

bool Event::operator<(const Event &other) const {
    if (other.time_ != time_) {
        return time_ < other.time_;
    }
    if (type_ != other.type_) {
        return type_ < other.type_;
    }
    return ship_ < other.ship_;
}
