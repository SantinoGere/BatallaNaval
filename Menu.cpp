#include "Menu.hpp"
#include "Estadisticas.hpp"
#include "Reglas.hpp"
#include "Creditos.hpp"
#include <iostream>
#include <string>
#include <SFML/Audio.hpp>

sf::Music musicaMenu; // DEFINICIÓN GLOBAL

Menu::Menu(sf::RenderWindow& v) : _ventana(v) {
    if (!_fuente.loadFromFile("arial.ttf.ttf")) {
        std::cerr << "Error: arial.ttf no encontrado.\n";
    }

    _titulo.setFont(_fuente);
    _titulo.setString("BATALLA ESTELAR");
    _titulo.setCharacterSize(60);
    _titulo.setFillColor(sf::Color::White);
    _titulo.setStyle(sf::Text::Bold);
    auto bounds = _titulo.getLocalBounds();
    _titulo.setOrigin(bounds.width / 2.f, 0);
    _titulo.setPosition(600, 200);

    _jugar.setFont(_fuente);
    _jugar.setString("JUGAR");
    _jugar.setCharacterSize(40);
    bounds = _jugar.getLocalBounds();
    _jugar.setOrigin(bounds.width / 2.f, 0);
    _jugar.setPosition(600, 380);

    _estadisticas.setFont(_fuente);
    _estadisticas.setString("ESTADISTICAS");
    _estadisticas.setCharacterSize(40);
    bounds = _estadisticas.getLocalBounds();
    _estadisticas.setOrigin(bounds.width / 2.f, 0);
    _estadisticas.setPosition(600, 440);

    _reglas.setFont(_fuente);
    _reglas.setString("REGLAS");
    _reglas.setCharacterSize(40);
    _reglas.setFillColor(sf::Color::White);
    _reglas.setPosition(530, 500);

    _creditos.setFont(_fuente);
    _creditos.setString("CREDITOS");
    _creditos.setCharacterSize(40);
    _creditos.setFillColor(sf::Color::White);
    _creditos.setPosition(500, 560);

    _salir.setFont(_fuente);
    _salir.setString("SALIR");
    _salir.setCharacterSize(40);
    bounds = _salir.getLocalBounds();
    _salir.setOrigin(bounds.width / 2.f, 0);
    _salir.setPosition(600, 620);

    if (!_texturaFondoMenu.loadFromFile("FondoMenu.png.png") ||
        !_texturaFondoNombre.loadFromFile("FondoNombre.png.png")) {
        std::cerr << "Error: Faltan fondos del menú.\n";
    }
    _spriteFondoMenu.setTexture(_texturaFondoMenu);
    _spriteFondoMenu.setScale(1200.f / _texturaFondoMenu.getSize().x,
        900.f / _texturaFondoMenu.getSize().y);
    _spriteFondoNombre.setTexture(_texturaFondoNombre);
    _spriteFondoNombre.setScale(1200.f / _texturaFondoNombre.getSize().x,
        900.f / _texturaFondoNombre.getSize().y);

    if (musicaMenu.openFromFile("musicaMenu.ogg.ogg")) {
        musicaMenu.setLoop(true);
        musicaMenu.setVolume(40.f);
        musicaMenu.play();
    }
}

Menu::~Menu() {
    musicaMenu.stop();
}

bool Menu::mostrar() {
    while (_ventana.isOpen()) {
        sf::Event evento;
        while (_ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) return false;
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Up || evento.key.code == sf::Keyboard::W)
                    _opcion = (_opcion - 1 + 5) % 5;
                if (evento.key.code == sf::Keyboard::Down || evento.key.code == sf::Keyboard::S)
                    _opcion = (_opcion + 1) % 5;
                if (evento.key.code == sf::Keyboard::Enter) {
                    if (_opcion == 0) return true;
                    if (_opcion == 1) { Estadisticas e(_ventana, _spriteFondoMenu, _fuente); e.mostrar(); }
                    if (_opcion == 2) { Reglas r(_ventana, _spriteFondoMenu, _fuente); r.mostrar(); }
                    if (_opcion == 3) { Creditos c(_ventana, _spriteFondoMenu, _fuente); c.mostrar(); }
                    if (_opcion == 4) return false;
                }
            }
        }

        _jugar.setFillColor(_opcion == 0 ? sf::Color::Yellow : sf::Color::White);
        _estadisticas.setFillColor(_opcion == 1 ? sf::Color::Yellow : sf::Color::White);
        _reglas.setFillColor(_opcion == 2 ? sf::Color::Yellow : sf::Color::White);
        _creditos.setFillColor(_opcion == 3 ? sf::Color::Yellow : sf::Color::White);
        _salir.setFillColor(_opcion == 4 ? sf::Color::Yellow : sf::Color::White);

        _ventana.clear();
        _ventana.draw(_spriteFondoMenu);
        _ventana.draw(_titulo);
        _ventana.draw(_jugar);
        _ventana.draw(_estadisticas);
        _ventana.draw(_reglas);
        _ventana.draw(_creditos);
        _ventana.draw(_salir);
        _ventana.display();
    }
    return false;
}

std::string Menu::obtenerNombre() {
    sf::Text textoInput, mensaje;
    textoInput.setFont(_fuente); textoInput.setCharacterSize(30); textoInput.setFillColor(sf::Color::White);
    mensaje.setFont(_fuente); mensaje.setString("INGRESE SU NOMBRE PILOTO:");
    mensaje.setCharacterSize(24); mensaje.setFillColor(sf::Color::Yellow);
    auto bounds = mensaje.getLocalBounds();
    mensaje.setOrigin(bounds.width / 2.f, 0);
    mensaje.setPosition(600, 350);
    _nombreJugador = "";

    while (_ventana.isOpen()) {
        sf::Event evento;
        while (_ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) { _ventana.close(); return ""; }
            if (evento.type == sf::Event::TextEntered && evento.text.unicode < 128) {
                if (evento.text.unicode == 8 && !_nombreJugador.empty()) _nombreJugador.pop_back();
                else if (evento.text.unicode == 13 && !_nombreJugador.empty()) return _nombreJugador;
                else if (evento.text.unicode >= 32 && _nombreJugador.length() < 15)
                    _nombreJugador += static_cast<char>(evento.text.unicode);
            }
        }

        _ventana.clear();
        _ventana.draw(_spriteFondoNombre);
        textoInput.setString(_nombreJugador + "_");
        bounds = textoInput.getLocalBounds();
        textoInput.setOrigin(bounds.width / 2.f, 0);
        textoInput.setPosition(600, 450);
        _ventana.draw(mensaje);
        _ventana.draw(textoInput);
        _ventana.display();
    }
    return _nombreJugador;
}