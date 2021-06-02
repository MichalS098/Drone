#pragma once
#include <string>
#include "GeometricFigure.hh"

/**
 * @brief Klasa LongRidgeHill definiuje góre z długą granią.
 * 
 * Klasa LongRidgeHill definiuje góre z długą granią, jako klasa pochodna klasy
 * GeometricFigure czyli figura geometryczna.
 */
class LongRidgeHill: public GeometricFigure{
public:
    // funkcja tworzy góre z długą granią
    LongRidgeHill(const Vector<3>& pos, const Vector<3>& scale, std::string refFile, std::string finalFile);
};