#pragma once

#include <iostream>
#include <initializer_list>
#include <iomanip>
/**
 * @brief Klasa Vector opisuje wektor roznowymiarowy bedacy tablica liczb double o wielkosci T, opisuje ona 
 *        wektor w sposob taki sam jak w algebrze.
 * @tparam T, wielkosc wektora
 */
template<unsigned int T>
class Vector{
private:
	unsigned int vecSize={T};
    double elem[T];         
public:
  	Vector();
  	Vector(const std::initializer_list<double>& lst);
	
  	/**
  	 * @brief Operator pozwalajacy na dostep do prywatnej czesci obiektu
	 * 
	 * 	  Operator pozwalajacy na dostep do prywatnej czesci obiektu
  	 *        czyli do tablicy z elementami wektora, pozwala na ich odczyt oraz 
  	 *        na dostep przez referencje
  	 * @param index Numer elementu wektora do ktorego chcemy sie odniesc 
  	 *              musi byc mniejszy od wielkosci wektora i nie byc ujemny
  	 * @return T, wielkosc wektora
  	 */
  	double operator[](unsigned int index) const { if(0>index || index>=vecSize){std::exit(1);} return elem[index];}
  	double& operator[](unsigned int index) { if(0>index || index>=vecSize){std::exit(1);} return elem[index];}

  	/**
  	 * @brief Zwraca wielkosc/wymiar wektora w postaci liczby int 
  	 */
  	int size() const {return static_cast<int>(vecSize);}

  	Vector<T> operator+(const Vector<T>& v) const;
  	Vector<T> operator-(const Vector<T>& v) const;
  	double operator*(const Vector<T>& v) const;
  	Vector<T> operator&(const Vector<T>& v) const;   //overloading operator & for vector product
	Vector<T> operator*(const double& multiplier) const;

  	void operator=(const Vector<T>& v);
};

template<unsigned int T>
std::istream& operator>>(std::istream &is, Vector<T>& v);

template<unsigned int T>
std::ostream& operator<<(std::ostream &os, const Vector<T>& v);

#include "../src/Vector.ipp"		//including implementation file