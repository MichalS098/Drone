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
	
	Drone firstDrone;
	Vector<3> initialPosition={20,20,20};
	Vector<3> scale={1,1,1};
	firstDrone.makeDrone(initialPosition, scale, 1, Lacze);
  	Lacze.Rysuj();        // Teraz powinno pojawic sie okienko gnuplota
  	cout << "Nacisnij ENTER, aby pokazac sciezke przelotu drona " << flush;
  	cin.ignore(10000,'\n');

	firstDrone.planInitialFlightPath(FLIGHT_HEIGHT, 45, FLIGHT_LENGHT, Lacze);
  	Lacze.Rysuj();

  	cout << "Nacisnij ENTER, aby wykonac animacje lotu drona " << flush;
  	cin.ignore(10000,'\n');
	//animacja

  	cout << endl << "Nacisnij ENTER, aby usunac sciezke ... " << flush;
  	cin.ignore(10000,'\n');

	firstDrone.deleteFlightPath(Lacze);
  	Lacze.Rysuj();
  
  	cout << "Nacisnij ENTER, aby zakonczyc ... " << flush;
  	cin.ignore(10000,'\n');

	return 0;
}
