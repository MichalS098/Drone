#pragma once
#include "Vector.hh"
#include "SceneObject.hh"

/**
 * @brief Klasa definiuje siatkę drona jako okrąg o podanym promieniu w środku którego 
 *        znajduję się dron. Klasa wykorzystywana jest przy sprawdzaniu czy położenie 
 *        lądowania jest wolne.
 */
class DroneGrid: public SceneObject{
    Vector<3> _DronePosition;
    double    _DroneRadius;
public:
    DroneGrid(const Vector<3>& pos, double radius): _DronePosition{pos}, _DroneRadius{radius} {};
    virtual Vector<3> getPosition() const override {return _DronePosition;}
    double getRadius() const {return _DroneRadius;}
};