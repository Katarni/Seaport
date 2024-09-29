#include "Event.h"

Event::Event() {}

Event::Event(int time,
             TypeOfEvent type,
             Ship *ship) : id_crane_(-1),
                           time_(time),
                           type_(type),
                           ship_(ship) {}

Event::Event(int id_crane,
             int time,
             TypeOfEvent type,
             Ship *ship) : id_crane_(id_crane),
                           time_(time),
                           type_(type),
                           ship_(ship) {}


int Event::GetIdCrane() {
    return id_crane_;
}

Ship* Event::GetShip() {
    return ship_;
}

int Event::GetTime() {
    return time_;
}

TypeOfEvent Event::GetTypeOfEvent() {
    return type_;
}