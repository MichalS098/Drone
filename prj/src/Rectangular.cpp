#include <string>
#include "Matrix.hh"
#include "Rectangular.hh"

using namespace std;

/**
 * @brief Funkcja przekształca zadany wierzchołek do układu współrzędnych rodzica.
 * 
 * @param apex - wektor do transformacji.
 * @return Vector<3> - zwracany jest przetransformowany wektor.
 */
Vector<3> Rectangular::transfToParentCoordSys(const Vector<3>& apex) const{
    Matrix<3> rotationMatrix;
    makeRotationMatrix('z', _orientationAngle, rotationMatrix);
    Vector<3> apexAfterTransf = rotationMatrix * apex;
    return (apexAfterTransf + _position);
}
