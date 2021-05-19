#include "Scene.hh"

#define FIRST_DRONE_COLOR 0 //czerwony
#define SECOND_DRONE_COLOR 3 //niebieski
#define UNIT_SCALE 1,1,1


using namespace std;


/**
 * @brief Konstruktor sceny, tworzący dwa drony.
 * 
 * Konstruktor sceny, inicjalizujący scenę referencją do łącza do gnuplota,
 * tworząc dwa drony z wprowadzonymi na sztywno pozycjami początkowymi.
 * Ustawia również początkowo pierwszego drona jako aktywnego drona.
 */
Scene::Scene(PzG::LaczeDoGNUPlota& Lacze): _lacze{Lacze}, _ID_of_active_Drone{0}
{
    Vector<3> initialPosition({20,20,0});  //wysokosc poczatkowa 4 bo skala 8(zeby korpus byl nad ziemia)
	Vector<3> secondInitialPosition({50,70,0});
	Vector<3> scale({UNIT_SCALE});
    _droneArray[0].makeDrone(initialPosition, scale, 1, Lacze, FIRST_DRONE_COLOR);
	_droneArray[0].calcAndSave_DroneCoords();
	_droneArray[1].makeDrone(secondInitialPosition, scale, 2, Lacze, SECOND_DRONE_COLOR);
    _droneArray[1].calcAndSave_DroneCoords();
}


/**
 * @brief Funkcja pozwala na wybranie przez użytkownika aktywnego drona.
 * 
 * @return const Drone& - zwraca stałą referencje do aktywnego drona.
 */
const Drone& Scene::takeActiveDrone(){
	unsigned int droneID=0;
	cout<<"1 - Polozenie (x,y): "<<_droneArray[0].takeDronePosition();
	if(_ID_of_active_Drone==0) cout<<" <-- Dron aktywny";
	cout<<endl;
    cout<<"2 - Polozenie (x,y): "<<_droneArray[1].takeDronePosition();
	if(_ID_of_active_Drone==1) cout<<" <-- Dron aktywny";
	cout<<endl;
	cout<<"Wprowadz numer aktywnego drona(1 lub 2)> ";
	cin>>droneID;
	_ID_of_active_Drone=droneID-1;
    return _droneArray[_ID_of_active_Drone];
}


/**
 * @brief Funkcja używa aktywnego drona.
 * 
 * Funkcja używa aktywnego drona czyli zwraca referencje do aktywnego
 * drona co pozwala na wykonanie jego metod czy użycie go w funckji. 
 * @return Drone& - zwracana jest referencja do używanego drona.
 */
Drone& Scene::useActiveDrone(){
	return _droneArray[_ID_of_active_Drone];
}


/**
 * @brief Funkcja wypisuje na stdout położenie aktywnego drona. 
 */
void Scene::printPositionOfActiveDrone(){
    cout<<"Polozenie Drona aktywnego (x,y):"
        <<_droneArray[_ID_of_active_Drone].takeDronePosition()<<endl;
}


/**
 * @brief Funkcja wykonuje animacje lotu drona.
 * 
 * 
 * Pyta użytkownika o podanie długości lotu, kierunku lotu oraz jego wysokości,
 * wywołuje funkcje rysującą planowaną ścieżke i wywołuje lot aktywnego drona.
 */
void Scene::droneFlightAnimation(){
	double flightDirection;
	double flightLenght;
	double flightHeight;
	cout<<"Podaj kierunek lotu (kat w stopniach)> ";
	cin>>flightDirection;
	cout<<"Podaj dlugosc lotu> ";
	cin>>flightLenght;
	cout<<"Podaj wysokosc lotu> ";
	cin>>flightHeight; cout<<endl<<endl;

	cout<<"Rysuje zaplanowana sciezke lotu..." <<endl<<endl<<endl<<endl;
	useActiveDrone().planInitialFlightPath(flightHeight, flightDirection, flightLenght, _lacze);
  	_lacze.Rysuj();
  	cout<<"Realizacja przelotu..." <<endl<<endl<<endl<<endl;
	useActiveDrone().makeVerticalFlight(flightHeight, 		 _lacze);
	useActiveDrone().changeDroneOrientation(flightDirection, _lacze);
	useActiveDrone().makeHorizontalFlight(flightLenght, 	 _lacze);
	useActiveDrone().makeVerticalFlight(-flightHeight, 		 _lacze);
  	cout<<endl<<"Dron wyladowal... " <<endl<<endl<<"Usuwam sciezke..."<<endl;
	useActiveDrone().deleteFlightPath(_lacze);
  	_lacze.Rysuj();
	printPositionOfActiveDrone();
}