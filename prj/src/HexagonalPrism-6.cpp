#include <string>
#include "HexagonalPrism-6.hh"

using namespace std;

/**
 * @brief Standardowy konstruktos graniastoslupa prawidlowego szesciokatnego.
 * 
 * Standardowy konstruktor graniastosłupa prawidłowego sześciokątnego, 
 * powinniśmy podać nazwy pliku wzorcowego i finalnego, gdzie w pliku wzorcowym .dat
 * powinnien znaleźć się odpowiednio zdefiniowany graniastoslup o podstawie szesciokata foremnego
 * 
 * @param refFileName - Nazwa pliku wzorcowego graniastoslupa
 * @param finalFileName - Nazwa pliku finalnego graniastosłupa
 */
HexagonalPrism::HexagonalPrism(string refFileName, string finalFileName)
: position{0,0,0}, orientationAngle{0}
{
    enterFileName_finalFig(finalFileName);
    enterFileName_finalFig(refFileName);
}

Vector<3> HexagonalPrism::transfToParentCoordSys(const Vector<3>& apex) const{
    
}