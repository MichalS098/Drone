#include "Drone.hh"
#include "GeometricFigure.hh"
#include "Matrix.hh"
#include <fstream>
#include <iostream>
#include <assert.h>
#include <unistd.h>
#include <string>

using namespace std;

// REF stands for reference (plik wzorcowy)
#define REF_ROTOR_FILE_NAME "bryly_wzorcowe/graniastoslup6.dat"
#define REF_BODY_FILE_NAME "bryly_wzorcowe/szescian.dat"
#define FLIGHT_PATH_FILE_NAME "dat/trasa_przelotu.dat"

/**
 * @brief Funkcja inicjalizuje Drona.
 * 
 * 
 * Funkcja inicjalizuje drona poprzez, przypisanie jego atrybutom 
 * (prostopadłościanowi oraz rotorom) ich nazwy plików właściwych oraz wzorcowych.
 * Funkcja dodaje odrazu nazwy tych plików do łącza do GNU-PLOT'a
 * @param ID    - id Drona
 * @param Lacze - Lacze do gnuplota 
 */
void Drone::makeDrone(const Vector<3>& position, const Vector<3>& scale, unsigned int ID, PzG::LaczeDoGNUPlota& Lacze){
    _ID=ID;
    _position=position;
    unsigned int index=0;
    string fileName;

    for(HexagonalPrism& rotor: _droneRotor){
        fileName=makeRotorFileName(ID, ++index);
        rotor.enterFileName_finalFig(fileName);
        rotor.enterFileName_refFig(REF_ROTOR_FILE_NAME);
        Lacze.DodajNazwePliku(fileName.c_str());
        rotor.setScale(scale);
    }
    fileName=makeBodyFileName(ID);
    _droneBody.enterFileName_finalFig(fileName);
    _droneBody.enterFileName_refFig(REF_BODY_FILE_NAME);
    _droneBody.setScale(scale);
}


/**
 * @brief Funkcja oblicza i zapisuje współrzędne globalne korpusu do plików. 
 * 
 * 
 * @return true  - Operacja udana
 * @return false - Błąd operacji
 */
bool Drone::calcAndSave_BodyCoords() const{
    ifstream referenceFigStream{_droneBody.takeFileName_refFig()};
    ofstream finalFigStream{_droneBody.takeFileName_finalFig()};
    if (!referenceFigStream.is_open()) {
        cerr << endl
	    << " Blad otwarcia do odczytu pliku: " << _droneBody.takeFileName_refFig() << endl
	    << endl;
        return false;
    }
    if (!finalFigStream.is_open()) {
        cerr << endl
        << " Blad otwarcia do odczytu pliku: " << _droneBody.takeFileName_finalFig() << endl
	    << endl;
        return false;
    }
    Vector<3> apex{};
    referenceFigStream >> apex;
    while (!referenceFigStream.fail()) {
        for (int apexCounter = 0; apexCounter < 4; ++apexCounter) {
            apex = _droneBody.scaleUp(apex);
            apex = _droneBody.transfToParentCoordSys(apex);
            apex = transfToParentCoordSys(apex);
            finalFigStream     << apex << endl;
            referenceFigStream >> apex;
            assert(apexCounter == 3 || !referenceFigStream.fail());
        }
        finalFigStream << endl;
    }
    return !finalFigStream.fail();
}


/**
 * @brief Funkcja oblicza i zapisuje współrzędne globalne rotora do plików. 
 * 
 * 
 * @param rotor  - jeden z 4 rotorów drona, którego współrzędne będą przeliczane
 * @return true  - Operacja udana
 * @return false - Błąd operacji
 */
bool Drone::calcAndSave_RotorCoords(const HexagonalPrism& rotor) const{
    ifstream referenceFigStream{rotor.takeFileName_refFig()};
    ofstream finalFigStream{rotor.takeFileName_finalFig()};
    if (!referenceFigStream.is_open()) {
        cerr << endl
	    << " Blad otwarcia do odczytu pliku: " << rotor.takeFileName_refFig() << endl
	    << endl;
        return false;
    }
    if (!finalFigStream.is_open()) {
        cerr << endl
        << " Blad otwarcia do odczytu pliku: " << rotor.takeFileName_finalFig() << endl
	    << endl;
        return false;
    }
    Vector<3> apex;
    referenceFigStream >> apex;
    while (!referenceFigStream.fail()) {
        for (int apexSize = 0; apexSize < 4; ++apexSize) {
            apex = rotor.scaleUp(apex);
            apex = rotor.transfToParentCoordSys(apex);
            apex = transfToParentCoordSys(apex);
            finalFigStream     << apex << endl;
            referenceFigStream >> apex;
            assert(apexSize == 3 || !referenceFigStream.fail());
        }
        finalFigStream << endl;
    }
    return !finalFigStream.fail();
}


/**
 * @brief Funkcja oblicza i zapisuje współrzędne globalne drona do plików. 
 * 
 * 
 * @return true  - Operacja udana
 * @return false - Błąd operacji
 */
bool Drone::calcAndSave_DroneCoords() const{
    if(!calcAndSave_BodyCoords()) return false;
    for(int i=0; i<4; ++i){
        if(!calcAndSave_RotorCoords(_droneRotor[i])) return false;
    }
    return true;
}


/**
 * @brief Funkcja przekształca zadany wierzchołek do układu współrzędnych rodzica. 
 * 
 * 
 * @param apex       - Wierzchołek do przekształcenia 
 * @return Vector<3> - Przekształcony wierzchołek
 */
Vector<3> Drone::transfToParentCoordSys(const Vector<3>& apex) const{
    Matrix<3> rotationMatrix;
    makeRotationMatrix('z', _orientationAngle, rotationMatrix);
    Vector<3> apexAfterTransf = rotationMatrix * apex;
    return (apexAfterTransf + _position);
}


/**
 * @brief Funkcja planuje początkową ścieżkę lotu drona.
 *
 * 
 * Funkcja tworzy ścieżkę lotu drona dla zadanego kąta i długości
 * lotu i zapisuje współrzędne jej punktów w wektorze PunktySciezki
 * 
 * @param turnAngle    - Kąt obrotu w stopniach
 * @param flightLenght - Długość ścieżki
 */
void Drone::planInitialFlightPath(double flightHeight, double turnAngle, double flightLenght){
    Vector<3> vec = _position;
    Matrix<3> rotationMatrix;
    makeRotationMatrix('z', turnAngle, rotationMatrix);
 
    vector<Vector<3>> pathPoints{};
    pathPoints.push_back(transfToParentCoordSys(rotationMatrix * vec));    /* poczatek */
    vec[2] += flightHeight;
    pathPoints.push_back(transfToParentCoordSys(rotationMatrix * vec));    /* po uniesieniu */
    vec[0]+=flightLenght;
    pathPoints.push_back(transfToParentCoordSys(rotationMatrix * vec));    /* po locie poziomym */
    vec[2]-=flightHeight;
    pathPoints.push_back(transfToParentCoordSys(rotationMatrix * vec));    /* po ladowaniu */

    ofstream fileNameStr(FLIGHT_PATH_FILE_NAME);
    if (!fileNameStr.is_open()){
        cerr << endl
	    << " Blad otwarcia pliku: " << FLIGHT_PATH_FILE_NAME << endl
	    << endl;
    }   
    for(const Vector<3> vector3d : pathPoints){
        fileNameStr << vector3d << endl;
    }
    fileNameStr << endl;
}


/**
 * @brief Funkcja usuwa powstałą wcześniej ścieżkę lotu drona
 */
void Drone::deleteFlightPath(PzG::LaczeDoGNUPlota& lacze) const{
    ofstream fileNameStr(FLIGHT_PATH_FILE_NAME);
    if (!fileNameStr.is_open()){
        cerr << endl
	    << " Blad otwarcia pliku: " << FLIGHT_PATH_FILE_NAME << endl
	    << endl;
    }
    fileNameStr << " ";
  	lacze.UsunNazwePliku(FLIGHT_PATH_FILE_NAME);
}


/**
 * @brief Funckja przemieszcza dron do przodu.
 * 
 * 
 * @param flightLenght - długość na jaką ma polecieć dron
 * @param Lacze        - łącze do gnuplota
 * @return true        - zwraca kiedy operacja się powiedzie
 * @return false       - kiedy operacja się nie powiedzie
 */
bool Drone::makeHorizontalFlight(double flightLenght, PzG::LaczeDoGNUPlota& Lacze){
    cout << "Lot do przodu ... " << endl;
    for (; _position[0] <= 150; _position[0] += 1, _position[1] += 1) {
        if (!this->calcAndSave_DroneCoords()) return false;
        usleep(100000);
        Lacze.Rysuj();
    }  
    _position[0] -= 1;
    _position[1] -= 1;
}


/**
 * @brief Funkcja przemieszcza drona wokol wlasnej osi o podany kat.
 * 
 * 
 * @param angle  - kąt obrotu
 * @param Lacze  - łącze do gnuplota
 * @return true  - zwraca kiedy operacja się powiedzie
 * @return false - kiedy operacja się nie powiedzie
 */
bool Drone::changeDroneOrientation(double angle, PzG::LaczeDoGNUPlota& Lacze){
    cout << "Zmiana orientacji ... " << endl;
    for (; _orientationAngle <= angle; _orientationAngle += 5) {
        if (!this->calcAndSave_DroneCoords()) return false;
        usleep(100000);
        Lacze.Rysuj();
    }
    _orientationAngle -= 5;
}


/**
 * @brief Funkcja przemieszcza drona w kierunku pionowym wokół osi z.
 * 
 * 
 * @param flightHeight - Wielkość zmiany położenia wysokości
 * @param Lacze        - łącze do gnuplota
 * @return true        - zwraca kiedy operacja się powiedzie
 * @return false       - kiedy operacja się nie powiedzie
 */
bool Drone::makeVerticalFlight(double flightHeight, PzG::LaczeDoGNUPlota& Lacze){
    if(flightHeight>0){
        cout << endl << "Wznoszenie ... " << endl;
        for (; _position[2] <= flightHeight; _position[2] += 2) {
            if (!this->calcAndSave_DroneCoords()) return false;
            usleep(100000); // 0.1 ms
            Lacze.Rysuj();
        }
        _position[2] -= 2;
    }
    else{
        cout << endl << "Opadanie ... " << endl;
        for (; _position[2] >= 0; _position[2] -= 2) {
            if (!this->calcAndSave_DroneCoords()) return false;
            usleep(100000); // 0.1 ms
            Lacze.Rysuj();
        }
        _position[2] -= 2;
    }
}