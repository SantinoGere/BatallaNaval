#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class Disparos : public sf::Drawable {
public:
    // === ESTRUCTURAS DENTRO DE LA CLASE ===
    struct Misil {
        sf::CircleShape forma;
        sf::Vector2f velocidad;
        sf::Vector2f direccion;
        float rapidez = 0.f;
        bool usado = false;
        enum Tipo { PrimerDisparo, DisparoDoble } tipo = PrimerDisparo;
    };

    struct Laser {
        sf::Vertex rayo[2];
        sf::Vector2f inicio;
        sf::Vector2f fin;
        sf::Vector2f direccion;
        float duracion = 0.f;
        bool usado = false;
    };

    struct Explosion {
        sf::CircleShape forma;
        float tiempoVida = 0.f;
        float dano = 0.f;
    };
    // ======================================

    Disparos();
    Disparos(const sf::Vector2u& tamVentana);
    void dispararPrimerMisil(const sf::Vector2f& pos, const sf::Vector2f& objetivo, float rotacionNave);
    void dispararDobleMisil(const sf::Vector2f& pos, const sf::Vector2f& dir, float rotacionNave);
    void dispararRayo(const sf::Vector2f& pos, const sf::Vector2f& dir);
    void actualizar(float deltaTime, const sf::Vector2f& ratonPos);
    void agregarExplosion(const Explosion& exp);
    const std::vector<Misil>& obtenerMisiles() const { return _misiles; }
    const std::vector<Laser>& obtenerRayos() const { return _rayos; }
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Vector2u _tamVentana;
    std::vector<Misil> _misiles;
    std::vector<Laser> _rayos;
    std::vector<Explosion> _explosiones;

    // SONIDO SOLO PARA RAYO
    sf::SoundBuffer _sndRayo;
    sf::Sound _sonidoRayo;

    float _enfriamientoPrimer = 0.f;
    float _enfriamientoDoble = 0.f;
    float _enfriamientoRayo = 0.f;

    bool puedeDisparar(float& enfriamiento, float maxEnfriamiento);
    void iniciarEnfriamiento(float& enfriamiento, float maxEnfriamiento);
};