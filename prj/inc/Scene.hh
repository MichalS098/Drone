#pragma once
#include <list>
#include <memory>
#include "Drone.hh"
#include "lacze_do_gnuplota.hh"
#include "GeometricFigure.hh"

/**
 * @brief Klasa definiująca scene zawierającą dwa drony.
 * 
 * Klasa definiująca scene zawierającą dwa drony, jej metody pozwalają 
 * na wybór aktywnego drona oraz na wywołanie animacji lotu drona wraz 
 * z rysowaniem planowanej ścieżki.
 */
class Scene{
    /**
     * @brief Lista dwukierunkowa przechowująca elementu sceny (przeszkody).
     */
    std::list<GeometricFigure*> _lstOfElements;
    /**
     * @brief tablica zawierajaca ilosc powstalych elementow sceny,
     * _numberOfElements[0] - ilosc plaskowyzy,
     * _numberOfElements[1] - ilosc gor z ostrym szczytem,
     * _numberOfElements[2] - ilosc gor z dluga grania.
     */
    unsigned int                _numberOfElements[3]; 
    Drone                       _droneArray[2];
    unsigned int                _ID_of_active_Drone;
    PzG::LaczeDoGNUPlota&       _lacze;
public:
    Scene(PzG::LaczeDoGNUPlota& lacze);
    const Drone& takeActiveDrone();
    Drone& useActiveDrone();
    void printPositionOfActiveDrone() const;
    void droneFlightAnimation();
    void makeNewElement();
    void deleteElement();
    void freeMemoryFromList();
};

