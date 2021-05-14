#pragma once

#include <string>
#include "Vector.hh"

/**
 * @brief Klasa Bazowa opisujaca figure geometryczna
 * 
 * Klasa bazowa opisujaca figure geometryczna 
 * przechowuja nazwe pliku z wzorcowa figura oraz nazwe pliku z finalna figura         
 */
class GeometricFigure{
private:
    std::string _referenceFig_FileName;
    std::string _finalFig_fileName;
    Vector<3>   _scale;
public:
    /**
     * @brief Funkcja zwraca zapisaną nazwe pliku wzorcowego obiektu.
     * @return const std::string& - zwracana nazwa pliku .dat z zapisanymi współrzędnymi
     */
    const std::string& takeFileName_refFig() const {return _referenceFig_FileName;}
    /**
     * @brief Funkcja zwraca zapisaną nazwe pliku finalnego/właściwego obiektu.
     * @return const std::string& - zwracana nazwa pliku .dat z zapisanymi współrzędnymi
     */
    const std::string& takeFileName_finalFig() const {return _finalFig_fileName;}

    /**
     * @brief Funkcje pozwalające na dostęp do zapisu nazwy pliku wzorcowego i 
     *        pliku finalnego/właściwego figury geometrycznej.
     * @param fileName - nazwa pliku do zapisu.
     */
    void enterFileName_refFig(std::string fileName) {_referenceFig_FileName=fileName;}
    void enterFileName_finalFig(std::string fileName) {_finalFig_fileName=fileName;}
    
    Vector<3> scaleUp(const Vector<3>& apex) const ;
};


std::string makeRotorFileName(unsigned int droneNR, unsigned int rotorNR);
std::string makeBodyFileName(unsigned int droneNR);
