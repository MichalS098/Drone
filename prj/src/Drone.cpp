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
 * @brief Konstruktor inicjalizujący Drona.
 * 
 * 
 * Funkcja inicjalizuje drona poprzez, przypisanie jego atrybutom 
 * (prostopadłościanowi oraz rotorom) ich nazwy plików właściwych oraz wzorcowych.
 * Funkcja dodaje odrazu nazwy tych plików do łącza do GNU-PLOT'a.
 * Ustawia również domyślny kolor rysowanego drona, jego pozycje początkową, skale wielkości
 * oraz jego ID.
 * @param[in] ID           - id Drona
 * @param[in,out] Lacze    - Lacze do gnuplota 
 * @param[in] position     - pozycja początkowa drona
 * @param[in] scale        - skala rysowanego drona
 * @param[in] droneColor   - kolor rysowanego Drona
 */
Drone::Drone(const Vector<3>& position, const Vector<3>& scale, unsigned int ID, PzG::LaczeDoGNUPlota& Lacze, int droneColor)
{
    _ID=ID;
    _position=position;
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
 * @param flightHeight - wysokosc lotu
 * @param lacze        - lacze do GNUPlota       
 */
void Drone::planInitialFlightPath(double flightHeight, double turnAngle, double flightLenght, PzG::LaczeDoGNUPlota& lacze){
    Vector<3> vec;
    Vector<3> direction({(flightLenght>0.0 ? 1.0: -1.0), 0.0, 0.0});
    Vector<3> destination;
    std::vector<Vector<3U>> pathPoints;
    Matrix<3> rotationMatrix;
    makeRotationMatrix('z', turnAngle+_orientationAngle, rotationMatrix);
    
    direction = rotationMatrix * direction;
    destination = _position + direction * flightLenght;

    vec = _position;   
    vec[2] = 0;
    pathPoints.push_back(vec);
    vec[2] = (flightHeight+4);
    pathPoints.push_back(vec);
    vec=destination;
    vec[2] = (flightHeight+4);
    pathPoints.push_back(vec);
    vec[2] = 0;
    pathPoints.push_back(vec);

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
 * @brief Funkcja wykonuje lot drona. 
 * 
 * Funkcja wykonuje lot drona w odpowiednim kierunku (direction)
 * na odpowiedniom długość (flightLenght).
 * @param[in] direction     Kierunek lotu drona.
 * @param[in] flightLenght  Długość lotu.
 * @param[out] lacze        Łącze do GNUPlota (API).
 * @return true Kiedy operacja się powiedzie.
 * @return false Kiedy operacja się nie powiedzie.
 */
bool Drone::makeFlight(const Vector<3>& direction, double flightLenght, PzG::LaczeDoGNUPlota& lacze){
    assert(fabs(direction.lenght()-1) < COMPUTATION_ERR);
    Vector<3> startPosition = _position;
    Vector<3> destination = startPosition + direction * flightLenght;
    Vector<3> directionSingleStep = direction * singleStepLenght; 

    double remainingFlightLenght=flightLenght;
    remainingFlightLenght -= singleStepLenght;
    while(remainingFlightLenght > 0){
        _position = _position + directionSingleStep;
        this->rotateRotor(5*remainingFlightLenght);
        if (!this->calcAndSave_DroneCoords()) return false;
        usleep(100000);
        lacze.Rysuj();    
        remainingFlightLenght -= singleStepLenght;
    }
    _position=destination;
    if (!this->calcAndSave_DroneCoords()) return false;
    usleep(100000);
    lacze.Rysuj();
    return true;
}   


/**
 * @brief Funkcja przemieszcza drona do przodu.
 * 
 * 
 * @param flightLenght - długość na jaką ma polecieć dron
 * @param Lacze        - łącze do gnuplota
 * @return true        - zwraca kiedy operacja się powiedzie
 * @return false       - kiedy operacja się nie powiedzie
 */
bool Drone::makeHorizontalFlight(double flightLenght, PzG::LaczeDoGNUPlota& Lacze){
    Vector<3> direction({(flightLenght>0.0 ? 1.0: -1.0), 0.0, 0.0});
    Matrix<3> rotationMatrix;

    makeRotationMatrix('z', _orientationAngle, rotationMatrix);
    direction = rotationMatrix * direction;
    makeFlight(direction, fabs(flightLenght), Lacze);
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
    Vector<3> direction({0.0,0.0,(flightHeight>0.0?1.0:-1.0)}); //inicjalizacja (0,0,1) lub (0,0,-1)
    makeFlight(direction, fabs(flightHeight), Lacze);
    return true;
}


/**
 * @brief Funkcja odpowiadająca za animacje obrotu wirników drona.
 * 
 *
 * Funkcja odpowiadająca za animacje obrotu wirników drona, 
 * obraca ona wszystkie 4 rotory drona o zadany kąt, uwzględniając zależność
 * że wirniki nie kręcą się wszystkie w jedną stronę.
 * @param angle - Kąt obrotu
 */
void Drone::rotateRotor(double angle){
    int k=0;
    for(int i=0; i<4; ++i){
        k = (i%2==0) ? 1 : -1;
        _droneRotor[i].enterOrientationAngle(k*angle);
    }
}


/** NIE WIEM CO TO
 * @brief Funkcja pochyla drona w kierunku lotu
 * 
 * @param Lacze - lacze do GNUPlota
 */
bool Drone::tiltForward(PzG::LaczeDoGNUPlota& Lacze){
    for (; _orientationAngle <= 20; _orientationAngle += 5) {
        
        if (!this->calcAndSave_DroneCoords()) return false;
        usleep(100000);
        Lacze.Rysuj();
    }
    _orientationAngle -= 5;
    return true;
}


/**
 * @brief Funkcja sprawdza czy miejsce w którym dron się znajduję jest wolne.
 * 
 * @return true 
 * @return false 
 */
bool Drone::checkIfPlaceIsAvaliable() const{return 1;}