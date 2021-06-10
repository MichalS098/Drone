#pragma once
#include "SceneObject.hh"
#include "Vector.hh"

/**
 * @brief Klasa definiuje siatke przeszkody i metody z nią związane
 *        służące do przechowywania skali przeszkody oraz położenia przeszkody.
 *        Klasa wykorzystywana jest przy sprawdzaniu czy położenie lądowania jest wolne.
 */
class ObstacleGrid: public SceneObject{
    Vector<3> _ObstaclePosition;
    Vector<3> _ObstacleScale;
public:
    ObstacleGrid(const Vector<3>& pos, const Vector<3>& scale): _ObstaclePosition{pos}, _ObstacleScale{scale} {};
    virtual Vector<3> getPosition() const override {return _ObstaclePosition;}
    Vector<3> getScale() const {return _ObstacleScale;}
};