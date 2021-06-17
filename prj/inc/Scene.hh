#pragma once
#include <list>
#include <memory>
#include "Drone.hh"
#include "lacze_do_gnuplota.hh"
#include "GeometricFigure.hh"
#include "SceneObject.hh"

/**
 * @brief Klasa definiująca scene zawierającą dwa drony.
 * 
 * Klasa definiująca scene zawierającą dwa drony, jej metody pozwalają 
 * na wybór aktywnego drona oraz na wywołanie animacji lotu drona wraz 
 * z rysowaniem planowanej ścieżki.
 */
class Scene{
    /**
     * @brief Lista obiektów sceny (zawiera przeszkody oraz drony).
     */
    std::list<std::shared_ptr<SceneObject>> _lstOfObjects;
    /**
     * @brief Lista zawierająca drony.
     */
    std::list<std::shared_ptr<Drone>>       _lstOfDrones;
    /**
     * @brief tablica zawierajaca ilosc powstalych elementow sceny,            
     * _numberOfElements[0] - ilosc plaskowyzy,                  
     * _numberOfElements[1] - ilosc gor z ostrym szczytem,                   
     * _numberOfElements[2] - ilosc gor z dluga grania.                
     */
    unsigned int                _numberOfElements[3]; 
    unsigned int                _ID_of_active_Drone;
    PzG::LaczeDoGNUPlota&       _lacze;
public:
    Scene(PzG::LaczeDoGNUPlota& lacze);
    const Drone& takeActiveDrone();
    Drone& useActiveDrone();
    std::shared_ptr<Drone> takePointerToActiveDrone();
    void printPositionOfActiveDrone();
    void droneFlightAnimation();

    void makeNewElement(const Vector<3>& pos, const Vector<3> scale, unsigned int type);
    void deleteElement();
    /**
     * @brief Zwraca pierwszy element z listy Obiektow sceny.
     * @return const std::shared_ptr<SceneObject>& Pierwszy element na liście.
     */
    inline const std::shared_ptr<SceneObject>& takeFirstElemFromList() {return *_lstOfObjects.begin();}
    /**
     * @brief Zwraca ostatni element z listy Obiektow sceny.
     * @return const std::shared_ptr<SceneObject>& Ostatni element na liście.
     */
    inline const std::shared_ptr<SceneObject>& takeLastElemFromList() {return *_lstOfObjects.end();}
    /**
     * @brief Funkcja dodaje element typu figura geometryczna na liste.
     * @param[in] elem inteligentny wskaźnik na typ Obiekt sceny.
     */
    inline void addElementToList(std::shared_ptr<SceneObject>& elem) {_lstOfObjects.push_back(elem);}

    int checkIfPlaceIsOccupied(const std::shared_ptr<Drone> &P_drone) const;
};