#pragma once
#include "Vector.hh"
#include "Rectangular.hh"
#include "HexagonalPrism-6.hh"
#include "lacze_do_gnuplota.hh"
#include <vector>

class Drone{
private:
    Vector<3> position;
    double orientationAngle;
    Rectangular droneBody;
    HexagonalPrism droneRotor[4];

    bool calcAndSave_BodyCoords() const;
    bool calcAndSave_RotorCoords(const HexagonalPrism& rotor) const;

protected:
    Vector<3> transfToParentCoordSys(const Vector<3>& apex) const;

public:
    Drone();
    
    void planInitialFlightPath(double turnAngle, double flightLenght, double height,
                               std::vector<Vector<3>>& pathPoints, PzG::LaczeDoGNUPlota& Lacze);
    bool makeVerticalFlight(double height, PzG::LaczeDoGNUPlota& Lacze);
    bool makeHorizontalFlight(double flightLenght, PzG::LaczeDoGNUPlota& Lacze);
    bool calcAndSave_DroneCoords() const;
};