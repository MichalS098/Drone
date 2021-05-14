#pragma once

#include "Vector.hh"
#include "GeometricFigure.hh"

/**
 * @brief Klasa pochodna klasy figury geometrycznej definiująca prostopadłościan.
 * 
 * Klasa pochodna klasy figury geometrycznej definiująca
 * prostopadłościan. Określamy go na podstawie jego pozycji
 * kąta orientacji, oraz nazw plików wzorcowego i właściwego 
 * dziedziczonych z klasy "GeometricFigure".
 */
class Rectangular: public GeometricFigure{
	Vector<3> 	_position;		
    double 		_orientationAngle;
public:
	Vector<3> transfToParentCoordSys(const Vector<3>& apex) const;
};

