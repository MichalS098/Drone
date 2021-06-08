#include <iostream>
#include "UserInterface.hh"
#include "Vector.hh"

using namespace std;

/**
 * @brief Funkcja wypisuje na stdout menu dla użytkownika
 * 
 */
void printMenu(){
	cout<<"\ta - wybierz aktywnego drona"<<endl;
	cout<<"\tp - zadaj parametry przelotu"<<endl;
	cout<<"\td - dodaj element powierzchni"<<endl;
	cout<<"\tu - usun element powierzchni"<<endl;
	cout<<"\tm - wyswietl menu"<<endl<<endl;
	cout<<"\tk - zakoncz dzialanie programu"<<endl<<endl;
}


/**
 * @brief Funkcja pyta użytkownika o położenie oraz skale nowego elementu, oraz jaki
 * 		  to ma być element i wywołuje funkcje tworzącą nowy element.
 * @param scene Referencja do sceny zawierającej odpowiednią funkcję dodającą element.
 */
void additionOfNewElement(Scene& scene){
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
	scene.makeNewElement(position,scale,elementType);
}