#pragma once
#include <SFML/Graphics.hpp>

class Reglas {
public:
    Reglas(sf::RenderWindow& ventana, const sf::Sprite& fondo, const sf::Font& fuente);
    void mostrar();

private:
    sf::RenderWindow& _ventana;
    sf::Sprite _fondo;
    sf::Font _fuente;
    float _desplazamientoY = 0.f;
};