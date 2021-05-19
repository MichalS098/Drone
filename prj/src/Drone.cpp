#include "Drone.hh"
#include "GeometricFigure.hh"
#include "Matrix.hh"
#include <cmath>
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

#define DRONE_BODY_SCALE 8.0
#define DRONE_ROTOR_SCALE 4.0

/**
 * @brief Funkcja inicjalizuje Drona.
 * 
 * 
 * Funkcja inicjalizuje drona poprzez, przypisanie jego atrybutom 
 * (prostopadłościanowi oraz rotorom) ich nazwy plików właściwych oraz wzorcowych.
 * Funkcja dodaje odrazu nazwy tych plików do łącza do GNU-PLOT'a.
 * Ustawia również domyślny kolor rysowanego drona, jego pozycje początkową, skale wielkości
 * oraz jego ID.
 * @param ID    - id Drona
 * @param Lacze - Lacze do gnuplota 
 * @param position - pozycja początkowa drona
 * @param scale - skala rysowanego drona
 * @param droneColor - kolor rysowanego Drona
 */
void Drone::makeDrone(const Vector<3>& position, const Vector<3>& scale, 
                      unsigned int ID, PzG::LaczeDoGNUPlota& Lacze, int droneColor)
{
    _ID=ID;
    _position=position;
    _position[2]+=4;     //zwiększenie wysokości o 4 żeby dron znajdował się równo na ziemi
    _orientationAngle=0;
    unsigned int index=0,index2=0;
    string fileName;

    // Ustawiam polozenie rotorow drona w stosunku do srodka korpusu drona
    Vector<3> rotorPositions[4]={{4,4,4},{4,-4,4},{-4,4,4},{-4,-4,4}};

    for(HexagonalPrism& rotor: _droneRotor){
        fileName=makeRotorFileName(ID, ++index);
        rotor.enterFileName_finalFig(fileName);
        rotor.enterFileName_refFig(REF_ROTOR_FILE_NAME);
        Lacze.DodajNazwePliku(fileName.c_str()).ZmienKolor(droneColor);
        rotor.setScale(scale*DRONE_ROTOR_SCALE);
        rotor.enterOrientationAngle(_orientationAngle);
        rotor.enterPosition(rotorPositions[index2++]);
    }
    fileName=makeBodyFileName(ID);
    _droneBody.enterFileName_finalFig(fileName);
    _droneBody.enterFileName_refFig(REF_BODY_FILE_NAME);
    _droneBody.setScale(scale*DRONE_BODY_SCALE);
    _droneBody.enterOrientationAngle(_orientationAngle);
    Lacze.DodajNazwePliku(fileName.c_str()).ZmienKolor(droneColor);
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
    Vector<3> apex;
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
        for (int apexSize = 0; apexSize < 4; ++apexSize){
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
void Drone::planInitialFlightPath(double flightHeight, double turnAngle, double flightLenght, PzG::LaczeDoGNUPlota& lacze){
    Vector<3U> vec;
    _orientationAngle=turnAngle;
    vector<Vector<3U>> pathPoints;

    vec[2] -=4;
    pathPoints.push_back(transfToParentCoordSys(vec));    /* poczatek */
    vec[2] += flightHeight-4;
    pathPoints.push_back(transfToParentCoordSys(vec));    /* po uniesieniu */
    vec[0] = flightLenght;
    pathPoints.push_back(transfToParentCoordSys(vec));    /* po locie poziomym */
    vec[2] -= flightHeight-4;
    pathPoints.push_back(transfToParentCoordSys(vec));    /* po ladowaniu */

    ofstream fileNameStr(FLIGHT_PATH_FILE_NAME);
    if (!fileNameStr.is_open()){
        cerr << endl
	    << " Blad otwarcia pliku: " << FLIGHT_PATH_FILE_NAME << endl
	    << endl;
    }   
    for(unsigned int i=0; i<pathPoints.size(); ++i){
        fileNameStr << pathPoints[i] << endl;
    }
    fileNameStr << endl;
    fileNameStr.close();
    lacze.DodajNazwePliku(FLIGHT_PATH_FILE_NAME);
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
    fileNameStr.close();
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
    double angleRad = _orientationAngle*(M_PI/180);
    double xlenght=_position[0]+flightLenght*cos(angleRad);     //dlugosc do przebycia w kierunku osi x
    double ylenght=_position[1]+flightLenght*sin(angleRad);     
    for (; _position[0] <= xlenght && _position[1]<= ylenght; _position[0] += xlenght/100, _position[1] += ylenght/100) {
        this->rotateRotor(36*_position[0]);
        if (!this->calcAndSave_DroneCoords()) return false;
        usleep(100000);
        Lacze.Rysuj();
    }  
    return true;
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
    for (; _orientationAngle <= angle; _orientationAngle += 5) {
        this->rotateRotor(4*_orientationAngle);
        if (!this->calcAndSave_DroneCoords()) return false;
        usleep(100000);
        Lacze.Rysuj();
    }
    _orientationAngle -= 5;
    return true;
}


/**
 * @brief Funkcja przemieszcza drona w kierunku pionowym wzdłuż osi z.
 * 
 * 
 * @param flightHeight - Wielkość zmiany położenia wysokości
 * @param Lacze        - łącze do gnuplota
 * @return true        - zwraca kiedy operacja się powiedzie
 * @return false       - kiedy operacja się nie powiedzie
 */
bool Drone::makeVerticalFlight(double flightHeight, PzG::LaczeDoGNUPlota& Lacze){
    if(flightHeight>0){
        for (; _position[2] <= flightHeight-4; _position[2] += 2) {       
            this->rotateRotor(4*_position[2]);
            if (!this->calcAndSave_DroneCoords()) return false;
            usleep(100000); // 0.1 ms
            Lacze.Rysuj();
        }
    }
    else{
        for (; _position[2] >= 4; _position[2] -= 2) {
            this->rotateRotor(4*_position[2]);
            if (!this->calcAndSave_DroneCoords()) return false;
            usleep(100000); // 0.1 ms
            Lacze.Rysuj();
        }
        _position[2]+=2;
    }
    return true;
}


/**
 * @brief Funkcja odpowiadająca za animacje obrotu wirników drona.
 * 
 *
 * Funkcja odpowiadająca za animacje obrotu wirników drona, 
 * obraca ona wszystkie 4 rotory drona o zadany kąt, uwzględniając zależność
 * że wirniki nie kręcą się wszystkie w jedną stronę.
 * @param rotor - rotor drona
 */
void Drone::rotateRotor(double angle){
    int k=0;
    for(int i=0; i<4; ++i){
        k = (i%2==0) ? 1 : -1;
        _droneRotor[i].enterOrientationAngle(k*angle);
    }
}