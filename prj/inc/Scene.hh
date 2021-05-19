#pragma once
#include "Drone.hh"
#include "lacze_do_gnuplota.hh"

/**
 * @brief Klasa definiująca scene zawierającą dwa drony.
 * 
 * Klasa definiująca scene zawierającą dwa drony, jej metody pozwalają 
 * na wybór aktywnego drona oraz na wywołanie animacji lotu drona wraz 
 * z rysowaniem planowanej ścieżki.
 */
class Scene{
    Drone                 _droneArray[2];
    unsigned int          _ID_of_active_Drone;
    PzG::LaczeDoGNUPlota& _lacze;
public:
    Scene(PzG::LaczeDoGNUPlota& lacze);
    const Drone& takeActiveDrone();
    Drone& useActiveDrone();
    void printPositionOfActiveDrone();
    void droneFlightAnimation();
};

