#pragma once


/**
 * @brief Podstawowy konstruktor macierzy 
 * @tparam T, wielkosc macierzy
 */
template<unsigned int T>
Matrix<T>::Matrix(){
    for(int i=0; i<size(); ++i){
        for(int j=0; j<size(); ++j){
            elem[i][j]=0;
        }
    }
}

/**
 * @brief Konstruktor inicjalizujacy macierz podanymi elementami w liscie.
 *        Elementy nalezy podawac od gory do dolu kolejno po kolumnach.
 * @tparam T, wielkosc macierzy
 * @param lst Lista elementow typu double ktorymi zainicjalizowana zostanie macierz, liczba 
 *            elementow w liscie powinna rownac sie kwadratowi wielkosci macierzy  
 */
template<unsigned int T>
Matrix<T>::Matrix(const std::initializer_list<double>& lst){
    if(lst.size()!=std::pow(size(),2)) throw std::invalid_argument("Incorrect matrix size");
    int k=0;
    for(int i=0; i<size(); ++i){
        for(int j=0; j<size(); ++j){
            elem[j][i]=lst.begin()[k++];
        }
    }
}

/**
 * @brief Konstruktor macierzy, inicjalizujacy macierz wektorami zawierajacymi 
 *        elementy typu double.
 * @tparam T, wielkosc macierzy i wektora (dimension) 
 * @param lst Lista zawierajaca wektory z elementami typu double, wielkosc listy powinna byc rowna wielkosci macierzy
 */
template<unsigned int T>                                          
Matrix<T>::Matrix(const std::initializer_list<Vector<T>>& lst){
    if(lst.size()!=size()) throw std::invalid_argument("Incorrect amount of vectors");
    for(int i=0; i<size(); ++i){
        for(int j=0; j<size(); ++j){
            elem[j][i]=(lst.begin()[i])[j];
        }
    }    
}

/**
 * @brief Operacja dodawania macierzy.
 * @tparam T, wielkosc macierzy 
 * @param m Macierz do dodania
 * @return Matrix<T>, zwraca macierz bedaca suma dwoch macierzy 
 */
template<unsigned int T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& m) const{
    if(size()!=m.size()) throw std::invalid_argument("Cannot add matrixes with different sizes");
    Matrix<T> sum;        //matrix sum with the same size as matrix m
    for (int i=0; i<size(); ++i){
        for (int j=0; i<size(); ++i){
            sum(i,j)=elem[i][j]+m(i,j);
        }
    }
    return sum;
}

/**
 * @brief Operacja odejmowania macierzy
 * @param m Odjemnik
 * @return Matrix<T>, zwraca macierz bedaca roznica dwoch macierzy 
 */
template<unsigned int T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& m) const{
    if(size()!=m.size()) throw std::invalid_argument("Cannot subtract matrixes with different sizes");
    Matrix<T> result;        //matrix with the same size as matrix m
    for (int i=0; i<size(); ++i){
        for (int j=0; i<size(); ++i){
            result(i,j)=elem[i][j]-m(i,j);
        }
    }
    return result;
}

/**
 * @brief Operacja mnozenia macierzy przez macierz
 * @tparam T, wielkosc macierzy
 * @param m Druga macierz przez ktora mnozymy, musi ona byc tej samej wielkosci
 *          co macierz obiektu
 * @return Matrix<T>, zwracana jest macierz bedaca iloczynem dwoch macierzy
 */
template<unsigned int T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& m) const{
    if(size()!=m.size()) throw std::invalid_argument("Cannot multiply matrixes with different sizes");
    Matrix<T> result;
        for(int i=0; i<size(); ++i){
            for(int j=0; j<size(); ++j){
                for(int k=0; k<size(); ++k){
                    result(i,j)+=elem[i][k]*m(k,j);
                }
            }
        }
    return result;
}

/**
 * @brief Operacja mnozenia macierzy przez wektor
 * @tparam T, wielkosc macierzy i wektora (musi byc oczywiscie ten sam)
 * @param v wektor elementow typu double, jego wymiar musi odpowiadac wymiarowi macierzy
 * @return Vector<T> zwracany jest wektor elementow typu double, jego wielkosc jest rowna wektorowi v
 */
template<unsigned int T>
Vector<T> Matrix<T>::operator*(const Vector<T>& v) const{
    if(this->size()!=v.size()) throw std::invalid_argument("Size of vector is inapropriate for multiplying");
    Vector<T> result;
        for(int i=0; i<this->size(); ++i){
            for(int j=0; j<this->size(); ++j){
                result[i]=result[i]+(elem[i][j]*v[j]);
            }
        }
    return result;
}

/**
 * @brief Operacja mnozenia macierzy elementow typu double przez liczbe double
 * @tparam T, wielkosc macierzy
 * @param multiplier liczba przez ktora przemnozymy elementy macierzy
 * @return Matrix<T>, zwracana jest macierz po przemnozeniu jej elementow przez mnoznik
 */
template<unsigned int T>
Matrix<T> Matrix<T>::operator*(const double& multiplier) const{
    Matrix<T> result;
        for(int i=0; i<size(); ++i){
            for(int j=0; j<size(); ++j){
                result(i,j)=elem[i][j]*multiplier;
            } 
        }  
    return result;
}

/**
 * @brief Operacja przypisania dla macierzy, kazdy element macierzy m przypisuje
 *        do macierzy obiektu, macierze musza miec ten sam rozmiar
 * @tparam T, wielkosc macierzy
 * @param m macierz pierwotna ktora kopiujemy
 */
template<unsigned int T>
void Matrix<T>::operator=(const Matrix<T>& m){
    if(size()!=m.size()) throw std::invalid_argument("Cannot assing matrixes with different sizes");
    for(int i=0; i<size(); ++i){
        for(int j=0; j<size(); ++j){
            elem[i][j]=m(i,j);
        }
    }
}

/**
 * @brief Funkcja tworząca macierz obrotu.
 * 
 * Funkcja tworzy macierz obrotu na podstawie osi wokół której ma obrócić oraz
 * kąta obrotu. Funkcja potrafi tworzyć macierz obrotu wielkości 3x3 pozwalających na obrót
 * wokół osi (x,y,z) oraz również potrafi stworzyć macierz obrotu wielkości 2x2, wtedy jako 
 * oś czyli "axis" należy podać 'o' a macierz podana jako argument powinna być macierzą wielkości 2x2.
 * @tparam T 
 * @param axis - oś wokół której macierz ma obracać, dostępne opcje : (o,x,y,z)
 * @param angleInDegrees - kąt obrotu
 * @param rotationMatrix - macierz rotacji podana jako argument
 */
template<unsigned int T>
void makeRotationMatrix(char axis, double angleInDegrees, Matrix<T>& rotationMatrix){           
    double angleRad = angleInDegrees*(number_pi/180);
    if(axis=='o' && T==2){
	    rotationMatrix = Matrix<T>({std::cos(angleRad), std::sin(angleRad), -std::sin(angleRad), std::cos(angleRad)}); 
    }
    else if(axis=='x' && T==3){
        rotationMatrix = Matrix<T>({1, 0, 0, 0, std::cos(angleRad), std::sin(angleRad), 0, -std::sin(angleRad), std::cos(angleRad)}); 
    }
    else if(axis=='y' && T==3){
        rotationMatrix = Matrix<T>({std::cos(angleRad), 0, -std::sin(angleRad), 0, 1, 0, std::sin(angleRad), 0, std::cos(angleRad)}); 
    }
    else if(axis=='z' && T==3){
        rotationMatrix = Matrix<T>({std::cos(angleRad), std::sin(angleRad), 0, -std::sin(angleRad), std::cos(angleRad), 0, 0, 0, 1}); 
    }
    else{
        throw std::invalid_argument("unknown axis, there are only options(o,x,y,z), or unknown size for rotation matrix");
    }
}

/**
 * @brief Operacja wysylajaca macierz na podany strumien, wysylane sa 
 *        elementy z ktorych macierz sie sklada odpowiednio sformatowane
 * @tparam T wielkosc macierzy
 * @param os Strumien podany przez uzytkownika na ktory zostanie wyslana macierz
 * @param m Macierz ktora zostanie wyslana (wyswietlona w szczegolnosci dla stdout)
 * @return ostream&
 */
template<unsigned int T>
std::ostream& operator<<(std::ostream &os, const Matrix<T> &m){
    for(int i=0; i<m.size(); ++i){
        for(int j=0; j<m.size(); ++j){
            os<<std::setw(16)<<std::fixed<<std::setprecision(10)<<m(i,j);
        }
        os<<std::endl;
    }
    return os;
}

template<unsigned int T>
bool Matrix<T>::operator==(const Matrix<T>& m){
    for(int i=0; i<m.size(); ++i){
        for(int j=0; j<m.size(); ++j){
            if(elem[i][j]!=m(i,j)) return false;
        }
    }
    return true;
}

template<unsigned int T>
bool Matrix<T>::operator!=(const Matrix<T>& m){
    return !(*this == m);
}