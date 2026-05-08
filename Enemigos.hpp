#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Disparos.hpp"
#include "Puntos.hpp"
#include "Naves.hpp"
#include "PowerUp.hpp"
#include <vector>

class Enemigos : public sf::Drawable {
public:
    Enemigos(const sf::Vector2u& tamVentana, sf::Texture& textura, PowerUps& powerups);
    void generarEnemigo(float multVida, float multVel);
    void actualizar(float dt, const sf::Vector2f& posJugador, const Puntos& puntos);
    void manejarColisiones(const Disparos& disparosJugador, Naves& jugador, Puntos& puntos);
    sf::Texture* obtenerTextura() const { return _textura; }
    int obtenerCantidad() const { return static_cast<int>(_enemigos.size()); }
    void cambiarTextura(sf::Texture& nueva);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    struct Enemigo {
        sf::Sprite sprite;
        Disparos disparos;
        sf::Vector2f velocidad;
        float vida = 100.f;
        float tiempoDisparo = 0.f;
        float multVida = 1.f;
        float multVel = 1.f;
        mutable sf::RectangleShape barraFondo, barraVida;
        mutable bool barraLista = false;
        static constexpr float VIDA_BASE = 100.f;
        static constexpr float VELOCIDAD_BASE = 40.f;
    };

    sf::Vector2u _tamVentana;
    sf::Texture* _textura;
    std::vector<Enemigo> _enemigos;
    PowerUps& _powerups;

    // EXPLOSIÓN VISUAL + SONIDO
    sf::SoundBuffer _bufferExplosion;
    sf::Sound _sonidoExplosion;
    bool _sonidoCargado = false;
    void reproducirExplosion(const sf::Vector2f& pos);
};