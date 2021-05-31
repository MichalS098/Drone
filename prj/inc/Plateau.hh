#pragma once
#include <string>
#include <iostream>
#include "GeometricFigure.hh"


/**
 * @brief Klasa Plateau definiuje Płaskowyż.
 * 
 * Klasa Plateau definiuje Płaskowyż, jako klasa pochodna klasy
 * GeometricFigure czyli figura geometryczna.
 */
class Plateau: public GeometricFigure{
    Vector<3> _position;
public:
    // konstruktor tworzy płaskowyż
    Plateau(const Vector<3>& pos, const Vector<3>& scale, std::string refFile, std::string finalFile);
    inline void printPosition() const {std::cout<<"("<<_position<<")";}
};