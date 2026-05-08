#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

class Menu {
public:
    Menu(sf::RenderWindow& v);
    ~Menu();
    bool mostrar();
    std::string obtenerNombre();

private:
    sf::RenderWindow& _ventana;
    sf::Font _fuente;
    sf::Text _titulo, _jugar, _estadisticas, _reglas, _creditos, _salir;
    sf::Texture _texturaFondoMenu, _texturaFondoNombre;
    sf::Sprite _spriteFondoMenu, _spriteFondoNombre;
    std::string _nombreJugador;
    int _opcion = 0;
};