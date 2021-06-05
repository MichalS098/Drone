#pragma once
#include <string>
#include "GeometricFigure.hh"


/**
 * @brief Klasa Plateau definiuje Płaskowyż.
 * 
 * Klasa Plateau definiuje Płaskowyż, jako klasa pochodna klasy
 * GeometricFigure czyli figura geometryczna.
 */
class Plateau: public GeometricFigure{
    Vector<3>    _position;
    unsigned int _ID;
public:
    // konstruktor tworzy płaskowyż
    Plateau(const Vector<3>& pos, const Vector<3>& scale, unsigned int id);
    virtual const char* getType() const override {return "Plaskowyz";}
    virtual Vector<3> getPosition() const override {return _position;}
    unsigned int getID() const {return _ID;}
};

std::string makeFilePlateau(unsigned int id);
