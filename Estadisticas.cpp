#include "Estadisticas.hpp"
#include "Puntos.hpp"

Estadisticas::Estadisticas(sf::RenderWindow& ventana, const sf::Sprite& fondo, const sf::Font& fuente)
    : _ventana(ventana), _fondo(fondo), _fuente(fuente) {
}

void Estadisticas::mostrar() {
    GestorRanking gr;
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
        gr.mostrarRanking(_ventana, "", 0, _fuente);

        sf::Text volver;
        volver.setFont(_fuente);
        volver.setString("Presiona ESC o ENTER para volver al menu");
        volver.setCharacterSize(25);
        volver.setFillColor(sf::Color::White);
        volver.setPosition(300, 800);
        _ventana.draw(volver);
        _ventana.display();
    }
}