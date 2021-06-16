#pragma once
#include "Vector.hh"
#include <string>

/**
 * @brief Klasa obiekt sceny jest klasą bazową dla 
 *        klas GeometricFigure, oraz Dron i pozwala na umieszczenie
 *        tych obiektów na jednej liście poprzez rzutowanie w góre.
 */
class SceneObject{
public:
    /**
    * @brief Funkcja wirtualna, sprawdza czy miejsce zajmowane przez obiekt sceny, pokrywa sie z 
    *        okręgiem o parametrach podanych jako argumenty (położenie i promień).
    * @retval true Jeśli miejsce jest wolne i okręgi się nie pokrywają.
    * @retval false Jeśli miejsce jest zajęte i okręgi się pokrywają.
    */
    virtual bool checkIfPlaceIsAvaliable(const Vector<3> &center, double radius) const = 0;
    /**
     * @brief Funkcja wirtualna pozwalająca na wypisanie położenia przez klasę pochodną.
     * @retval Vector<3> położenie
     */
    virtual Vector<3> getPosition() const {return Vector<3>({0.0,0.0,0.0});}

    /**
     * @brief Funkcja wirtualna pozwalająca na odczytanie 
     *        typu elementu klas pochodnych.
     * 
     * @retval const char* Nazwa typu obiektu.
     */
    virtual const char* getType() const {return "ObiektSceny";}

    /**
     * @brief Funkcja zwraca promień okręgu sceny.
     * 
     * @retval Zwracany jest promień okręgu zajmowanego przez element sceny
     */
    virtual double getRadius() const {return 0.0;}

    /**
     * @brief Funkcja wirtualna pozwalająca na zwrócenie nazwy pliku wzorcowego.
     * @retval const std::string& - zwracana nazwa pliku .dat z zapisanymi współrzędnymi
     */
    virtual const std::string takeFileName_refFig() const {return "Obiekt sceny ref filename";}

    /**
     * @brief Funkcja wirtualna pozwalająca na zwrócenie nazwy pliku finalnego.
     * @retval const std::string& - zwracana nazwa pliku .dat z zapisanymi współrzędnymi
     */
    virtual const std::string takeFileName_finalFig() const {return "obiekt sceny final filename";}
};