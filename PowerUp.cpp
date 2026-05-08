#include "PowerUp.hpp"
#include <cstdlib>
#include <ctime>

PowerUps::PowerUps(const sf::Vector2u& tamVentana) : _tamVentana(tamVentana) {
    if (!_fuente.loadFromFile("arial.ttf.ttf")) {
        // No hay error crítico, solo advertencia
    }
}

void PowerUps::generar(const sf::Vector2f& posicion) {
    Power p;
    p.tipo = static_cast<Tipo>(std::rand() % 3);
    p.relojVida.restart();
    p.activo = true;

    p.simbolo.setFont(_fuente);
    p.simbolo.setCharacterSize(24);
    p.simbolo.setFillColor(sf::Color::White);
    p.simbolo.setStyle(sf::Text::Bold);
    p.simbolo.setPosition(posicion.x - 12, posicion.y - 18);

    switch (p.tipo) {
    case ESCUDO: p.simbolo.setString("E"); break;
    case INVENCIBILIDAD: p.simbolo.setString("I"); break;
    case CURACION: p.simbolo.setString("H"); break;
    }
    _lista.push_back(p);
}

void PowerUps::actualizar(float dt) {
    for (auto it = _lista.begin(); it != _lista.end(); ) {
        if (!it->activo || it->relojVida.getElapsedTime().asSeconds() > 10.f) {
            it = _lista.erase(it);
        }
        else {
            ++it;
        }
    }
}

bool PowerUps::recoger(sf::FloatRect rectNave, Tipo& tipoRecogido) {
    for (auto& p : _lista) {
        if (!p.activo) continue;
        sf::FloatRect bounds(p.simbolo.getPosition().x - 15, p.simbolo.getPosition().y - 15, 30, 30);
        if (rectNave.intersects(bounds)) {
            tipoRecogido = p.tipo;
            p.activo = false;
            return true;
        }
    }
    return false;
}

void PowerUps::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& p : _lista) {
        if (p.activo) {
            sf::CircleShape circ(15.f);
            circ.setOrigin(15, 15);
            circ.setPosition(p.simbolo.getPosition() + sf::Vector2f(12, 18));
            switch (p.tipo) {
            case ESCUDO: circ.setFillColor(sf::Color(100, 180, 255)); break;
            case INVENCIBILIDAD: circ.setFillColor(sf::Color(255, 215, 0)); break;
            case CURACION: circ.setFillColor(sf::Color(255, 100, 100)); break;
            }
            target.draw(circ, states);
            target.draw(p.simbolo, states);
        }
    }
}