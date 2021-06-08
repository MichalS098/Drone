#include <iterator>
#include "Vector.hh"
#include "Scene.hh"
#include "Plateau.hh"
#include "LongRidgeHill.hh"
#include "SharpTopHill.hh"
#define FIRST_DRONE_COLOR 3  //niebieski
#define SECOND_DRONE_COLOR 3 //niebieski
#define UNIT_SCALE 1,1,1

using namespace std;


/**
 * @brief Konstruktor sceny, tworzący dwa drony.
 * 
 * Konstruktor sceny, inicjalizujący scenę referencją do łącza do gnuplota,
 * tworząc dwa drony z wprowadzonymi na sztywno pozycjami początkowymi.
 * Ustawia również początkowo pierwszego drona jako aktywnego drona.
 * 
 * @param[in] Lacze Łącze do GNUPLOT'a.
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
	Plateau *plt = new Plateau(Vector<3>({70,30,6}), Vector<3>({20,16,12}), 1);
	_lacze.DodajNazwePliku(plt->takeFileName_finalFig().c_str());
	_lstOfElements.push_back(plt);
	_numberOfElements[0]=1;

	//Góra z długą granią
	LongRidgeHill *lrg = new LongRidgeHill(Vector<3>({80,140,8}), Vector<3>({20,18,16}), 1);
	_lacze.DodajNazwePliku(lrg->takeFileName_finalFig().c_str());
	_lstOfElements.push_back(lrg);
	_numberOfElements[1]=1;

	//Góra z ostrym szczytem
	SharpTopHill *sth = new SharpTopHill(Vector<3>({150,70,10}), Vector<3>({20,20,20}), 1);
	_lacze.DodajNazwePliku(sth->takeFileName_finalFig().c_str());
	_lstOfElements.push_back(sth);
	_numberOfElements[2]=1;
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
	cout<<endl<<endl<<endl<<"Podaj skale wzdluz kolejnych osi OX, OY, OZ."<<endl;
	cout<<"Wprowadz skale> ";
	cin>>scale;
	cout<<endl<<"Wprowadz wspolrzedne srodka podstawy x,y."<<endl;
	cout<<"Wprowadz wspolrzedne: x, y>";	
	cin>>xPos>>yPos;
	position=Vector<3>({xPos,yPos,scale[2]/2});
	
	switch (elementType){
	case 1:{
		_numberOfElements[1]++;
		SharpTopHill *sth = new SharpTopHill(position, scale, _numberOfElements[1]);
		_lacze.DodajNazwePliku(sth->takeFileName_finalFig().c_str());
		_lstOfElements.push_back(sth);
		break;
	}
	case 2:{
		_numberOfElements[2]++;
		LongRidgeHill *lrg = new LongRidgeHill(position, scale, _numberOfElements[2]);
		_lacze.DodajNazwePliku(lrg->takeFileName_finalFig().c_str());
		_lstOfElements.push_back(lrg);
		break;
	}
	case 3:{
		_numberOfElements[0]++;
		Plateau *plt = new Plateau(position, scale, _numberOfElements[0]);
		_lacze.DodajNazwePliku(plt->takeFileName_finalFig().c_str());
		_lstOfElements.push_back(plt);
		break;
	}
	default:
		cerr<<"Nieprawidlowy numer typu elementu!"<<endl;
		break;
	}
	_lacze.Rysuj();
	cout<<endl<<endl<<"Element zostal dodany do sceny."<<endl;
}


/**
 * @brief Funkcja usuwa element ze sceny.
 * 
 * Funkcja usuwa wybrany przez użytkownika element ze sceny, jedno-
 * cześnie usuwając nazwe jego pliku z łącza do gnuplota.
 */
void Scene::deleteElement(){
	unsigned int k{0}, elemNumber{0};
	cout<<endl<<"Wybierz element powierzchni do usuniecia:"<<endl;
	//Wypisanie wszystkich elementów
	for(const GeometricFigure* fig : _lstOfElements){  			
		cout<<++k<<" - ";
		cout<<fig->getPosition()<<fig->getType()<<endl;
	}
	cout<<"Podaj numer elementu> ";
	cin>>elemNumber;
	list<GeometricFigure*>::iterator iter = _lstOfElements.begin();
	advance(iter, (elemNumber-1));						//przesuwa wskaźnik "elemNumber-1" razy 
	GeometricFigure* fig = *iter;						//kopiuje wskaznik na ten element zeby dostac dostep do jego nazwy pliku
	_lstOfElements.erase(iter);							//usuwa element z listy
	_lacze.UsunNazwePliku(fig->takeFileName_finalFig().c_str());	//usuwa plik tego elementu z lącza
	_lacze.Rysuj();
	cout<<"Element poprawnie usunięty"<<endl;
}

