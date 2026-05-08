#include "Reglas.hpp"

Reglas::Reglas(sf::RenderWindow& ventana, const sf::Sprite& fondo, const sf::Font& fuente)
    : _ventana(ventana), _fondo(fondo), _fuente(fuente) {
}

void Reglas::mostrar() {
    sf::Text titulo, texto;
    titulo.setFont(_fuente);
    titulo.setString("REGLAS DEL JUEGO");
    titulo.setCharacterSize(50);
    titulo.setFillColor(sf::Color::Yellow);
    titulo.setPosition(350, 30);

    texto.setFont(_fuente);
    texto.setCharacterSize(24);
    texto.setFillColor(sf::Color::White);
    texto.setPosition(100, 120);
    texto.setString(
        "• Cada jugador tiene un sector estelar de 10x10 donde coloca sus naves.\n"
        "• Las naves pueden variar en tamaño (2, 3, 4, 5 coordenadas espaciales).\n"
        "• Cada turno, el jugador elige una coordenada para atacar.\n\n"
        "1. Tipos de disparos especiales:\n"
        " - Radar: revela si hay una nave en un area pequeña (3x3).\n"
        " - Bomba: afecta un area de 2x2.\n"
        " - Misil: dispara en linea recta horizontal o vertical.\n\n"
        "2. Naves con habilidades:\n"
        " Algunas naves pueden moverse una casilla por turno o repararse una vez.\n\n"
        "3. Sistema de puntos y niveles:\n"
        " No solo gana quien hunda todas las naves,\n"
        " tambien se lleva puntaje por precision, combos de aciertos y uso eficiente de disparos.\n\n"
        "4. Eventos aleatorios en el sector estelar:\n"
        " - Tormentas que bloquean coordenadas espaciales.\n"
        " - Minas ocultas que dañan al jugador que dispare alli.\n"
        " - Bonos (municion extra, disparo gratis, etc.).\n\n"
        "5. Historial de jugadas:\n"
        " Cada jugador puede consultar en pantalla un registro de aciertos y fallos.\n\n"
        "CONTROLES\n"
        "Menú principal:\n"
        " Letras W S A D para moverse por las opciones.\n"
        " Enter para seleccionar.\n\n"
        "Colocacion de naves:\n"
        " Letras W S A D para mover la nave.\n"
        " Tecla R para rotar.\n"
        " Tecla Enter para confirmar posicion.\n\n"
        "Juego en sector estelar:\n"
        " Letras W S A D para moverse por las coordenadas espaciales.\n"
        " Tecla Enter para disparar.\n"
        " Teclas 1, 2, 3 para seleccionar disparo especial (Radar, Bomba, Misil).\n\n"
        "Otros controles:\n"
        " Tecla P para pausar.\n"
        " Tecla H para ver historial de disparos.\n"
        " Tecla Q para salir de la partida.\n\n"
        "Presiona ESC o ENTER para volver al menu."
    );

    while (_ventana.isOpen()) {
        sf::Event evento;
        while (_ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                _ventana.close();
            }
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Escape || evento.key.code == sf::Keyboard::Enter) {
                    return;
                }
                if (evento.key.code == sf::Keyboard::Down) {
                    _desplazamientoY -= 20.f;
                }
                if (evento.key.code == sf::Keyboard::Up) {
                    _desplazamientoY += 20.f;
                }
            }
        }

        texto.setPosition(100, 120 + _desplazamientoY);
        _ventana.clear();
        _ventana.draw(_fondo);
        _ventana.draw(titulo);
        _ventana.draw(texto);
        _ventana.display();
    }
}