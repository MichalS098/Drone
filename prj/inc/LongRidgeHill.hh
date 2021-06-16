#pragma once
#include <string>
#include "GeometricFigure.hh"

/**
 * @brief Klasa LongRidgeHill definiuje góre z długą granią.
 * 
 * Klasa LongRidgeHill definiuje góre z długą granią, jako klasa pochodna klasy
 * GeometricFigure czyli figura geometryczna.
 */
class LongRidgeHill: public GeometricFigure{
    Vector<3>    _position;
    unsigned int _ID;
    double       _radius;
public:
    // konstruktor tworzy góre z długą granią
    LongRidgeHill(const Vector<3>& pos, const Vector<3>& scale, unsigned int id);
    unsigned int getID() const {return _ID;}
    virtual const char* getType() const override {return "Gora z dluga grania";}
    virtual Vector<3> getPosition() const override {return _position;}

    virtual double getRadius() const override {return _radius;}
    virtual bool checkIfPlaceIsAvaliable(const Vector<3> &center, double radius) const override final;
};

std::string makeFileLongRidgeHill(unsigned int id);