#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>

class Puntos : public sf::Drawable {
public:
    Puntos();
    void sumarPuntos(int cantidad);
    int obtenerPuntos() const { return _puntos; }
    int obtenerPuntosTotales() const { return _puntosTotales; }
    int obtenerNivel() const { return _nivel; }
    int obtenerPuntosSiguienteNivel() const;
    float obtenerMultiplicadorVelocidad() const;
    float obtenerMultiplicadorVida() const;
    void reset();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    int _puntos = 0;
    int _puntosTotales = 0;
    int _nivel = 1;
    sf::Font _fuente;
    mutable sf::Text _textoPuntos, _textoNivel;
    mutable bool _inicializado = false;
};

struct EntradaRanking {
    std::string nombre;
    int puntaje = 0;
    bool operator<(const EntradaRanking& otra) const { return puntaje > otra.puntaje; }
};

class GestorRanking {
public:
    GestorRanking();
    void registrarPuntaje(const std::string& nombre, int puntaje);
    void mostrarRanking(sf::RenderWindow& ventana, const std::string& nombreActual = "", int puntajeActual = 0, const sf::Font& fuente = sf::Font()) const;

private:
    std::vector<EntradaRanking> _ranking;
    static constexpr int MAX_ENTRADAS = 10;
    void cargar();
    void guardar() const;
};