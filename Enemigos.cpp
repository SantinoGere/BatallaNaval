#include "Enemigos.hpp"
#include "PowerUp.hpp"
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iostream>

Enemigos::Enemigos(const sf::Vector2u& tamVentana, sf::Texture& textura, PowerUps& powerups)
    : _tamVentana(tamVentana), _textura(&textura), _powerups(powerups) {
    // CARGAR SONIDO DE EXPLOSIÓN
    if (_bufferExplosion.loadFromFile("Explocion.ogg.ogg")) {
        _sonidoExplosion.setBuffer(_bufferExplosion);
        _sonidoExplosion.setVolume(70.f);
        _sonidoCargado = true;
    }
    else {
        std::cerr << "Advertencia: No se pudo cargar Explocion.ogg\n";
    }
}

void Enemigos::reproducirExplosion(const sf::Vector2f& pos) {
    if (_sonidoCargado) {
        _sonidoExplosion.setPosition(pos.x, pos.y, 0);
        _sonidoExplosion.play();
    }
}

void Enemigos::generarEnemigo(float multVida, float multVel) {
    Enemigo e;
    e.disparos = Disparos(_tamVentana);
    e.sprite.setTexture(*_textura);
    float escala = 15.f / static_cast<float>(_textura->getSize().x);
    e.sprite.setScale(escala, escala);
    e.sprite.setOrigin(_textura->getSize().x / 2.f, _textura->getSize().y / 2.f);

    int lado = std::rand() % 4;
    sf::Vector2f pos;
    float x = static_cast<float>(_tamVentana.x);
    float y = static_cast<float>(_tamVentana.y);
    if (lado == 0) pos = { -100.f, static_cast<float>(std::rand() % static_cast<int>(y)) };
    else if (lado == 1) pos = { x + 100.f, static_cast<float>(std::rand() % static_cast<int>(y)) };
    else if (lado == 2) pos = { static_cast<float>(std::rand() % static_cast<int>(x)), -100.f };
    else pos = { static_cast<float>(std::rand() % static_cast<int>(x)), y + 100.f };

    e.sprite.setPosition(pos);
    e.vida = Enemigo::VIDA_BASE * multVida;
    e.tiempoDisparo = static_cast<float>(std::rand() % 100) * 0.01f;
    e.multVida = multVida;
    e.multVel = multVel;
    e.barraLista = false;
    _enemigos.push_back(std::move(e));
}

void Enemigos::actualizar(float dt, const sf::Vector2f& posJugador, const Puntos& puntos) {
    for (auto& e : _enemigos) {
        sf::Vector2f dir = posJugador - e.sprite.getPosition();
        float len = std::hypot(dir.x, dir.y);
        if (len > 0.001f) dir /= len;
        float velocidad = Enemigo::VELOCIDAD_BASE * e.multVel * puntos.obtenerMultiplicadorVelocidad();
        e.sprite.move(dir * velocidad * dt);
        float angulo = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
        e.sprite.setRotation(angulo + 180.f);
        e.tiempoDisparo += dt;
        if (e.tiempoDisparo >= 1.5f) {
            e.disparos.dispararPrimerMisil(e.sprite.getPosition(), posJugador, angulo);
            if (puntos.obtenerNivel() >= 5) {
                e.disparos.dispararDobleMisil(e.sprite.getPosition(), dir, angulo);
            }
            e.tiempoDisparo = 0.f;
        }
        e.disparos.actualizar(dt, posJugador);
    }

    _enemigos.erase(
        std::remove_if(_enemigos.begin(), _enemigos.end(),
            [](const Enemigo& e) { return e.vida <= 0.f; }),
        _enemigos.end()
    );
}

void Enemigos::manejarColisiones(const Disparos& disparosJugador, Naves& jugador, Puntos& puntos) {
    const auto& misiles = disparosJugador.obtenerMisiles();
    const auto& rayos = disparosJugador.obtenerRayos();

    for (size_t i = 0; i < misiles.size(); ++i) {
        if (misiles[i].usado) continue;
        for (size_t j = 0; j < _enemigos.size(); ++j) {
            if (_enemigos[j].vida <= 0.f) continue;
            if (_enemigos[j].sprite.getGlobalBounds().intersects(misiles[i].forma.getGlobalBounds())) {
                const_cast<Disparos::Misil&>(misiles[i]).usado = true;
                _enemigos[j].vida -= 100.f;
                puntos.sumarPuntos(10);
                if (_enemigos[j].vida <= 0.f) {
                    // EXPLOSIÓN ROJA
                    sf::CircleShape explosion(30.f);
                    explosion.setFillColor(sf::Color(255, 0, 0, 200));
                    explosion.setOrigin(30, 30);
                    explosion.setPosition(_enemigos[j].sprite.getPosition());
                    _enemigos[j].disparos.agregarExplosion({ explosion, 0.6f, 80.f });
                    reproducirExplosion(_enemigos[j].sprite.getPosition());
                    if ((std::rand() % 100) < 10) {
                        _powerups.generar(_enemigos[j].sprite.getPosition());
                    }
                }
                break;
            }
        }
    }

    for (size_t i = 0; i < rayos.size(); ++i) {
        if (rayos[i].usado) continue;
        for (size_t j = 0; j < _enemigos.size(); ++j) {
            if (_enemigos[j].vida <= 0.f) continue;
            sf::FloatRect laserRect(
                std::min(rayos[i].inicio.x, rayos[i].fin.x),
                std::min(rayos[i].inicio.y, rayos[i].fin.y),
                std::abs(rayos[i].fin.x - rayos[i].inicio.x),
                std::abs(rayos[i].fin.y - rayos[i].inicio.y)
            );
            laserRect.width = std::max(laserRect.width, 20.f);
            laserRect.height = std::max(laserRect.height, 20.f);
            if (_enemigos[j].sprite.getGlobalBounds().intersects(laserRect)) {
                const_cast<Disparos::Laser&>(rayos[i]).usado = true;
                _enemigos[j].vida -= 200.f;
                puntos.sumarPuntos(20);
                if (_enemigos[j].vida <= 0.f) {
                    sf::CircleShape explosion(40.f);
                    explosion.setFillColor(sf::Color(255, 50, 50, 220));
                    explosion.setOrigin(40, 40);
                    explosion.setPosition(_enemigos[j].sprite.getPosition());
                    _enemigos[j].disparos.agregarExplosion({ explosion, 0.8f, 120.f });
                    reproducirExplosion(_enemigos[j].sprite.getPosition());
                    if ((std::rand() % 100) < 10) {
                        _powerups.generar(_enemigos[j].sprite.getPosition());
                    }
                }
                break;
            }
        }
    }

    for (auto& e : _enemigos) {
        const auto& misilesEnemigo = e.disparos.obtenerMisiles();
        for (size_t i = 0; i < misilesEnemigo.size(); ++i) {
            if (misilesEnemigo[i].usado) continue;
            if (jugador.obtenerLimites().intersects(misilesEnemigo[i].forma.getGlobalBounds())) {
                const_cast<Disparos::Misil&>(misilesEnemigo[i]).usado = true;
                int nivel = puntos.obtenerNivel();
                float dano = 10.f * std::powf(1.3f, static_cast<float>(nivel - 1));
                jugador.recibirDanio(dano);
                break;
            }
        }
    }
}

void Enemigos::cambiarTextura(sf::Texture& nueva) {
    _textura = &nueva;
    for (auto& e : _enemigos) {
        e.sprite.setTexture(nueva);
        float escala = 15.f / static_cast<float>(nueva.getSize().x);
        e.sprite.setScale(escala, escala);
        e.sprite.setOrigin(nueva.getSize().x / 2.f, nueva.getSize().y / 2.f);
        e.barraLista = false;
    }
}

void Enemigos::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& e : _enemigos) {
        target.draw(e.sprite, states);
        target.draw(e.disparos, states);
        if (!e.barraLista) {
            Enemigo& enemigo = const_cast<Enemigo&>(e);
            enemigo.barraFondo.setSize({ 40.f, 6.f });
            enemigo.barraFondo.setFillColor(sf::Color(30, 30, 30));
            enemigo.barraFondo.setOrigin(20.f, 3.f);
            enemigo.barraVida.setSize({ 40.f, 6.f });
            enemigo.barraVida.setFillColor(sf::Color::Red);
            enemigo.barraVida.setOrigin(20.f, 3.f);
            enemigo.barraLista = true;
        }
        sf::Vector2f pos = e.sprite.getPosition();
        const_cast<Enemigo&>(e).barraFondo.setPosition(pos.x, pos.y - 30.f);
        const_cast<Enemigo&>(e).barraVida.setPosition(pos.x, pos.y - 30.f);
        float porcentajeVida = e.vida / (Enemigo::VIDA_BASE * e.multVida);
        const_cast<Enemigo&>(e).barraVida.setSize({ 40.f * std::max(0.f, porcentajeVida), 6.f });
        target.draw(e.barraFondo, states);
        target.draw(e.barraVida, states);
    }
}