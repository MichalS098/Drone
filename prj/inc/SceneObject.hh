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
     * @brief Funkcja czysto wirtualna definiująca sprawdzanie czy miejsce lądowania jest puste.
     * @return true Zwraca jeśli miejsce jest puste
     * @return false Zwraca jeśli miejsce jest zajete
     */
    virtual bool checkIfPlaceIsAvaliable() const = 0;
    /**
     * @brief Funkcja wirtualna pozwalająca na wypisanie położenia przez klasę pochodną.
     */
    virtual Vector<3> getPosition() const {return Vector<3>({0.0,0.0,0.0});}

    /**
     * @brief Funkcja wirtualna pozwalająca na odczytanie 
     *        typu elementu klas pochodnych.
     * 
     * @return const char* Nazwa typu obiektu.
     */
    virtual const char* getType() const {return "ObiektSceny";}

    /**
     * @brief Funkcja wirtualna pozwalająca na zwrócenie nazwy pliku wzorcowego.
     * @return const std::string& - zwracana nazwa pliku .dat z zapisanymi współrzędnymi
     */
    virtual const std::string takeFileName_refFig() const {return "Obiekt sceny ref filename";}

    /**
     * @brief Funkcja wirtualna pozwalająca na zwrócenie nazwy pliku finalnego.
     * @return const std::string& - zwracana nazwa pliku .dat z zapisanymi współrzędnymi
     */
    virtual const std::string takeFileName_finalFig() const {return "obiekt sceny final filename";}
};