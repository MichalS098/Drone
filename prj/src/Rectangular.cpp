#include "Rectangular.hh"
#include <iostream>
#include <iomanip>

using namespace std;

/**
 * @brief Podstawowy konstruktor obiektu Rectangular
 */
Rectangular::Rectangular(): rectSize{8}, orientationAngle{0}{
    for(Vector<3>& vec: apex){
        for(int i=0; i<3; ++i){
            vec[i]=0;
        }
    }
    position=apex[0];
}

/**
 * @brief Konstruktor prostopadloscianu, wymaga podania wektorow, z ktorych kazdy
 *        wskazuje na jeden wierzcholek prostokata/prostopadloscianu
 * @param lst lista zawierajaca wektory wodzace prostokata/prostopadloscianu
 */
Rectangular::Rectangular(std::initializer_list<Vector<3>> lst): rectSize{8}, orientationAngle{0}{
    if(lst.size()!=8) throw invalid_argument("Invalid rectangular size, number of vectors should be 8");
    copy(lst.begin(), lst.end(), apex);
    position=apex[0];
}

/**
 * @brief Przeciazenie operatora przypisania prostokata 
 * @param rect2 prostokat ktory przypisujemy
 */
void Rectangular::operator=(const Rectangular& rect2){
    for(int i=0; i<size(); ++i){
        apex[i]=rect2(i);
    }
}

/**
 * @brief Funkcja przesuwajaca prostokat/prostopadloscian o podany wektor
 * @param v, wektor przesuniecia, jego wielkosc musi odpowiadac wielkosci wektorow z ktorych sklada
 *           sie prostokat/prostopadloscian
 */
void Rectangular::shiftByVector(const Vector<3>& v){
    for(int i=0; i<size(); ++i){
        apex[i]=apex[i]+v;
    }
}

/**
 * @brief Funkcja obracajaca prostopadloscian o macierz obrotu 
 * podana jako argument
 * @param rotationMatrix macierz obrotu przez ktora przemnozymy kazdy z wektorow prostopadloscianu
 */
void Rectangular::rotateAroundCenter(const Matrix<3>& rotationMatrix){
    for(int i=0; i<size(); i++){
        apex[i]=rotationMatrix*apex[i];
    }
}

/**
 * @brief Funkcja zwracajaca dlugosc boku okreslonego przez podanie dwoch wierzcholkow prostopadloscianu
 *        
 * @param firstVertex liczba wskazujaca numer pierwszego wierzcholka 
 * @param secondVertex liczba wskazujaca numer drugiego wierzcholka
 * @return double, wartosc zwracana jest dlugoscia boku  
 */
double Rectangular::lenghtOfSide(int firstVertex, int secondVertex) const{
    if((firstVertex<0 || secondVertex<0) || (firstVertex>=8 || secondVertex>=8)) throw invalid_argument("Invalid vertex number, only available : (0-7)");
    double lenght=sqrt(pow(abs(apex[firstVertex][0]-apex[secondVertex][0]),2)+
                       pow(abs(apex[firstVertex][1]-apex[secondVertex][1]),2)+
                       pow(abs(apex[firstVertex][2]-apex[secondVertex][2]),2));
    return lenght;
}

/**
 * @brief Funkcja wypisuje wspolrzedne wierzcholkow prostopadloscianu na podany 
 *        strumien os.
 * @param os strumien na ktory zostana wyslane wspolrzedne
 * @param rect Obiekt typu prostopadloscian ktory wysylamy na strumien
 * @return ostream& 
 */
ostream& operator<<(ostream& os, const Rectangular& rect){
    for(int i=0; i<rect.size(); ++i){
        os<<rect(i)<<endl;
        if(i%2!=0){
            os<<endl;
        }
    }
    return os;
}

/**
 * @brief Funkcja tworzaca prostopadloscian poprzez podanie wektora polozenia jednego
 *        z wierzcholkow oraz podanie dlugosci trzech krawedzi
 * @param position Wektor polozenia lewego dolnego wierzcholka
 * @param dx Dlugosc krawedzi x 
 * @param dy Dlugosc krawedzi y
 * @param dz Dlugosc krawedzi z
 * @return Rectangular zwracany jest prostopadloscian 
 */
Rectangular makeCuboid(Vector<3> v, double dx, double dy, double dz){
    double x{v[0]}, y{v[1]}, z{v[2]};
    Vector<3> vecArray[8]={{x,y,z},{x+dx,y,z},{x,y+dy,z},{x+dx,y+dy,z},{x,y+dy,z+dz},{x+dx,+y+dy,z+dz},{x,y,z+dz},{x+dx,y,z+dz}};
    Rectangular rect;
    int i=0;
    for(const Vector<3>& vec : vecArray){
        rect(i++)=vec;
    }
    return rect;
}