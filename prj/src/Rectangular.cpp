#include <string>
#include "Rectangular.hh"

using namespace std;

/**
 * @brief Standardowy konstruktos prostopadłościanu.
 * 
 * Standardowy konstruktor prostopadłościanu, 
 * powinniśmy podać nazwy pliku wzorcowego i finalnego, gdzie w pliku wzorcowym .dat
 * powinnien znaleźć się odpowiednio zdefiniowany prostopadłościan w formie współrzednych jego wierzchołków.
 * 
 * @param refFileName - Nazwa pliku wzorcowego prostopadłościanu
 * @param finalFileName - Nazwa pliku finalnego prostopadłościanu
 */
Rectangular::Rectangular(string refFileName, string finalFileName)
: position{0,0,0}, orientationAngle{0}
{
    enterFileName_finalFig(finalFileName);
    enterFileName_finalFig(refFileName);
}

Vector<3> Rectangular::transfToParentCoordSys(const Vector<3>& apex) const{
    
}
