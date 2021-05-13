#include "Drone.hh"
#include <fstream>
#include <iostream>
#include <assert.h>
#include <unistd.h>

#define PLIK_TRASY_PRZELOTU "dat/trasa_przelotu.dat"
#define PLIK_WZORCOWY_GRANIASTOSLUP "bryly_wzorcowe/graniastoslup6.dat"
#define PLIK_WZORCOWY_PROSTOPADLOSCIAN "bryly_wzorcowe/szescian"

using namespace std;

/**
 * @brief Konstruktor obiektu dron.
 * 
 * Konstruktor drona, przypisuje on wartości zerowe do wektorów pozycji i kąta orientacji,
 * tworzy obiekty prostopadłościan oraz 4 graniastosłupy sześciokątne inicjalizując je nazwami
 * plików wzorcowych oraz plików finalnych
 * 
 */
Drone::Drone(){
    Rectangular body(PLIK_WZORCOWY_GRANIASTOSLUP, "text");
    droneBody=body;
    HexagonalPrism rotor2(PLIK_WZORCOWY_GRANIASTOSLUP, "xd");
    for(int i=0; i<4; ++i){
        droneRotor[i]=rotor2
    }
    droneRotor
}
























/*!
 * \brief Przelicza wartość kąta ze stopni na radiany.
 *
 * Przelicza wartość kąta ze stopni na radiany.
 * \param[in] Kat_st - wartość kąta wyrażona w stopniach.
 * \return Wartość przeliczonego kąta wyrażona w radianach.
 */
double StopnieNaRadiany(double Kat_st)
{
    return Kat_st*M_PI/180;
}

void  ObrocWzgledemOsiOZ( double  KatObrotu_st, Vector<3>& wierz)
{
    double Kat_rad = StopnieNaRadiany(KatObrotu_st);
    double sn = sin(Kat_rad), cn = cos(Kat_rad);
    wierz[0] = wierz[0]*cn-wierz[1]*sn;
    wierz[1] = wierz[0]*sn+wierz[1]*cn;
}























/**
 * @brief Oblicza i zapisuje wspolrzedne korpusu
 * 
 * @return true kiedy operacja sie powiodla
 * @return false kiedy operacja sie nie powiodla
 */
bool Drone::calcAndSave_BodyCoords() const{
    ifstream referenceFig{droneBody.takeFileName_refFig()};
    ofstream finalFig{droneBody.takeFileName_finalFig()};

    if (!referenceFig.is_open()) {
        cerr << endl
	    << " Blad otwarcia do odczytu pliku: " << droneBody.takeFileName_refFig() << endl
	    << endl;
        return false;
    }
    if (!finalFig.is_open()) {
        cerr << endl
        << " Blad otwarcia do odczytu pliku: " << droneBody.takeFileName_finalFig() << endl
	    << endl;
        return false;
    }

    Vector<3> bodyPosVec;
    while (!referenceFig.fail()) {
        for (unsigned int apexCounter=0; apexCounter < 4; ++apexCounter){
            referenceFig>>bodyPosVec;         //wczytanie z pliku linii
            bodyPosVec = droneBody.scaleUp(this->position);  
            bodyPosVec = droneBody.transfToParentCoordSys(bodyPosVec);
            bodyPosVec = this->transfToParentCoordSys(bodyPosVec);
            finalFig << bodyPosVec[0] << " " << bodyPosVec[1] << " " << bodyPosVec[2] << endl;
            assert(apexCounter == 3 || !referenceFig.fail());
        }
        finalFig << endl;
    }
    referenceFig.close();
    finalFig.close();
    return !finalFig.fail();
}

/**
 * @brief Funkcja oblicza i zapisuje wspolrzedne rotora.
 * 
 * @param rotor Obiekt ktorego wspolrzedne beda obliczane.
 * @return true jesli operacja a dokladniej otwarcie pliku sie powiedzie.
 * @return false jesli operacja sie nie powiedzie, czyli np nie ma pliku do otwarcia.
 */
bool Drone::calcAndSave_RotorCoords(const HexagonalPrism& rotor) const{
    ifstream referenceFig{rotor.takeFileName_refFig()};
    ofstream finalFig{rotor.takeFileName_finalFig()};

    if (!referenceFig.is_open()) {
        cerr << endl
	    << " Blad otwarcia do odczytu pliku: " << rotor.takeFileName_refFig() << endl
	    << endl;
        return false;
    }
    if (!finalFig.is_open()) {
        cerr << endl
        << " Blad otwarcia do odczytu pliku: " << rotor.takeFileName_finalFig() << endl
	    << endl;
        return false;
    }

    Vector<3> rotorPos;
    while (!referenceFig.fail()) {
        for (unsigned int apexCounter=0; apexCounter < 4; ++apexCounter){
            referenceFig>>rotorPos;         //wczytanie z pliku linii
            //rotorPos = rotor.scaleUp(rotorPos);  
            rotorPos = rotor.transfToParentCoordSys(this->position);
            rotorPos = this->transfToParentCoordSys(rotorPos);
            finalFig << rotorPos[0] << " " << rotorPos[1] << " " << rotorPos[2] << endl;
            assert(apexCounter == 3 || !referenceFig.fail());
        }
        finalFig << endl;
    }
    referenceFig.close();
    finalFig.close();
    return !finalFig.fail();
}


Vector<3> Drone::transfToParentCoordSys(const Vector<3>& apex) const{

}

/**
 * @brief Funkcja zapisuje do pliku planowana sciezke przelotu drona
 * 
 *  Funkcja zapisuje do pliku planowana sciezke przelotu drona
 *  i przypisuje ten plik do gnuplota poprzez lacze
 * @param turnAngle kat w kierunku ktorego bedziemy rysowac sciezke
 * @param flightLenght dlugosc sciezki
 * @param pathPoints wektor z STL do przechowywania punktow przelotu
 */
void Drone::planInitialFlightPath(double turnAngle, double flightLenght, double height,
                                  std::vector<Vector<3>>& pathPoints,
                                  PzG::LaczeDoGNUPlota& Lacze)
{
    orientationAngle=turnAngle;

    ofstream  pathFile(PLIK_TRASY_PRZELOTU);
    if (!pathFile.is_open()) {
        cerr << endl
	    << " Nie mozna otworzyc do zapisu pliku: " << PLIK_TRASY_PRZELOTU << endl
	    << endl;
        exit (1);
    }
    Vector<3> v2=position;
    v2[2]=v2[2]+height;
    Vector<3> v3{position[0]+flightLenght,
                 position[1]+flightLenght,
                 position[2]+flightLenght};
    ObrocWzgledemOsiOZ(turnAngle, v3);
    Vector<3> v4=v3;
    v4[2]=position[2];
    pathFile<< this->position << endl
         	<< v2 << endl
         	<< v3 << endl
         	<< v4 << endl;
    Lacze.DodajNazwePliku(PLIK_TRASY_PRZELOTU);
}



bool Drone::makeVerticalFlight(double height, PzG::LaczeDoGNUPlota& Lacze){
    if(height>0){
        cout << endl << "Wznoszenie ... " << endl;
        for (; position[2] <= height; position[2] += 2) {
            calcAndSave_DroneCoords();
            usleep(100000); // 0.1 ms
            Lacze.Rysuj();
        }
        position[2] = position[2]-2;
    }
    else{
        cout << endl << "Opadanie ... " << endl;
        for (; position[2] >= height; position[2] -= 2) {
            calcAndSave_DroneCoords();
            usleep(100000); // 0.1 ms
            Lacze.Rysuj();
        }
        position[2] = position[2]+2;
    }   
    }
}

bool Drone::makeHorizontalFlight(double flightLenght, PzG::LaczeDoGNUPlota& Lacze){
    cout << "Lot do przodu ... " << endl;
    for (; position[0] <= flightLenght; position[0] += 1, position[1] += 1) {
        calcAndSave_DroneCoords();
        usleep(100000);
        Lacze.Rysuj();
    }  
    position[0] = position[0]-1, position[1] = position[1]-1;
}

/**
 * @brief Funkcja oblicza i zapisuje wspolrzedne drona 
 *        
 * Zapisujac wspolrzedne rotorow i korpusu w plikach 
 * 
 * @return true kiedy operacja sie powiedzie
 * @return false kiedy operacja sie nie powiedzie
 */
bool Drone::calcAndSave_DroneCoords() const{
    if(!calcAndSave_BodyCoords()) return 1;
    for(int i=0; i<4; ++i){
        if(!calcAndSave_RotorCoords(droneRotor[i])) return 1;
    }
    return 0;
}