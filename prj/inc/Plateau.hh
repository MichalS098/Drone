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
public:
    // konstruktor tworzy płaskowyż
    Plateau(const Vector<3>& pos, const Vector<3>& scale, std::string refFile, std::string finalFile);
};