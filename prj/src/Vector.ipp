#pragma once
//
//	Implementation file for Vector template
//


/**
 * @brief Konstruktor podstawowy wymagajacy podania wielkosci
 * 		  wektora jako parametru
 * @tparam T, wielkosc wektora
 */
template<unsigned int T>
Vector<T>::Vector(){
	for(int i=0; i<size(); ++i){
		elem[i]=0;
	}
	VECTOR_OBJECT_COUNTER++;
	VECTOR_OBJECT_SUM++;
}

/**
 * @brief Konstruktor wektora pozwalajacy na inizjalizacje
 * 		  wektora lista inicjalizacyjna liczb typu double
 * @tparam T, wielkosc wektora
 * @param lst Lista inicjalizacyjna elementow typu double
 */
template<unsigned int T>
Vector<T>::Vector(const std::initializer_list<double>& lst){
	if(lst.size()!=static_cast<long unsigned int>(vecSize)) throw std::invalid_argument("Size of vector does not match size of initializing list");
	int i=0;
	for(const double& x : lst){
		elem[i++]=x;
	}
	VECTOR_OBJECT_COUNTER++;
	VECTOR_OBJECT_SUM++;
}

/**
 * @brief Destruktor obiektu Vector, stworzony jedynie na potrzeby zadania do zliczania
 * 		  ilości stworzonych obiektów typu wektor.
 */
template<unsigned int T>
Vector<T>::~Vector(){
	VECTOR_OBJECT_COUNTER--;
}

/**
 * @brief Podstawowa operacja dodawania dwoch wektorow
 * 		  tworzy ona trzeci wektor bedacy suma dwoch podanych 
 * @tparam T, wielkosc wektorow
 * @param v wektor do dodania
 * @return Vector<T>, wartosc zwracana jest wektorem, o tej samej wielkosci co elementy dodawania 
 */
template<unsigned int T>
Vector<T> Vector<T>::operator+(const Vector<T>& v) const{
	if(v.size()!=size()) throw std::invalid_argument("Cannot add vectors with different sizes");
	Vector<T> result;
	for(int i=0; i<size(); ++i){
		result[i]=elem[i]+v[i];
	} 
	return result;
}

/**
 * @brief Operacja odejmowania wektorow, kazdy element wektora 
 * 		  jest odejmowany po kolei
 * @tparam T, wielkosc wektora 
 * @param v odjemnik
 * @return Vector<T>, wartosc zwracana jest typu Vector, jest to 
 * 		wektor bedacy roznica dwoch wektorow 
 */
template<unsigned int T>
Vector<T> Vector<T>::operator-(const Vector<T>& v) const{
	if(v.size()!=size()) throw std::invalid_argument("Cannot subtract vectors with different sizes");
	Vector<T> result;
	for(int i=0; i<size(); ++i){
		result[i]=elem[i]-v[i];
	} 
	return result;
}

/**
 * @brief Operacja iloczynu skalarnego dla dwoch wektorow
 * 		  kazdy element wektora jest po kolei mnozony
 * @tparam T, wielkosc wektora
 * @param v wektor przez ktory mnozymy
 * @return double zwracana jest liczba double
 */
template<unsigned int T>
double Vector<T>::operator*(const Vector<T>& v) const{
	if(size()!=v.size()) throw std::invalid_argument("Cannot multiply vectors with different sizes");
	double result;
	for(int i=0; i<size(); ++i){
		result+=elem[i]*v[i];
	} 
	return result;
}

/**
 * @brief Przeciazenie oparatora & do iloczynu wektorowego dla dwoch wektorow
 * @tparam T wielkosc wektorow
 * @param v wektor ktory mnozymy
 * @return Vector<T> ,zwraca wektor bedacy iloczynem wektorowym dwoch wektorow 
 */
template<unsigned int T>
Vector<T> Vector<T>::operator&(const Vector<T>& v) const{			
	if(T==2) throw std::invalid_argument("Vector product of 2 dim vectors is inappropriate");
	Vector<T> result;
	result[0]=elem[1]*v[2] - elem[2]*v[1];
	result[1]=elem[2]*v[0] - elem[0]*v[2];
	result[2]=elem[0]*v[1] - elem[1]*v[0];
	return result;
}

/**
 * @brief Operacja mnozenia wektora przez skalar
 * @tparam T, wielkosc wektora
 * @param multiplier Skalar bedacy mnoznikiem 
 * @return Vector<T>, zwracany jest wektor ktorego elementy zostaly 
 * 		   przemnozone przez skalar
 */
template<unsigned int T>
Vector<T> Vector<T>::operator*(const double& multiplier) const{
	Vector<T> result;
	for(int i=0; i<size(); ++i){
		result[i]=elem[i]*multiplier;
	}
	return result;
}

/**
 * @brief Operacja przypisania dla wektora
 * @tparam T, wielkosc wektora
 * @param v, wektor ktory przypisujemy do naszego obiektu  
 */
template<unsigned int T>
void Vector<T>::operator=(const Vector<T>& v){
	if(size()!=v.size()) throw std::invalid_argument("Cannot assing vector with different size");
	for(int i=0; i<size(); ++i){
		elem[i]=v[i];
	}
}

/**
 * @brief Funkcja przeciazajaca operator wczytywania ze strumienia do
 * 		  wektora, wczytuje sekwencje obiektow zapisuje do obiektu
 * @tparam T, wielkosc wektora
 * @param is strumien wejsciowy
 * @param v wektor do ktorego zapisujemy wspolrzedne
 * @return istream& 
 */
template<unsigned int T>
std::istream& operator>>(std::istream &is, Vector<T>& v){
	for(int i=0; i<v.size(); ++i){
		is>>v[i];
	}
	return is;
}

/**
 * @brief Operacja wysylajaca wektor(jego wspolrzedne) na podany strumien os 
 * @tparam T, wielkosc wektora
 * @param os strumien wyjsciowy
 * @param v wektor do wyswietlenia
 * @return ostream&, funkcja powinna poprawnie wyswietlic na strumieniu elementy wektora v
 */
template<unsigned int T>
std::ostream& operator<<(std::ostream &os, const Vector<T>& v){
	for(int i=0; i<v.size(); ++i){
		os << v[i] << " ";
	}
	return os;
}
