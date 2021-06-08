#include <fstream>
#include <iostream>
#include "Plateau.hh"
#define AMOUNT_OF_VERTICES 4    //wierzchołki
#define AMOUNT_OF_EDGES 5       //krawędzie
#define CUBE_SAMPLE_FILE "bryly_wzorcowe/szescian.dat"

using namespace std;

/**
 * @brief Konstruktor tworzy płaskowyż.
 * 
 * Funkcja tworzy płaskowyż, obliczając nowe współrzędne na podstawie 
 * współrzędnych z pliku wzorcowego i zapisuje je do pliku roboczego. 
 * @param[in] pos Wektor położenia początkowego (definiowany jako wektor
 *                przesunięcia wzorcowego prostopadłościanu)
 * @param[in] scale Wektor skali figury.
 * @param[in] id Numer id nowego elementu.
 */
Plateau::Plateau(const Vector<3>& pos, const Vector<3>& scale, unsigned int id): _position{pos}, _ID{id}{
    string finalFile=makeFilePlateau(id);
    this->enterFileName_finalFig(finalFile);
    this->enterFileName_refFig(CUBE_SAMPLE_FILE);
    this->setScale(scale);
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


/**
 * @brief Funkcja tworzy nazwe pliku dla nowego elementu.
 * 
 * Funkcja tworzy nazwe pliku dla nowego elementu na podstawie
 * ilości elementów tzn. numeru id.
 * @return string Zwracana nazwa nowego pliku dla elementu.
 */
string makeFilePlateau(unsigned int id){
	ostringstream nameStream;
    nameStream<<"dat/PlikWlasciwy_Plaskowyz"<<id<<".dat";
    return nameStream.str();
}
