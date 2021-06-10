#include <iostream>
#include <string>
#include "GeometricFigure.hh"

using namespace std;

/**
 * @brief Funkcja przskalowuje wektor podany jako argument.
 * 
 * Funkcja przeskalowuje wektor podany jako argument mnożąc
 * poszczególne współrzedne wektora "apex" przez współrzędne wektora skali "scale".
 * 
 * @param apex - Wektor który będziemy skalować.
 * @return Vector<3> - zwracana jest przeskalowana wersja wektora "apex".
 */
Vector<3> GeometricFigure::scaleUp(const Vector<3>& apex) const {
    Vector<3> vec;
    for(int i=0; i<3; ++i){
        vec[i]=apex[i] * _scale[i];
    }
    return vec;
}

/**
 * @brief Funkcja ułatwiająca tworzenie nazw plików w których
 *        znajdować się będą wspołrzędne rotorów.
 * 
 * @param droneNR - Numer drona 
 * @param rotorNR - Numer rotora
 * @return std::string - zwracana jest nazwa pliku rotora poszczególnego drona
 */
string makeRotorFileName(unsigned int droneNR, unsigned int rotorNR){
    ostringstream nameStream;
    nameStream<<"dat/PlikWlasciwy_Dron"<<droneNR<<"_Rotor"<<rotorNR<<".dat";
    return nameStream.str();
}

/**
 * @brief Funkcja ułatwiająca tworzenie nazw plików w których
 *        znajdować się będą wspołrzędne korpusów dronów.
 * 
 * @param droneNR - Numer drona 
 * @return std::string - zwracana jest nazwa pliku korpusu poszczególnego drona
 */
string makeBodyFileName(unsigned int droneNR){
    ostringstream nameStream;
    nameStream<<"dat/PlikWlasciwy_Dron"<<droneNR<<"_Korpus.dat";
    return nameStream.str();
}


bool GeometricFigure::checkIfPlaceIsAvaliable() const{return 1;}