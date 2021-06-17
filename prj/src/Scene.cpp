#include <cmath>
#include <unistd.h>
#include <string.h>
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
Scene::Scene(PzG::LaczeDoGNUPlota& Lacze): _ID_of_active_Drone{0}, _lacze{Lacze}{
	Vector<3> DroneInitialPosition({20,20,4});  //wysokosc poczatkowa 4 bo skala 8(zeby korpus byl nad ziemia)
	Vector<3> secondDroneInitialPosition({50,70,4});
	Vector<3> scale({UNIT_SCALE});
    shared_ptr<SceneObject> sceneObjPtr;  	//inteligentny wskaźnik na obiekt sceny
	shared_ptr<GeometricFigure> GeoFig_Ptr;	//inteligentny wskaźnik na figure geometryczną
	// Tworze pierwszego drona
	shared_ptr<Drone> firstDronePtr = make_shared<Drone>(DroneInitialPosition, scale, 1, Lacze, FIRST_DRONE_COLOR);
	firstDronePtr->calcAndSave_DroneCoords();
	_lstOfDrones.push_back(firstDronePtr);
	sceneObjPtr = firstDronePtr;
	addElementToList(sceneObjPtr);
	// Tworze drugiego drona
	shared_ptr<Drone> secondDronePtr = make_shared<Drone>(secondDroneInitialPosition, scale, 2, Lacze, SECOND_DRONE_COLOR);
	secondDronePtr->calcAndSave_DroneCoords();
	_lstOfDrones.push_back(secondDronePtr);
	sceneObjPtr= secondDronePtr;
	addElementToList(sceneObjPtr);

	//Płaskowyż
	shared_ptr<Plateau> plt_Ptr = make_shared<Plateau>(Vector<3>({70,30,6}), Vector<3>({20,16,12}), 1);
	_lacze.DodajNazwePliku(plt_Ptr->takeFileName_finalFig().c_str());
	sceneObjPtr = plt_Ptr;
	addElementToList(sceneObjPtr);
	_numberOfElements[0]=1;

	//Góra z długą granią
	shared_ptr<LongRidgeHill> lrg_Ptr = make_shared<LongRidgeHill>(Vector<3>({80,140,8}), Vector<3>({20,18,16}), 1);
	_lacze.DodajNazwePliku(lrg_Ptr->takeFileName_finalFig().c_str());
	sceneObjPtr = lrg_Ptr;
	addElementToList(sceneObjPtr);
	_numberOfElements[1]=1;

	//Góra z ostrym szczytem
	shared_ptr<SharpTopHill> sth_Ptr = make_shared<SharpTopHill>(Vector<3>({150,70,10}), Vector<3>({20,20,20}), 1);
	_lacze.DodajNazwePliku(sth_Ptr->takeFileName_finalFig().c_str());
	sceneObjPtr = sth_Ptr;
	addElementToList(sceneObjPtr);
	_numberOfElements[2]=1;
}


/**
 * @brief Funkcja pozwala na wybranie przez użytkownika aktywnego drona.
 * 
 * @return const Drone& - zwraca stałą referencje do aktywnego drona.
 */
const Drone& Scene::takeActiveDrone(){
	unsigned int droneID=0;
	unsigned int k=1;
	for(shared_ptr<Drone> dr : _lstOfDrones){
		cout<<k++<<" - Polozenie (x,y): "<<dr->takeDronePosition();
		if(_ID_of_active_Drone==k-2) cout<<" <-- Dron aktywny";
		cout<<endl;
	}
	cout<<"Wprowadz numer aktywnego drona(1 lub 2)> ";
	cin>>droneID;
	_ID_of_active_Drone=droneID-1;

	//tworze drona na nowo by zmienic kolor
	for(int i=1; i<=4; ++i){
		_lacze.UsunNazwePliku(makeRotorFileName(_ID_of_active_Drone+1, i));
	}
	_lacze.UsunNazwePliku(makeBodyFileName(_ID_of_active_Drone+1));

	list<shared_ptr<Drone>>::iterator iter = _lstOfDrones.begin();
	advance(iter, (_ID_of_active_Drone));	
	shared_ptr<Drone> activeDrone = *iter;
	Vector<3> newPosition = activeDrone->takeDronePosition();
	activeDrone = make_shared<Drone>(newPosition, Vector<3>{1,1,1},_ID_of_active_Drone+1, _lacze, 0);
    _lacze.Rysuj();
	return *activeDrone.get();
}


/**
 * @brief Funkcja używa aktywnego drona.
 * 
 * Funkcja używa aktywnego drona czyli zwraca referencje do aktywnego
 * drona co pozwala na wykonanie jego metod czy użycie go w funckji. 
 * @return Drone& - zwracana jest referencja do używanego drona.
 */
Drone& Scene::useActiveDrone(){
	list<shared_ptr<Drone>>::iterator iter = _lstOfDrones.begin();
	advance(iter, (_ID_of_active_Drone));	
	shared_ptr<Drone> activeDrone = *iter;
	return *activeDrone.get();
}


/**
 * @brief Funkcja zwraca inteligentny wskaźnik na aktywnego drona z listy dronów.
 * @return std::shared_ptr<Drone> zwracany wskaźnik na aktywnego drona. 
 */
shared_ptr<Drone> Scene::takePointerToActiveDrone(){
	list<shared_ptr<Drone>>::iterator iter = _lstOfDrones.begin();
	advance(iter, (_ID_of_active_Drone));	
	shared_ptr<Drone> activeDrone = *iter;
	return activeDrone;
}


/**
 * @brief Funkcja wypisuje na stdout położenie aktywnego drona. 
 */
void Scene::printPositionOfActiveDrone(){
    cout<<"\tPolozenie Drona aktywnego (x,y):"
        <<useActiveDrone().takeDronePosition()<<endl;
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
	bool landingFieldIsFree=false, IsItPlateau=false;
	Vector<3> initialPosition = useActiveDrone().getPosition();

	cout<<"Podaj kierunek lotu (kat w stopniach)> ";
	cin>>flightDirection;
	cout<<"Podaj dlugosc lotu> ";
	cin>>flightLenght;
	cout<<"Podaj wysokosc lotu> ";
	cin>>flightHeight; cout<<endl<<endl;

	cout<<"\tRysuje zaplanowana sciezke lotu..." <<endl<<endl<<endl<<endl;
	useActiveDrone().planInitialFlightPath(flightHeight, flightDirection, initialPosition, flightLenght, _lacze);
  	_lacze.Rysuj();
  	cout<<"\tRealizacja przelotu..." <<endl<<endl<<endl<<endl;
	useActiveDrone().makeVerticalFlight(flightHeight, 		 _lacze);
	useActiveDrone().changeDroneOrientation(flightDirection, _lacze);
	useActiveDrone().makeHorizontalFlight(flightLenght, 	 _lacze);

	while(landingFieldIsFree != true){
		switch (checkIfPlaceIsOccupied(this->takePointerToActiveDrone())){
		case 1:{
			flightLenght+=20;
			usleep(1500000); //1,5sec
			useActiveDrone().deleteFlightPath(_lacze);
			useActiveDrone().planInitialFlightPath(flightHeight, 0, initialPosition, flightLenght, _lacze);
			useActiveDrone().makeHorizontalFlight(20, _lacze);
			break;
		}
		case 2:{
			landingFieldIsFree=true;
			break;
		}
		case 3:{
			IsItPlateau=true;
			usleep(1500000); //1,5sec
			cout<<":)  Ladowisko dostepne, rozpoczeto procedure ladowania na plaskowyzu" <<endl<<endl<<endl<<endl;
			flightHeight -= 12;
			landingFieldIsFree=true;
			break;
		}
		default:
			cerr<<"Blad: nieprawidlowa opcja w funkcji do sprawdzania dostepnosci miejsca ladowania.";
			break;
		}
	}
	if(IsItPlateau == false) {cout<<":)  Ladowisko dostepne, rozpoczeto procedure ladowania" <<endl<<endl<<endl<<endl;}
	useActiveDrone().makeVerticalFlight(-flightHeight, _lacze);
  	cout<<endl<<"\tDron wyladowal... " <<endl<<endl<<"Usuwam sciezke..."<<endl;
	useActiveDrone().deleteFlightPath(_lacze);
  	_lacze.Rysuj();
}


/**
 * @brief Funkcja dodaje nowy element powierzchni.
 * 
 * @param[in] pos Wektor pozycji nowego elementu.
 * @param[in] scale Wektor skali nowego elementu.
 * @param[in] type Typ nowego elementu będący liczbą int wybraną przez użytkownika.
 */
void Scene::makeNewElement(const Vector<3>& pos, const Vector<3> scale, unsigned int type){
	shared_ptr<SceneObject> sceneObjectPtr;
	switch (type){
	case 1:{
		_numberOfElements[1]++;
		shared_ptr<SharpTopHill> sth_Ptr = make_shared<SharpTopHill>(pos,scale,_numberOfElements[1]);
		_lacze.DodajNazwePliku(sth_Ptr->takeFileName_finalFig().c_str());
		sceneObjectPtr = sth_Ptr;
		addElementToList(sceneObjectPtr);
		break;
	}
	case 2:{
		_numberOfElements[2]++;
		shared_ptr<LongRidgeHill> lrg_Ptr = make_shared<LongRidgeHill>(pos,scale,_numberOfElements[2]);
		_lacze.DodajNazwePliku(lrg_Ptr->takeFileName_finalFig().c_str());
		sceneObjectPtr = lrg_Ptr;
		addElementToList(sceneObjectPtr);
		break;
	}
	case 3:{
		_numberOfElements[0]++;
		shared_ptr<Plateau> plt_Ptr = make_shared<Plateau>(pos,scale,_numberOfElements[0]);
		_lacze.DodajNazwePliku(plt_Ptr->takeFileName_finalFig().c_str());
		sceneObjectPtr = plt_Ptr;
		addElementToList(sceneObjectPtr);
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
	for(const shared_ptr<SceneObject>& obj : _lstOfObjects){  			
		cout<<++k<<" - ";
		cout<<obj->getPosition()<<obj->getType()<<endl;
	}
	cout<<"Podaj numer elementu> ";
	cin>>elemNumber;
	list<shared_ptr<SceneObject>>::iterator iter = _lstOfObjects.begin();
	advance(iter, (elemNumber-1));						//przesuwa wskaźnik "elemNumber-1" razy 
	shared_ptr<SceneObject> fig = *iter;				//kopiuje wskaznik na ten element zeby dostac dostep do jego nazwy pliku
	_lstOfObjects.erase(iter);							//usuwa element z listy
	_lacze.UsunNazwePliku(fig->takeFileName_finalFig().c_str());	//usuwa plik tego elementu z lącza
	_lacze.Rysuj();
	cout<<"Element poprawnie usunięty"<<endl;
}


/**
 * @brief Funkcja sprawdza czy podany w argumencie dron koliduje z którymś elementem sceny. 
 * 
 * @param[in] drone_Ptr Wskaźnik na drona, dla którego sprawdzamy czy jego aktualne położenie jest zajęte.
 * @retval 1 Jeśli miejsce jest zajęte.
 * @retval 2 Jeśli miejsce jest wolne.
 * @retval 3 Jeśli miejsce zajmuje płaskowyż.
 */
int Scene::checkIfPlaceIsOccupied(const shared_ptr<Drone> &drone_Ptr) const {
	for(const shared_ptr<SceneObject> &Obj : _lstOfObjects){
		if(Obj==drone_Ptr){ continue; }
		//Jeśli nieprawda że miejsce jest wolne to zwróć true
		if( !Obj->checkIfPlaceIsAvaliable( drone_Ptr->getPosition(), drone_Ptr->getRadius() )) { 
			if(strcmp(Obj->getType(), "Plaskowyz") == 0){
				cout<<":)  Wykryto element powierzchni:"<< Obj->getType() <<endl<<endl<<endl<<endl;
				return 3;

			}
			cout<<":(  Ladowisko niedostepne!"<<endl;
			cout<<":(  Wykryto element powierzchni:"<< Obj->getType() <<endl<<"Lot zostal wydluzony"<<endl<<endl<<endl<<endl;
			return 1; 
		}
	}
	return 2;
}