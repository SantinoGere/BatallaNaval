#include "Naves.hpp"
#include "PowerUp.hpp"
#include <cmath>

Naves::Naves(const sf::Vector2u& tamVentana, sf::Texture& textura)
    : _puntos(), _tamVentana(tamVentana), _textura(&textura),
    _disparos(tamVentana), _vida(VIDA_BASE* _puntos.obtenerMultiplicadorVida())
{
    _sprite.setTexture(*_textura);
    float escala = 20.f / _textura->getSize().x;
    _sprite.setScale(escala, escala);
    _sprite.setOrigin(_textura->getSize().x / 2.f, _textura->getSize().y / 2.f);
    setPosition(tamVentana.x / 2.f, tamVentana.y / 2.f);
    _sprite.setPosition(getPosition());
    _sprite.setRotation(180.f);
}

void Naves::cambiarTextura(sf::Texture& nueva) {
    _textura = &nueva;
    _sprite.setTexture(nueva);
    float escala = 20.f / nueva.getSize().x;
    _sprite.setScale(escala, escala);
    _sprite.setOrigin(nueva.getSize().x / 2.f, nueva.getSize().y / 2.f);
    _barraLista = false;
}

void Naves::comando(const sf::Vector2u& tamVentana, float dt, const sf::Vector2f& raton) {
    sf::Vector2f entrada(0, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) entrada.y -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) entrada.y += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) entrada.x -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) entrada.x += 1;
    float len = std::hypot(entrada.x, entrada.y);
    if (len > 0.001f) {
        entrada /= len;
        float mult = (_tiempoRalentizado > 0) ? 0.5f : _puntos.obtenerMultiplicadorVelocidad();
        sf::Vector2f mov = entrada * _velocidad.x * mult * dt;
        sf::Vector2f nuevaPos = getPosition() + mov;
        float radio = _textura->getSize().x * _sprite.getScale().x / 2.f + 10.f;
        nuevaPos.x = std::max(radio, std::min((float)tamVentana.x - radio, nuevaPos.x));
        nuevaPos.y = std::max(radio, std::min((float)tamVentana.y - radio, nuevaPos.y));
        setPosition(nuevaPos);
        _sprite.setPosition(nuevaPos);
        float angulo = std::atan2(entrada.y, entrada.x) * 180.f / 3.14159f;
        setRotation(angulo);
        _sprite.setRotation(angulo + 180.f);
    }
}

void Naves::actualizar(float dt, const sf::Vector2f& raton) {
    _disparos.actualizar(dt, raton);
    if (_tiempoRalentizado > 0) _tiempoRalentizado -= dt;
    actualizarPowerUps(dt);

    static int ultimoNivel = 1;
    if (_puntos.obtenerNivel() > ultimoNivel) {
        _vida = VIDA_BASE * _puntos.obtenerMultiplicadorVida();
        ultimoNivel = _puntos.obtenerNivel();
    }
}

void Naves::actualizarPowerUps(float dt) {
    if (_tiempoInvencibilidad > 0) {
        _tiempoInvencibilidad -= dt;
    }
}

void Naves::intentarRecogerPowerUp(PowerUps& powerups) {
    PowerUps::Tipo tipo;
    if (powerups.recoger(obtenerLimites(), tipo)) {
        switch (tipo) {
        case PowerUps::CURACION:
            _vida = std::min(_vida + 50.f, VIDA_BASE * _puntos.obtenerMultiplicadorVida());
            break;
        case PowerUps::ESCUDO:
            _escudoActivo = true;
            break;
        case PowerUps::INVENCIBILIDAD:
            _tiempoInvencibilidad = 5.f;
            break;
        }
    }
}

void Naves::manejarEvento(const sf::Event& e, sf::RenderWindow& ventana) {
    if (e.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f clic = ventana.mapPixelToCoords(sf::Vector2i(e.mouseButton.x, e.mouseButton.y));
        sf::Vector2f dir = clic - getPosition();
        float len = std::hypot(dir.x, dir.y);
        if (len > 0.001f) dir /= len;
        float angulo = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
        if (e.mouseButton.button == sf::Mouse::Left) {
            _disparos.dispararPrimerMisil(getPosition(), clic, angulo);
        }
        else if (e.mouseButton.button == sf::Mouse::Right) {
            _disparos.dispararDobleMisil(getPosition(), dir, angulo);
        }
    }
    else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space) {
        float rad = getRotation() * 3.14159f / 180.f;
        sf::Vector2f dir(std::cos(rad), std::sin(rad));
        _disparos.dispararRayo(getPosition(), dir);
    }
}

void Naves::recibirDanio(float d) {
    if (_tiempoInvencibilidad > 0) return;
    if (_escudoActivo) {
        _escudoActivo = false;
        return;
    }
    _vida = std::max(0.f, _vida - d);
}

void Naves::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Efectos visuales powerups
    if (_tiempoInvencibilidad > 0) {
        float alpha = 100 + 155 * std::sin(_tiempoInvencibilidad * 10);
        sf::CircleShape aura(40);
        aura.setFillColor(sf::Color(255, 255, 100, static_cast<sf::Uint8>(alpha)));
        aura.setOrigin(40, 40);
        aura.setPosition(getPosition());
        target.draw(aura, states);
    }
    if (_escudoActivo) {
        sf::CircleShape escudo(35);
        escudo.setFillColor(sf::Color::Transparent);
        escudo.setOutlineColor(sf::Color(100, 180, 255));
        escudo.setOutlineThickness(3);
        escudo.setOrigin(35, 35);
        escudo.setPosition(getPosition());
        target.draw(escudo, states);
    }

    target.draw(_sprite, states);
    target.draw(_disparos, states);

    if (!_barraLista) {
        const_cast<Naves*>(this)->_barraFondo.setSize({ 50, 8 });
        const_cast<Naves*>(this)->_barraFondo.setFillColor(sf::Color::Black);
        const_cast<Naves*>(this)->_barraFondo.setOrigin(25, 4);
        const_cast<Naves*>(this)->_barraVida.setSize({ 50, 8 });
        const_cast<Naves*>(this)->_barraVida.setFillColor(sf::Color::Blue);
        const_cast<Naves*>(this)->_barraVida.setOrigin(25, 4);
        const_cast<Naves*>(this)->_barraLista = true;
    }
    sf::Vector2f pos = _sprite.getPosition();
    _barraFondo.setPosition(pos.x, pos.y - 35);
    _barraVida.setPosition(pos.x, pos.y - 35);
    float porc = _vida / (VIDA_BASE * _puntos.obtenerMultiplicadorVida());
    _barraVida.setSize({ 50 * std::max(0.f, porc), 8 });
    target.draw(_barraFondo, states);
    target.draw(_barraVida, states);
}