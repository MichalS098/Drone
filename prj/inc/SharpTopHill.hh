#pragma once
#include <string>
#include "GeometricFigure.hh"


/**
 * @brief Klasa SharpTopHill definiuje góre z ostrym szczytem.
 * 
 * Klasa definiuję góre z ostrym szczytem, jako klasa pochodna 
 * GeometricFigure czyli figury geometrycznej.
 */
class SharpTopHill: public GeometricFigure{
    Vector<3>    _position;
    unsigned int _ID;
public:
    // konstruktor tworzy góre z ostrym szczytem
    SharpTopHill(const Vector<3>& pos, const Vector<3>& scale, unsigned  int id);
    unsigned int getID() const {return _ID;}
    virtual const char* getType() const override {return "Gora z ostrym szczytem";}
    virtual Vector<3> getPosition() const override {return _position;}
};

std::string makeFileSharpTopHill(unsigned int id);