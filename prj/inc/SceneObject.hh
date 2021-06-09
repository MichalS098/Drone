#pragma once
#include "Drone.hh"

/**
 * @brief Klasa obiekt sceny jest klasą bazową dla 
 *        klas GeometricFigure, oraz Dron i pozwala na umieszczenie
 *        tych obiektów na jednej liście poprzez rzutowanie w góre.
 */
class SceneObject{
public:
    virtual bool checkIfPlaceIsAvaliable(const Drone& dr) const;
};