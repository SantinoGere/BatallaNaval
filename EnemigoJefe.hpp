// EnemigoJefe.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Disparos.hpp"
#include "Puntos.hpp"
#include "Naves.hpp"
#include "PowerUp.hpp"

class EnemigoJefe : public sf::Drawable {
public:
    EnemigoJefe(const sf::Vector2u& tamVentana, PowerUps& powerups);
    void actualizar(float dt, const sf::Vector2f& posJugador, const Puntos& puntos);
    void manejarColision(const Disparos& disparosJugador, Naves& jugador, Puntos& puntos);  // ← CORREGIDO
    bool estaVivo() const { return _vida > 0.f; }
    sf::FloatRect obtenerLimites() const { return _sprite.getGlobalBounds(); }
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Texture _textura;
    sf::Sprite _sprite;
    Disparos _disparos;
    sf::Vector2u _tamVentana;
    PowerUps& _powerups;

    float _vida = 800.f;
    float _tiempoDisparo = 0.f;
    static constexpr float VIDA_MAX = 800.f;
    static constexpr float VELOCIDAD = 10.f;
    static constexpr float CADENCIA = 0.8f;

    sf::SoundBuffer _bufferExplosion;
    sf::Sound _sonidoExplosion;
    bool _sonidoCargado = false;

    void reproducirExplosion();  // ← DECLARADO
};