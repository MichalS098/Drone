#pragma once

#include "Vector.hh"
#include "GeometricFigure.hh"

/**
 * @brief Klasa pochodna klasy figury geometrycznej definiująca graniastosłup sześciokątny.
 * 
 * Klasa pochodna klasy figury geometrycznej definiująca
 * graniastosłup o podstawie sześciokąta. Określamy go na podstawie jego pozycji
 * kąta orientacji, oraz nazw plików wzorcowego i właściwego dziedziczonych z klasy "GeometricFigure".
 */
class HexagonalPrism: public GeometricFigure{
    Vector<3>   _position;
    double      _orientationAngle;
public:
    Vector<3> transfToParentCoordSys(const Vector<3>& apex) const;
};