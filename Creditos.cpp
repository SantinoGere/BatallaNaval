#include "Creditos.hpp"

Creditos::Creditos(sf::RenderWindow& ventana, const sf::Sprite& fondo, const sf::Font& fuente)
    : _ventana(ventana), _fondo(fondo), _fuente(fuente) {
}

void Creditos::mostrar() {
    sf::Text titulo, texto;
    titulo.setFont(_fuente);
    titulo.setString("CREDITOS");
    titulo.setCharacterSize(50);
    titulo.setFillColor(sf::Color::Yellow);
    titulo.setPosition(450, 100);

    texto.setFont(_fuente);
    texto.setCharacterSize(28);
    texto.setFillColor(sf::Color::White);
    texto.setPosition(200, 250);
    texto.setString(
        "Juego inspirado en el clasico 'Batalla Naval'.\n\n"
        "Realizado por:\n\n"
        "Chaile Jorge Ariel\n"
        "Silvero Milena Ayelen\n"
        "Colombo Carmela\n"
        "Santino Gerevini Navarro\n\n"
        "Tecnicatura en Programacion - Proyecto 'Batalla Estelar'\n"
        "Año 2025\n\n"
        "Presiona ESC o ENTER para volver al menu."
    );

    while (_ventana.isOpen()) {
        sf::Event evento;
        while (_ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                _ventana.close();
            }
            if (evento.type == sf::Event::KeyPressed &&
                (evento.key.code == sf::Keyboard::Escape || evento.key.code == sf::Keyboard::Enter)) {
                return;
            }
        }

        _ventana.clear();
        _ventana.draw(_fondo);
        _ventana.draw(titulo);
        _ventana.draw(texto);
        _ventana.display();
    }
}