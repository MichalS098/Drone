#pragma once
#include <string>
#include <iostream>
#include "GeometricFigure.hh"


/**
 * @brief Klasa SharpTopHill definiuje góre z ostrym szczytem.
 * 
 * Klasa definiuję góre z ostrym szczytem, jako klasa pochodna 
 * GeometricFigure czyli figury geometrycznej.
 */
class SharpTopHill: public GeometricFigure{
    Vector<3> _position;
public:

    // funkcja tworzy góre z ostrym szczytem
    SharpTopHill(const Vector<3>& pos, const Vector<3>& scale, std::string refFile, std::string finalFile);
    inline void printPosition() const {std::cout<<"("<<_position<<")";}
};