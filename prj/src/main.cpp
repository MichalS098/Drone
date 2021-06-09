#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cassert>
#include <unistd.h>
#include "lacze_do_gnuplota.hh"
#include "Drone.hh"
#include "Scene.hh"
#include "UserInterface.hh"

using namespace std;

template<unsigned int T>
long int Vector<T>::VECTOR_OBJECT_COUNTER=0;
template<unsigned int T> 
unsigned long int Vector<T>::VECTOR_OBJECT_SUM=0;
void printNumberOfVectors();

int main(){
    PzG::LaczeDoGNUPlota  Lacze;
    bool endOfProgram=false;
	char userInput=' ';
	Lacze.ZmienTrybRys(PzG::TR_3D);
    Lacze.Inicjalizuj();  // Tutaj startuje gnuplot.
  	Lacze.UstawZakresX(0, 200);
  	Lacze.UstawZakresY(0, 200);
  	Lacze.UstawZakresZ(0, 120);
  	Lacze.UstawRotacjeXZ(64,65); // Tutaj ustawiany jest widok
	Lacze.DodajNazwePliku("bryly_wzorcowe/plaszczyzna.dat");

	Scene sceneOfDrones(Lacze); //inicjalizacja sceny
	Lacze.Rysuj(); //tutaj powinno pojawić się okienko GNUPlot'a

	cout<<endl;
	sceneOfDrones.printPositionOfActiveDrone();
	printMenu();
	printNumberOfVectors();
	while(endOfProgram!=true){
		sceneOfDrones.printPositionOfActiveDrone();
		printNumberOfVectors();
		cout<<"\tTwoj wybor, m - menu > ";
		cin>>userInput;
		switch (userInput){
			case 'a':{
				sceneOfDrones.takeActiveDrone();		
				break;
			}
			case 'p':{
				sceneOfDrones.droneFlightAnimation();
				break;
			}
			case 'd':{
				additionOfNewElement(sceneOfDrones);
				break;
			}
			case 'u':{
				sceneOfDrones.deleteElement();
				break;
			}
			case 'm':{
				printMenu();
				break;
			}
			case 'k':{
				endOfProgram=true;
				cout<<"Koniec dzialania programu DragonFly"<<endl<<endl;
				break;
			}
			default:{
				cerr<<"Nieznana opcja, sprobuje jeszcze raz."<<endl;
				printMenu();
				break;
			}
		}
	}
	return 0;
}


/**
 * @brief Funkcja wyswietla na stdout ilosc aktualnie 
 * 		  istniejących obiektów wektor3d oraz ich całkowitą ilość
 */
void printNumberOfVectors(){
	cout<<"\tAktualna ilość obiektów Vector<3>: "<<Vector<3>::VECTOR_OBJECT_COUNTER<<endl;
	cout<<"\tIlość stworzonych obiektów Vector<3>: "<<Vector<3>::VECTOR_OBJECT_SUM<<endl<<endl;
}