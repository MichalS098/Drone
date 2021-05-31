#include "Vector.hh"
#include "Scene.hh"
#include "Plateau.hh"
#include "LongRidgeHill.hh"
#include "SharpTopHill.hh"
#define FIRST_DRONE_COLOR 3 //niebieski
#define SECOND_DRONE_COLOR 3 //niebieski
#define UNIT_SCALE 1,1,1
#define SHARP_HILL_FILE "dat/PlikWlasciwy_GoraZOstrymSzczytem.dat"
#define LONG_RIDGE_HILL_FILE "dat/PlikWlasciwy_GoraZDlugaGrania.dat"
#define PLATEAU_FILE "dat/PlikWlasciwy_Plaskowyz.dat"
#define CUBE_SAMPLE_FILE "bryly_wzorcowe/szescian.dat"
using namespace std;


/**
 * @brief Konstruktor sceny, tworzący dwa drony.
 * 
 * Konstruktor sceny, inicjalizujący scenę referencją do łącza do gnuplota,
 * tworząc dwa drony z wprowadzonymi na sztywno pozycjami początkowymi.
 * Ustawia również początkowo pierwszego drona jako aktywnego drona.
 */
Scene::Scene(PzG::LaczeDoGNUPlota& Lacze): _ID_of_active_Drone{0}, _lacze{Lacze}
{
    Vector<3> DroneInitialPosition({20,20,4});  //wysokosc poczatkowa 4 bo skala 8(zeby korpus byl nad ziemia)
	Vector<3> secondDroneInitialPosition({50,70,4});
	Vector<3> scale({UNIT_SCALE});
    _droneArray[0].makeDrone(DroneInitialPosition, scale, 1, Lacze, FIRST_DRONE_COLOR);
	_droneArray[0].calcAndSave_DroneCoords();
	_droneArray[1].makeDrone(secondDroneInitialPosition, scale, 2, Lacze, SECOND_DRONE_COLOR);
    _droneArray[1].calcAndSave_DroneCoords();

	//Płaskowyż
	Plateau plt(Vector<3>({70,30,6}), Vector<3>({20,16,12}), CUBE_SAMPLE_FILE, PLATEAU_FILE);
	_lacze.DodajNazwePliku(PLATEAU_FILE);
	_lstOfElements.push_back(&plt);
	//Góra z długą granią
	LongRidgeHill lrg(Vector<3>({80,140,8}), Vector<3>({20,18,16}), CUBE_SAMPLE_FILE, LONG_RIDGE_HILL_FILE);
	_lacze.DodajNazwePliku(LONG_RIDGE_HILL_FILE);
	_lstOfElements.push_back(&lrg);
	//Góra z ostrym szczytem
	SharpTopHill sth(Vector<3>({150,70,10}), Vector<3>({20,20,20}), CUBE_SAMPLE_FILE, SHARP_HILL_FILE);
	_lacze.DodajNazwePliku(SHARP_HILL_FILE);
	_lstOfElements.push_back(&sth);
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

	//tworze drona na nowo by zmienic kolor
	for(int i=1; i<=4; ++i){
		_lacze.UsunNazwePliku(makeRotorFileName(_ID_of_active_Drone+1, i));
	}
	_lacze.UsunNazwePliku(makeBodyFileName(_ID_of_active_Drone+1));
	Vector<3> newPosition = _droneArray[_ID_of_active_Drone].takeDronePosition();
	_droneArray[_ID_of_active_Drone].makeDrone(newPosition, Vector<3>{1,1,1},_ID_of_active_Drone+1, _lacze, 0);
    _lacze.Rysuj();
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
void Scene::printPositionOfActiveDrone() const{
    cout<<"\tPolozenie Drona aktywnego (x,y):"
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
}

/**
 * @brief Funkcja dodaje nowy element powierzchni.
 * 
 * Funkcja dodaje nowy element powierzchni, pytając użytkownika
 * o rodzaj tego elementu, jego skale i współrzędne położenia.
 */
void Scene::makeNewElement(){
	unsigned int elementType{0};
	Vector<3> scale, position;
	double xPos, yPos;
	cout<<"Wybierz rodzaj powierzchniowego elementu"<<endl<<endl;
	cout<<"\t1 - Gora z ostrym szczytem"<<endl;
	cout<<"\t2 - Gora z grania"<<endl;
	cout<<"\t3 - Plaskowyz"<<endl<<endl;
	cout<<"Wprowadz numer typu elementu> ";
	cin>>elementType;
	cout<<endl<<endl<<endl<<"Podaj skale wzdlud kolejnych osi OX, OY, OZ."<<endl;
	cout<<"Wprowadz skale> ";
	cin>>scale;
	cout<<endl<<"Wprowadz wspolrzedne srodka podstawy x,y."<<endl;
	cout<<"Wprowadz wspolrzedne: x, y>";	
	cin>>xPos>>yPos;
	position=Vector<3>({xPos,yPos,scale[2]/2});

	switch (elementType){
	case 1:{
		SharpTopHill sth(position, scale, CUBE_SAMPLE_FILE, SHARP_HILL_FILE);
		_lacze.DodajNazwePliku(SHARP_HILL_FILE);
		_lstOfElements.push_back(&sth);
		break;
	}
	case 2:{
		LongRidgeHill lrg(position, scale, CUBE_SAMPLE_FILE, LONG_RIDGE_HILL_FILE);
		_lacze.DodajNazwePliku(LONG_RIDGE_HILL_FILE);
		_lstOfElements.push_back(&lrg);
		break;
	}
	case 3:{
		Plateau plt(position, scale, CUBE_SAMPLE_FILE, PLATEAU_FILE);
		_lacze.DodajNazwePliku(PLATEAU_FILE);
		_lstOfElements.push_back(&plt);
		break;
	}
	default:
		cerr<<"Nieprawidlowy numer typu elementu!"<<endl;
		break;
	}
	cout<<endl<<endl<<"Element zostal dodany do sceny."<<endl;
}


/**
 * @brief Funkcja usuwa element ze sceny.
 * 
 * Funkcja usuwa wybrany przez użytkownika element ze sceny.
 */
void Scene::deleteElement(){
	unsigned int k{0};
	cout<<endl<<"Wybierz element powierzchni do usuniecia:"<<endl;
	for(const GeometricFigure* elem: _lstOfElements){
		cout<<++k<<" - ";
		elem->
	}
}