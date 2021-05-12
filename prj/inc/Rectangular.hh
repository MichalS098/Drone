#pragma once

#include "Vector.hh"
#include "Matrix.hh"
#include "GeometricFigure.hh"
#include <iostream>
#include <initializer_list>
#include <iomanip>
#include <cmath>


/**
 * @brief Klasa opisuje prostopadloscian
 * 
 * 		  Klasa opisuje prostopadłościan przy pomocy 8 wektorow wodzacych
 * 		  oraz kata orientacji
 */
class Rectangular: public GeometricFigure{
	Vector<3> apex[8];		//array of vectors that follows every apex of Rectangular
	Vector<3> position;
    double orientationAngle;
	unsigned int rectSize;	//size is amount of vectors
public:
	Rectangular();
  	Rectangular(std::initializer_list<Vector<3>> lst);

	int size() const {return rectSize;}
	/**
	 * @brief Przeciazenia operatorow funkcyjnych pozwalajace na dostep
	 * 		  do wierzcholkow prostokata, na ich odczyt oraz zmiane przez referencje
	 * @param vertex, numer wierzcholka prostokata musi byc wiekszy od zera
	 * @return Vector<3>, zwraca wierzcholek prostokata w postaci wektora wodzacego 
	 */
  	Vector<3> operator()(int vertex) const {if(vertex<0){std::exit(1);} return apex[vertex];}
  	Vector<3>& operator()(int vertex) {if(vertex<0){std::exit(1);} return apex[vertex];}
	void operator=(const Rectangular& rect2);

  	void shiftByVector(const Vector<3>& v);
  	void rotateAroundCenter(const Matrix<3>& rotationMatrix);
  	double lenghtOfSide(int fv, int sv) const;

	Vector<3> transfToParentCoordSys(const Vector<3>& apex) const;
};

Rectangular makeCuboid(Vector<3> v, double dx, double dy, double dz);

std::ostream& operator<<(std::ostream& os, const Rectangular& rect);

