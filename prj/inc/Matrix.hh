#pragma once

#include <iostream>
#include <initializer_list>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include "Vector.hh"

/**
 * @brief Klasa Matrix reprezentuje macierz kwadratowa, ktorej wielkosc podajemy przy 
 * 	      deklaracji obiektu.
 * @tparam T wielkość macierzy kwadratowej.
 */
template<unsigned int T>
class Matrix{
private:
	unsigned int matrixSize={T};
  	double elem[T][T];			
public:
	Matrix();
  	Matrix(const std::initializer_list<double>& lst);
  	Matrix(const std::initializer_list<Vector<T>>& lst);
	
	/**
	 * @brief Przeciazenie operatora funkcyjnego w celu dostepu do elementow macierzy
	 * 		  pozwala na odczyt oraz na zmiane elementow poprzez referencje
	 * @param row numer wiersza macierzy, musi byc mniejszy od wielkosci macierzy i nie moze byc ujemny
	 * @param column numer kolumny macierzy, musi byc mniejszy od wielkosci macierzy i nie moze byc ujemny
	 * @return T wielkosc macierzy
	 */
	double operator()(unsigned int row, unsigned int column) const { if(row>=matrixSize || column>=matrixSize){std::exit(1);} return elem[row][column];}
  	double& operator()(unsigned int row, unsigned int column) { if(row>=matrixSize || column>=matrixSize){std::exit(1);} return elem[row][column];}
	
	/**
	 * @brief Funkcja zwraca wielkosc macierzy w postaci liczby int
	 * 		  (np dla macierzy 2x2 zwroci 2).
	 */
	int size() const {return static_cast<int>(matrixSize);}

  	Matrix<T> operator+(const Matrix<T>& m) const;
  	Matrix<T> operator-(const Matrix<T>& m) const;
  	Matrix<T> operator*(const Matrix<T>& m) const;
  	Vector<T> operator*(const Vector<T>& v) const;
  	Matrix<T> operator*(const double& multiplier) const; 
	void operator=(const Matrix<T>& m);
	bool operator==(const Matrix<T>& m);
	bool operator!=(const Matrix<T>& m);
};

template<unsigned int T>
void makeRotationMatrix(char axis, double angleInDegrees, Matrix<T>& rotationMatrix);

template<unsigned int T>
std::ostream& operator<<(std::ostream &os, const Matrix<T> &m);

constexpr long double number_pi=3.14159265358979323;		//number pi used to convert degree angle to radian angle

#include "../src/Matrix.ipp" //including implementation file