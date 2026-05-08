#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class PowerUps : public sf::Drawable {
public:
    enum Tipo { ESCUDO, INVENCIBILIDAD, CURACION };
    PowerUps(const sf::Vector2u& tamVentana);
    void generar(const sf::Vector2f& posicion);
    void actualizar(float dt);
    bool recoger(sf::FloatRect rectNave, Tipo& tipoRecogido);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    struct Power {
        sf::Text simbolo;
        Tipo tipo = ESCUDO;
        sf::Clock relojVida;
        bool activo = true;
    };
    sf::Vector2u _tamVentana;
    sf::Font _fuente;
    std::vector<Power> _lista;
};