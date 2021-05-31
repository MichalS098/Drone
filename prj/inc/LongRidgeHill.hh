#pragma once
#include <string>
#include <iostream>
#include "GeometricFigure.hh"

/**
 * @brief Klasa LongRidgeHill definiuje góre z długą granią.
 * 
 * Klasa LongRidgeHill definiuje góre z długą granią, jako klasa pochodna klasy
 * GeometricFigure czyli figura geometryczna.
 */
class LongRidgeHill: public GeometricFigure{
    Vector<3> _position;
public:
  
    // funkcja tworzy góre z długą granią
    LongRidgeHill(const Vector<3>& pos, const Vector<3>& scale, std::string refFile, std::string finalFile);
    inline void printPosition() const {std::cout<<"("<<_position<<")";}
};