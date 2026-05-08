// EnemigoJefe.cpp
#include "EnemigoJefe.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

EnemigoJefe::EnemigoJefe(const sf::Vector2u& tamVentana, PowerUps& powerups)
    : _tamVentana(tamVentana), _powerups(powerups), _disparos(tamVentana) {
    if (!_textura.loadFromFile("Jefe.png.png")) {
        std::cerr << "Error: No se pudo cargar Jefe.png\n";
    }
    _sprite.setTexture(_textura);
    float escala = 45.f / static_cast<float>(_textura.getSize().x); // 3x más grande
    _sprite.setScale(escala, escala);
    _sprite.setOrigin(_textura.getSize().x / 2.f, _textura.getSize().y / 2.f);
    _sprite.setPosition(tamVentana.x / 2.f, -100.f);

    if (_bufferExplosion.loadFromFile("ExplocionJefe.ogg.ogg")) {
        _sonidoExplosion.setBuffer(_bufferExplosion);
        _sonidoExplosion.setVolume(90.f);
        _sonidoCargado = true;
    }
}

void EnemigoJefe::reproducirExplosion() {
    if (_sonidoCargado) {
        _sonidoExplosion.play();
    }
}

void EnemigoJefe::actualizar(float dt, const sf::Vector2f& posJugador, const Puntos& puntos) {
    if (_vida <= 0.f) return;

    sf::Vector2f dir = posJugador - _sprite.getPosition();
    float len = std::hypot(dir.x, dir.y);
    if (len > 0.001f) dir /= len;
    _sprite.move(dir * VELOCIDAD * dt);
    float angulo = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
    _sprite.setRotation(angulo + 180.f);

    _tiempoDisparo += dt;
    if (_tiempoDisparo >= CADENCIA) {
        _disparos.dispararPrimerMisil(_sprite.getPosition(), posJugador, angulo);
        _disparos.dispararDobleMisil(_sprite.getPosition(), dir, angulo);
        _tiempoDisparo = 0.f;
    }
    _disparos.actualizar(dt, posJugador);
}

void EnemigoJefe::manejarColision(const Disparos& disparosJugador, Naves& jugador, Puntos& puntos) {
    const auto& misiles = disparosJugador.obtenerMisiles();
    const auto& rayos = disparosJugador.obtenerRayos();

    for (const auto& m : misiles) {
        if (m.usado) continue;
        if (_sprite.getGlobalBounds().intersects(m.forma.getGlobalBounds())) {
            const_cast<Disparos::Misil&>(m).usado = true;
            _vida -= 100.f;
            puntos.sumarPuntos(100);
            if (_vida <= 0.f) {
                sf::CircleShape exp(80.f);
                exp.setFillColor(sf::Color(255, 100, 0, 230));
                exp.setOrigin(80, 80);
                exp.setPosition(_sprite.getPosition());
                _disparos.agregarExplosion({ exp, 1.2f, 200.f });
                reproducirExplosion();
                _powerups.generar(_sprite.getPosition());
            }
        }
    }

    for (const auto& r : rayos) {
        if (r.usado) continue;
        sf::FloatRect laserRect(
            std::min(r.inicio.x, r.fin.x), std::min(r.inicio.y, r.fin.y),
            std::abs(r.fin.x - r.inicio.x), std::abs(r.fin.y - r.inicio.y)
        );
        laserRect.width = std::max(laserRect.width, 30.f);
        laserRect.height = std::max(laserRect.height, 30.f);
        if (_sprite.getGlobalBounds().intersects(laserRect)) {
            const_cast<Disparos::Laser&>(r).usado = true;
            _vida -= 200.f;
            puntos.sumarPuntos(200);
            if (_vida <= 0.f) {
                sf::CircleShape exp(100.f);
                exp.setFillColor(sf::Color(255, 150, 50, 240));
                exp.setOrigin(100, 100);
                exp.setPosition(_sprite.getPosition());
                _disparos.agregarExplosion({ exp, 1.5f, 250.f });
                reproducirExplosion();
                _powerups.generar(_sprite.getPosition());
            }
        }
    }

    const auto& misilesJefe = _disparos.obtenerMisiles();
    for (const auto& m : misilesJefe) {
        if (m.usado) continue;
        if (jugador.obtenerLimites().intersects(m.forma.getGlobalBounds())) {
            const_cast<Disparos::Misil&>(m).usado = true;
            int nivel = puntos.obtenerNivel();
            float dano = 20.f * std::powf(1.3f, static_cast<float>(nivel - 1));
            jugador.recibirDanio(dano);
        }
    }
}

void EnemigoJefe::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (_vida > 0.f) {
        target.draw(_sprite, states);
        target.draw(_disparos, states);

        sf::RectangleShape fondo({ 120.f, 12.f });
        fondo.setFillColor(sf::Color(50, 0, 0));
        fondo.setOrigin(60, 6);
        fondo.setPosition(_sprite.getPosition().x, _sprite.getPosition().y - 60);
        target.draw(fondo, states);

        sf::RectangleShape vida({ 120.f * (_vida / VIDA_MAX), 12.f });
        vida.setFillColor(sf::Color::Red);
        vida.setOrigin(60, 6);
        vida.setPosition(_sprite.getPosition().x, _sprite.getPosition().y - 60);
        target.draw(vida, states);
    }
}