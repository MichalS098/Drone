#pragma once
#include "Vector.hh"
#include "GeometricFigure.hh"

class HexagonalPrism: public GeometricFigure{
private:
    Vector<3> position;
    double orientationAngle;
public:
    //HexagonalPrism(std::string refFileName, std::string finalFileName);
    Vector<3> transfToParentCoordSys(const Vector<3>& apex) const;
};