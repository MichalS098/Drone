#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cassert>
#include <unistd.h>
#include "lacze_do_gnuplota.hh"
#include "Drone.hh"

#define FLIGHT_HEIGHT 50
#define FLIGHT_LENGHT 80

using namespace std;

int main(){
    PzG::LaczeDoGNUPlota  Lacze;
    Lacze.ZmienTrybRys(PzG::TR_3D);
    Lacze.Inicjalizuj();  // Tutaj startuje gnuplot.
  	Lacze.UstawZakresX(0, 200);
  	Lacze.UstawZakresY(0, 200);
  	Lacze.UstawZakresZ(0, 120);
  	Lacze.UstawRotacjeXZ(64,65); // Tutaj ustawiany jest widok
	
	Lacze.DodajNazwePliku("bryly_wzorcowe/plaszczyzna.dat");

	Drone firstDrone, secondDrone;
	Vector<3> initialPosition({20,20,4});  //wysokosc poczatkowa 4 bo skala 8(zeby korpus byl nad ziemia)
	Vector<3> scale({1,1,1});

	firstDrone.makeDrone(initialPosition, scale, 1, Lacze);
	secondDrone.makeDrone(Vector<3>{40,50,4}, scale, 2, Lacze);
	secondDrone.calcAndSave_DroneCoords();
	firstDrone.calcAndSave_DroneCoords();


  	Lacze.Rysuj();        // Teraz powinno pojawic sie okienko gnuplota
  	cout << "Nacisnij ENTER, aby pokazac sciezke przelotu drona " << flush;
  	cin.ignore(10000,'\n');

	firstDrone.planInitialFlightPath(FLIGHT_HEIGHT, 45, FLIGHT_LENGHT, Lacze);
  	Lacze.Rysuj();

  	cout << "Nacisnij ENTER, aby wykonac animacje lotu drona " << flush;
  	cin.ignore(10000,'\n');
	//animacja
	firstDrone.makeVerticalFlight(FLIGHT_HEIGHT, Lacze);
	firstDrone.changeDroneOrientation(45, Lacze);
	firstDrone.makeHorizontalFlight(FLIGHT_LENGHT-4, Lacze);
	firstDrone.makeVerticalFlight(-FLIGHT_HEIGHT, Lacze);
  	cout << endl << "Nacisnij ENTER, aby usunac sciezke ... " << flush;
  	cin.ignore(10000,'\n');
	firstDrone.deleteFlightPath(Lacze);
  	Lacze.Rysuj();
  
  	cout << "Nacisnij ENTER, aby zakonczyc ... " << flush;
  	cin.ignore(10000,'\n');

	return 0;
}
