#pragma once

#include <vector>
#include "Vector.hh"
#include "Rectangular.hh"
#include "HexagonalPrism-6.hh"
#include "lacze_do_gnuplota.hh"
#include "SceneObject.hh"

/**
 * @brief Klasa definiująca drona.
 * 
 * Klasa opisująca drona na podstawie jego korpusu będącego obiektem typu
 * prostopadłościan, jego 4 rotorów będących graniastosłupami foremnymi o podstawie sześciokąta, oraz
 * kąta orientacji, oraz wektora położenia.
 */
class Drone: public SceneObject{
    Vector<3>       _position;
    double          _orientationAngle;
    Rectangular     _droneBody;
    HexagonalPrism  _droneRotor[4];
    unsigned int    _ID;

    bool calcAndSave_BodyCoords() const;
    bool calcAndSave_RotorCoords(const HexagonalPrism& rotor) const;
    void rotateRotor(double angle);
protected:
    Vector<3> transfToParentCoordSys(const Vector<3>& apex) const;
    bool makeFlight(const Vector<3>& direction, double flightLenght, PzG::LaczeDoGNUPlota& lacze);
public:
    Drone(const Vector<3>& position, const Vector<3>& scale,  
          unsigned int ID, PzG::LaczeDoGNUPlota& Lacze, int droneColor);
    void planInitialFlightPath(double flightHeight, double turnAngle,  
                               double flightLenght, PzG::LaczeDoGNUPlota& lacze);
    void deleteFlightPath(PzG::LaczeDoGNUPlota& lacze) const;

    bool makeVerticalFlight(double height, PzG::LaczeDoGNUPlota& Lacze);
    bool makeHorizontalFlight(double flightLenght, PzG::LaczeDoGNUPlota& Lacze);
    bool changeDroneOrientation(double angle, PzG::LaczeDoGNUPlota& Lacze);
    bool tiltForward(PzG::LaczeDoGNUPlota& Lacze);

    bool calcAndSave_DroneCoords() const;
    Vector<3> takeDronePosition() const {return _position;}

    virtual bool checkIfPlaceIsAvaliable(const SceneObject& obj) const override;
    virtual Vector<3> getPosition() const override{return _position;}
    virtual const char* getType() const override{return "Dron";}
};

#define COMPUTATION_ERR 1e-8
constexpr double singleStepLenght=1;