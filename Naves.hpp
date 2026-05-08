#pragma once
#include <SFML/Graphics.hpp>
#include "Disparos.hpp"
#include "Puntos.hpp"
#include "PowerUp.hpp"

class Naves : public sf::Drawable, public sf::Transformable {
public:
    Naves(const sf::Vector2u& tamVentana, sf::Texture& textura);
    void comando(const sf::Vector2u& tamVentana, float dt, const sf::Vector2f& raton);
    void actualizar(float dt, const sf::Vector2f& raton);
    void manejarEvento(const sf::Event& e, sf::RenderWindow& ventana);
    void recibirDanio(float dano);
    void intentarRecogerPowerUp(PowerUps& powerups);
    bool estaVivo() const { return _vida > 0; }
    const Disparos& obtenerDisparos() const { return _disparos; }
    Disparos& obtenerDisparos() { return _disparos; }
    sf::FloatRect obtenerLimites() const { return _sprite.getGlobalBounds(); }
    sf::Vector2f obtenerPosicion() const { return getPosition(); }
    Puntos& obtenerPuntos() { return _puntos; }
    const sf::Texture* obtenerTextura() const { return _textura; }
    void cambiarTextura(sf::Texture& nueva);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    static constexpr float VIDA_BASE = 180.0f;
    static constexpr float VELOCIDAD_BASE = 50.0f;

    void actualizarPowerUps(float dt);

    Puntos _puntos;
    sf::Texture* _textura;
    sf::Sprite _sprite;
    sf::Vector2f _velocidad = { VELOCIDAD_BASE, VELOCIDAD_BASE };
    sf::Vector2u _tamVentana;
    Disparos _disparos;
    float _vida;
    float _tiempoRalentizado = 0.f;

    // PowerUps
    bool _escudoActivo = false;
    float _tiempoInvencibilidad = 0.f;

    mutable sf::RectangleShape _barraFondo, _barraVida;
    mutable bool _barraLista = false;
};