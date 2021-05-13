#pragma once
#include <string>
#include "Vector.hh"

/**
 * @brief Klasa Bazowa opisujaca figure geometryczna
 * 
 *        Klasa bazowa opisujaca figure geometryczna 
 *        przechowuja nazwe pliku z wzorcowa figura oraz nazwe pliku z finalna figura         
 */
class GeometricFigure{
private:
    std::string referenceFig_FileName;
    std::string finalFig_fileName;
    Vector<3> scale;
public:
    /**
     * @brief Funkcje zwracaja zapisane nazwy plikow
     *        referencujnego obiektu oraz finalnego
     * @return const std::string& nazwa pliku przechowujacego figure geometryczna
     */
    const std::string& takeFileName_refFig() const {return referenceFig_FileName;}
    const std::string& takeFileName_finalFig() const {return finalFig_fileName;}

    /**
     * @brief Funkcje pozwalajace na zapis do pliku przechowujacego figure geometryczna
     * 
     * @param fileName nazwa pliku do zapisu
     */
    void enterFileName_refFig(std::string fileName) {referenceFig_FileName=fileName;}
    void enterFileName_finalFig(std::string fileName) {finalFig_fileName=fileName;}
    
    Vector<3> scaleUp(const Vector<3>& apex) const ;
};

