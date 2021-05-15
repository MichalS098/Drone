#pragma once

#include <vector>
#include "Vector.hh"
#include "Rectangular.hh"
#include "HexagonalPrism-6.hh"
#include "lacze_do_gnuplota.hh"

/**
 * @brief Klasa definiująca drona.
 * 
 * Klasa opisująca drona na podstawie jego korpusu będącego obiektem typu
 * prostopadłościan, jego 4 rotorów będących graniastosłupami foremnymi o podstawie sześciokąta, oraz
 * kąta orientacji, oraz wektora położenia.
 */
class Drone{
    Vector<3>       _position;
    double          _orientationAngle;
    Rectangular     _droneBody;
    HexagonalPrism  _droneRotor[4];
    unsigned int    _ID;

    bool calcAndSave_BodyCoords() const;
    bool calcAndSave_RotorCoords(const HexagonalPrism& rotor) const;
protected:
    Vector<3> transfToParentCoordSys(const Vector<3>& apex) const;
public:
    void makeDrone(const Vector<3>& position, const Vector<3>& scale, unsigned int ID, PzG::LaczeDoGNUPlota& Lacze);
    void planInitialFlightPath(double flightHeight, double turnAngle, double flightLenght, PzG::LaczeDoGNUPlota& lacze);
    void deleteFlightPath(PzG::LaczeDoGNUPlota& lacze) const;
    bool makeVerticalFlight(double height, PzG::LaczeDoGNUPlota& Lacze);
    bool makeHorizontalFlight(double flightLenght, PzG::LaczeDoGNUPlota& Lacze);
    bool changeDroneOrientation(double angle, PzG::LaczeDoGNUPlota& Lacze);
    bool calcAndSave_DroneCoords() const;
};