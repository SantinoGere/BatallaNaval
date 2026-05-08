#include "Disparos.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

Disparos::Disparos() : _tamVentana(800, 600) {
    _sndRayo.loadFromFile("sonidorayo.ogg.ogg");
    _sonidoRayo.setBuffer(_sndRayo);
}

Disparos::Disparos(const sf::Vector2u& tamVentana) : _tamVentana(tamVentana) {
    _sndRayo.loadFromFile("sonidorayo.ogg.ogg");
    _sonidoRayo.setBuffer(_sndRayo);
}

bool Disparos::puedeDisparar(float& enfriamiento, float maxEnfriamiento) {
    return enfriamiento <= 0.f;
}

void Disparos::iniciarEnfriamiento(float& enfriamiento, float maxEnfriamiento) {
    enfriamiento = maxEnfriamiento;
}

void Disparos::dispararPrimerMisil(const sf::Vector2f& pos, const sf::Vector2f& objetivo, float rotacionNave) {
    if (!puedeDisparar(_enfriamientoPrimer, 1.0f)) return;
    iniciarEnfriamiento(_enfriamientoPrimer, 1.0f);
    Misil m;
    m.forma.setRadius(6.f);
    m.forma.setFillColor(sf::Color::Red);
    m.forma.setOrigin(6.f, 6.f);
    m.forma.setPosition(pos);
    m.tipo = Misil::PrimerDisparo;
    m.rapidez = 80.f;
    sf::Vector2f dir = objetivo - pos;
    float len = std::hypot(dir.x, dir.y);
    if (len > 0.001f) dir /= len;
    m.direccion = dir;
    m.velocidad = dir * m.rapidez;
    m.usado = false;
    _misiles.push_back(m);
    // SIN SONIDO MISIL
}

void Disparos::dispararDobleMisil(const sf::Vector2f& pos, const sf::Vector2f& dir, float rotacionNave) {
    if (!puedeDisparar(_enfriamientoDoble, 2.0f)) return;
    iniciarEnfriamiento(_enfriamientoDoble, 2.0f);
    float angRad = rotacionNave * 3.14159f / 180.f;
    sf::Vector2f perp(-std::sin(angRad), std::cos(angRad));
    sf::Vector2f dir1 = dir + perp * 0.4f;
    sf::Vector2f dir2 = dir - perp * 0.4f;
    auto crearMisil = [&](const sf::Vector2f& d) {
        Misil m;
        m.forma.setRadius(5.f);
        m.forma.setFillColor(sf::Color::Magenta);
        m.forma.setOrigin(5.f, 5.f);
        m.forma.setPosition(pos);
        m.tipo = Misil::DisparoDoble;
        m.rapidez = 250.f;
        float len = std::hypot(d.x, d.y);
        if (len > 0.001f) {
            m.direccion = d / len;
            m.velocidad = m.direccion * m.rapidez;
        }
        m.usado = false;
        _misiles.push_back(m);
        };
    crearMisil(dir1);
    crearMisil(dir2);
    // SIN SONIDO MISIL
}

void Disparos::dispararRayo(const sf::Vector2f& pos, const sf::Vector2f& dir) {
    if (!puedeDisparar(_enfriamientoRayo, 5.0f)) return;
    iniciarEnfriamiento(_enfriamientoRayo, 5.0f);
    _sonidoRayo.play(); // ÚNICO SONIDO ACTIVADO

    sf::Color colorInicio(255, 255, 100, 255);
    sf::Color colorFinal(255, 180, 0, 220);
    const float ANCHO_RAYO = 40.f;
    const int NUM_LINEAS = 10;
    sf::Vector2f perp(-dir.y, dir.x);
    float lenPerp = std::hypot(perp.x, perp.y);
    if (lenPerp > 0.001f) perp /= lenPerp;
    for (int i = 0; i < NUM_LINEAS; ++i) {
        float offset = (i - NUM_LINEAS / 2) * (ANCHO_RAYO / NUM_LINEAS);
        sf::Vector2f desplazamiento = perp * offset;
        Laser l;
        l.rayo[0] = sf::Vertex(pos + desplazamiento, colorInicio);
        l.rayo[1] = sf::Vertex(pos + dir * 1200.f + desplazamiento, colorFinal);
        l.inicio = l.rayo[0].position;
        l.fin = l.rayo[1].position;
        l.direccion = dir;
        l.duracion = 0.8f;
        l.usado = false;
        _rayos.push_back(l);
    }
}

void Disparos::actualizar(float deltaTime, const sf::Vector2f& ratonPos) {
    _enfriamientoPrimer = std::max(0.f, _enfriamientoPrimer - deltaTime);
    _enfriamientoDoble = std::max(0.f, _enfriamientoDoble - deltaTime);
    _enfriamientoRayo = std::max(0.f, _enfriamientoRayo - deltaTime);

    for (auto& m : _misiles) {
        if (m.usado) continue;
        m.forma.move(m.velocidad * deltaTime);
        sf::FloatRect bounds = m.forma.getGlobalBounds();
        if (bounds.left < 0 || bounds.left + bounds.width > _tamVentana.x ||
            bounds.top < 0 || bounds.top + bounds.height > _tamVentana.y) {
            m.usado = true;
        }
    }

    for (auto& l : _rayos) {
        if (l.usado) continue;
        l.duracion -= deltaTime;
        if (l.duracion <= 0.f) {
            l.usado = true;
        }
        else {
            float alpha = l.duracion / 0.8f;
            l.rayo[0].color.a = static_cast<sf::Uint8>(255 * alpha);
            l.rayo[1].color.a = static_cast<sf::Uint8>(220 * alpha);
        }
    }

    _misiles.erase(std::remove_if(_misiles.begin(), _misiles.end(),
        [](const Misil& m) { return m.usado; }), _misiles.end());
    _rayos.erase(std::remove_if(_rayos.begin(), _rayos.end(),
        [](const Laser& l) { return l.usado; }), _rayos.end());

    for (auto& e : _explosiones) {
        e.tiempoVida -= deltaTime;
        if (e.tiempoVida <= 0.f) continue;
        float escala = 1.f + (0.4f - e.tiempoVida) * 8.f;
        e.forma.setScale(escala, escala);
        sf::Uint8 alpha = static_cast<sf::Uint8>(255 * e.tiempoVida / 0.4f);
        e.forma.setFillColor(sf::Color(255, 150, 0, alpha));
    }
    _explosiones.erase(std::remove_if(_explosiones.begin(), _explosiones.end(),
        [](const Explosion& e) { return e.tiempoVida <= 0; }), _explosiones.end());
}

void Disparos::agregarExplosion(const Explosion& exp) {
    _explosiones.push_back(exp);
}

void Disparos::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& m : _misiles) {
        if (!m.usado) target.draw(m.forma, states);
    }
    for (const auto& l : _rayos) {
        if (!l.usado) {
            target.draw(l.rayo, 2, sf::Lines, states);
        }
    }
    for (const auto& e : _explosiones) {
        target.draw(e.forma, states);
    }
}