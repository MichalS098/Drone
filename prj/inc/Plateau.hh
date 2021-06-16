#pragma once
#include <string>
#include "GeometricFigure.hh"


/**
 * @brief Klasa Plateau definiuje Płaskowyż.
 * 
 * Klasa Plateau definiuje Płaskowyż, jako klasa pochodna klasy
 * GeometricFigure czyli figury geometrycznej.
 */
class Plateau: public GeometricFigure{
    Vector<3>    _position;
    unsigned int _ID;
    double       _radius;
public:
    // konstruktor tworzy płaskowyż
    Plateau(const Vector<3>& pos, const Vector<3>& scale, unsigned int id);
    unsigned int getID() const {return _ID;}
    virtual const char* getType() const override {return "Plaskowyz";}
    virtual Vector<3> getPosition() const override {return _position;}

    virtual double getRadius() const override {return _radius;}
    virtual bool checkIfPlaceIsAvaliable(const Vector<3> &center, double radius) const override final;
};

std::string makeFilePlateau(unsigned int id);
