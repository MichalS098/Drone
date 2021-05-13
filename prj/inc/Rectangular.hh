#pragma once

#include "Vector.hh"
#include "GeometricFigure.hh"

/**
 * @brief Klasa opisuje prostopadloscian
 * 
 * 		  Klasa opisuje prostopadłościan przy pomocy 8 wektorow wodzacych
 * 		  oraz kata orientacji
 */
class Rectangular: public GeometricFigure{
	Vector<3> position;		
    double orientationAngle;
public:
	Rectangular(std::string refFileName, std::string finalFileName);
	Vector<3> transfToParentCoordSys(const Vector<3>& apex) const;
};

