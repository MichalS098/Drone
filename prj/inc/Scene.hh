#pragma once
#include "Drone.hh"
#include "lacze_do_gnuplota.hh"

class Scene{
    Drone                 _droneArray[2];
    PzG::LaczeDoGNUPlota  _lacze;
public:
    const Drone& takeActiveDrone() const;
    Drone& useActiveDrone();
};