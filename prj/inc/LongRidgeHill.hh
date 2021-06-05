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
public:
    // funkcja tworzy góre z długą granią
    LongRidgeHill(const Vector<3>& pos, const Vector<3>& scale, unsigned int id);
    virtual const char* getType() const override {return "Gora z dluga grania";}
    virtual Vector<3> getPosition() const override {return _position;}
    unsigned int getID() const {return _ID;}
};

std::string makeFileLongRidgeHill(unsigned int id);