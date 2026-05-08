#pragma once
#include <SFML/Graphics.hpp>
#include "Puntos.hpp"

class Estadisticas {
public:
    Estadisticas(sf::RenderWindow& ventana, const sf::Sprite& fondo, const sf::Font& fuente);
    void mostrar();

private:
    sf::RenderWindow& _ventana;
    sf::Sprite _fondo;
    sf::Font _fuente;
};