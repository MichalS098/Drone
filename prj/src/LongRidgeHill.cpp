#include <fstream>
#include <iostream>
#include <cmath>
#include "LongRidgeHill.hh"
#define AMOUNT_OF_VERTICES 4    //wierzchołki
#define AMOUNT_OF_EDGES 5       //krawędzie

using namespace std;


/**
 * @brief Konstruktor tworzy góre z długą granią.
 * 
 * 
 * Funkcja tworzy góre z długą granią, obliczając nowe współrzędne na podstawie 
 * współrzędnych z pliku wzorcowego i zapisuje je do pliku roboczego. 
 * @param[in] pos Wektor położenia początkowego (definiowany jako wektor
 *                przesunięcia wzorcowego prostopadłościanu)
 * @param[in] scale Wektor skali figury.
 * @return Zwraca true jeśli udało się otworzyć pliki i stworzyć płaskowyż
 *         lub false jeśli operacja się nie powiodła.
 */
LongRidgeHill::LongRidgeHill(const Vector<3>& pos, const Vector<3>& scale, string refFile, string finalFile){
    this->enterFileName_finalFig(finalFile);
    this->enterFileName_refFig(refFile);
    this->setScale(scale);
    _position={pos};
    ifstream refFileStream{this->takeFileName_refFig()};
    ofstream finalFileStream{this->takeFileName_finalFig()};
    if (!refFileStream.is_open()) {
        cerr << ":( Niepowiodlo sie otwarcie pliku: " << this->takeFileName_refFig() << endl;
        exit(1);
    }
    if (!finalFileStream.is_open()) {
        cerr << ":( Niepowiodlo sie otwarcie pliku: " << this->takeFileName_finalFig() << endl;
        exit(1);
    }

    Vector<3> apex;
    for (unsigned int edge = 0; edge < AMOUNT_OF_EDGES; ++edge) {
        for (unsigned int vertice = 0; vertice < AMOUNT_OF_VERTICES; ++vertice) {
            refFileStream >> apex;
            //deformacja
            if (apex[2] >= 0.5) {
	            if (apex[0] < 0) apex[0] = fabs(apex[0]);
            }
            // transformacja wierzcholka 
            if (refFileStream.fail()) exit(1);
            for(unsigned int i=0; i<3; ++i){
                apex[i]=apex[i]*scale[i];
            }
            apex=apex+pos;    
            finalFileStream << apex << endl;
            if (finalFileStream.fail()) exit(1);
        }
        finalFileStream << endl; // Dodatkowa linia oddzielająca krawędzie
    }
}
