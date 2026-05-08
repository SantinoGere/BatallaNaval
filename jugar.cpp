#include "jugar.hpp"
#include "Naves.hpp"
#include "Enemigos.hpp"
#include "Puntos.hpp"
#include "PowerUp.hpp"
#include "EnemigoJefe.hpp"
#include <iostream>
#include <sstream>
#include <cmath>
#include <string>
#include <SFML/Audio.hpp>

// DECLARACIÓN EXTERNA
extern sf::Music musicaMenu;

void jugar(sf::RenderWindow& ventana, const std::string& nombreJugador) {
    musicaMenu.stop(); // DETENER MÚSICA DEL MENÚ

    sf::Font fuente;
    if (!fuente.loadFromFile("arial.ttf.ttf")) {
        std::cerr << "Advertencia: arial.ttf no encontrado.\n";
    }

    sf::Music musicaJugar;
    if (musicaJugar.openFromFile("musicaJugar.ogg.ogg")) {
        musicaJugar.setLoop(true);
        musicaJugar.setVolume(50.f);
        musicaJugar.play();
    }

    sf::Texture texNave, texEnemigo, texFondo;
    if (!texNave.loadFromFile("Nave.png.png") ||
        !texEnemigo.loadFromFile("NaveenemigaLV1.png.png") ||
        !texFondo.loadFromFile("Galaxy.png.png")) {
        std::cerr << "Error: Faltan texturas principales\n";
        return;
    }

    sf::Texture naveNivel3, naveNivel5, enemigoNivel4;
    naveNivel3.loadFromFile("Nave2.png.png");
    naveNivel5.loadFromFile("Nave3.png.png");
    enemigoNivel4.loadFromFile("NaveenemigaLV2.png.png");

    PowerUps powerups(ventana.getSize());
    sf::Sprite fondo(texFondo);
    fondo.setScale(1200.f / texFondo.getSize().x, 900.f / texFondo.getSize().y);

    sf::Text txtGameOver, txtPuntosFinales, txtReiniciar;
    txtGameOver.setFont(fuente); txtGameOver.setString("PERDISTE"); txtGameOver.setCharacterSize(60);
    txtGameOver.setFillColor(sf::Color::Red); txtGameOver.setPosition(600, 100);
    txtGameOver.setOrigin(txtGameOver.getLocalBounds().width / 2, txtGameOver.getLocalBounds().height / 2);
    txtPuntosFinales.setFont(fuente); txtPuntosFinales.setCharacterSize(40);
    txtPuntosFinales.setFillColor(sf::Color::Yellow); txtPuntosFinales.setPosition(600, 200);
    txtReiniciar.setFont(fuente); txtReiniciar.setString("Presiona ENTER para volver al menú");
    txtReiniciar.setCharacterSize(30); txtReiniciar.setFillColor(sf::Color::White);
    txtReiniciar.setPosition(600, 750); txtReiniciar.setOrigin(txtReiniciar.getLocalBounds().width / 2, 0);

    while (ventana.isOpen()) {
        Naves jugador(ventana.getSize(), texNave);
        Enemigos enemigos(ventana.getSize(), texEnemigo, powerups);
        std::unique_ptr<EnemigoJefe> jefe = nullptr;
        Puntos& puntos = jugador.obtenerPuntos();

        for (int i = 0; i < 5; ++i) enemigos.generarEnemigo(1.0f, 1.0f);

        sf::Clock reloj;
        bool gameOver = false;
        int puntosFinales = 0;

        sf::Text txtExp, txtNivel;
        txtExp.setFont(fuente); txtExp.setCharacterSize(20); txtExp.setFillColor(sf::Color::Yellow); txtExp.setPosition(10, 70);
        txtNivel.setFont(fuente); txtNivel.setCharacterSize(20); txtNivel.setFillColor(sf::Color::Cyan); txtNivel.setPosition(10, 100);
        sf::RectangleShape barraFondo({ 200, 10 }), barraExp;
        barraFondo.setPosition(10, 130); barraFondo.setFillColor(sf::Color::Black);
        barraExp.setPosition(10, 130); barraExp.setFillColor(sf::Color::Green);

        while (ventana.isOpen() && !gameOver) {
            float dt = reloj.restart().asSeconds();
            sf::Event evento;
            while (ventana.pollEvent(evento)) {
                if (evento.type == sf::Event::Closed) { ventana.close(); return; }
                jugador.manejarEvento(evento, ventana);
            }

            jugador.intentarRecogerPowerUp(powerups);
            sf::Vector2f raton = ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));
            jugador.comando(ventana.getSize(), dt, raton);
            jugador.actualizar(dt, raton);

            int nivel = puntos.obtenerNivel();

            if (nivel >= 5 && jugador.obtenerTextura() != &naveNivel5) jugador.cambiarTextura(naveNivel5);
            else if (nivel >= 3 && jugador.obtenerTextura() != &naveNivel3) jugador.cambiarTextura(naveNivel3);
            if (nivel >= 4 && enemigos.obtenerTextura() != &enemigoNivel4) enemigos.cambiarTextura(enemigoNivel4);

            if (nivel >= 3 && !jefe && (std::rand() % 300) == 0) {
                jefe = std::make_unique<EnemigoJefe>(ventana.getSize(), powerups);
            }

            powerups.actualizar(dt);
            enemigos.actualizar(dt, jugador.obtenerPosicion(), puntos);
            enemigos.manejarColisiones(jugador.obtenerDisparos(), jugador, puntos);

            if (jefe) {
                jefe->actualizar(dt, jugador.obtenerPosicion(), puntos);
                jefe->manejarColision(jugador.obtenerDisparos(), jugador, puntos);
                if (!jefe->estaVivo()) jefe.reset();
            }

            if (enemigos.obtenerCantidad() < 5 + nivel) {
                enemigos.generarEnemigo(1.0f + 0.2f * nivel, 1.0f + 0.1f * nivel);
            }

            std::ostringstream oss;
            oss << "EXP: " << puntos.obtenerPuntos() << " / " << puntos.obtenerPuntosSiguienteNivel();
            txtExp.setString(oss.str());
            oss.str(""); oss << "Nivel Nave: " << nivel;
            txtNivel.setString(oss.str());
            float porcentajeExp = static_cast<float>(puntos.obtenerPuntos()) / puntos.obtenerPuntosSiguienteNivel();
            barraExp.setSize({ 200.f * std::max(0.f, std::min(1.f, porcentajeExp)), 10.f });

            if (!jugador.estaVivo()) {
                gameOver = true;
                puntosFinales = puntos.obtenerPuntosTotales();
                oss.str(""); oss << "Puntos Finales (" << nombreJugador << "): " << puntosFinales;
                txtPuntosFinales.setString(oss.str());
                txtPuntosFinales.setOrigin(txtPuntosFinales.getLocalBounds().width / 2, 0);
            }

            ventana.clear();
            ventana.draw(fondo);
            ventana.draw(powerups);
            ventana.draw(enemigos);
            if (jefe) ventana.draw(*jefe);
            ventana.draw(jugador);
            ventana.draw(barraFondo);
            ventana.draw(barraExp);
            ventana.draw(txtExp);
            ventana.draw(txtNivel);
            ventana.draw(puntos);
            ventana.display();
        }

        musicaJugar.stop();
        GestorRanking gr;
        bool registrado = false;
        while (ventana.isOpen() && gameOver) {
            if (!registrado) { gr.registrarPuntaje(nombreJugador, puntosFinales); registrado = true; }
            sf::Event evento;
            while (ventana.pollEvent(evento)) {
                if (evento.type == sf::Event::Closed) return;
                if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Enter) return;
            }
            ventana.clear();
            ventana.draw(fondo);
            ventana.draw(txtGameOver);
            gr.mostrarRanking(ventana, nombreJugador, puntosFinales, fuente);
            ventana.draw(txtReiniciar);
            ventana.display();
        }
    }
}