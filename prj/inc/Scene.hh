#pragma once
#include "Drone.hh"
#include "lacze_do_gnuplota.hh"

class Scene{
private:
    Drone droneArray[2];
    PzG::LaczeDoGNUPlota lacze;
public:
    const Drone& takeActiveDrone() const;
    Drone& useActiveDrone();
};