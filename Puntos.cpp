#include "Puntos.hpp"
#include <cmath>
#include <iostream>

Puntos::Puntos() {
    if (!_fuente.loadFromFile("arial.ttf.ttf")) {
        std::cerr << "Advertencia: arial.ttf no encontrado para Puntos.\n";
    }
}

void Puntos::sumarPuntos(int cantidad) {
    _puntos += cantidad;
    _puntosTotales += cantidad;

    int puntosRequeridos = obtenerPuntosSiguienteNivel();
    while (_puntos >= puntosRequeridos) {
        _puntos -= puntosRequeridos;
        _nivel++;
        puntosRequeridos = obtenerPuntosSiguienteNivel();
    }
}

int Puntos::obtenerPuntosSiguienteNivel() const {
    return static_cast<int>(100 * std::pow(1.5f, _nivel - 1));
}

float Puntos::obtenerMultiplicadorVelocidad() const {
    return 1.0f + 0.1f * (_nivel - 1);
}

float Puntos::obtenerMultiplicadorVida() const {
    return 1.0f + 0.15f * (_nivel - 1);
}

void Puntos::reset() {
    _puntos = 0;
    _puntosTotales = 0;
    _nivel = 1;
}

void Puntos::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!_inicializado) {
        const_cast<Puntos*>(this)->_textoPuntos.setFont(_fuente);
        const_cast<Puntos*>(this)->_textoPuntos.setCharacterSize(24);
        const_cast<Puntos*>(this)->_textoPuntos.setFillColor(sf::Color::White);
        const_cast<Puntos*>(this)->_textoPuntos.setPosition(10, 10);

        const_cast<Puntos*>(this)->_textoNivel.setFont(_fuente);
        const_cast<Puntos*>(this)->_textoNivel.setCharacterSize(20);
        const_cast<Puntos*>(this)->_textoNivel.setFillColor(sf::Color::Cyan);
        const_cast<Puntos*>(this)->_textoNivel.setPosition(10, 40);
        const_cast<Puntos*>(this)->_inicializado = true;
    }

    std::ostringstream oss;
    oss << "Puntos: " << _puntosTotales;
    _textoPuntos.setString(oss.str());

    oss.str("");
    oss << "Nivel: " << _nivel;
    _textoNivel.setString(oss.str());

    target.draw(_textoPuntos, states);
    target.draw(_textoNivel, states);
}

// === GESTOR RANKING ===

GestorRanking::GestorRanking() {
    cargar();
}

void GestorRanking::cargar() {
    std::ifstream archivo("ranking.txt");
    if (!archivo.is_open()) return;

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::istringstream iss(linea);
        EntradaRanking e;
        if (iss >> std::quoted(e.nombre) >> e.puntaje) {
            _ranking.push_back(e);
        }
    }
    std::sort(_ranking.begin(), _ranking.end());
    if (_ranking.size() > MAX_ENTRADAS) {
        _ranking.resize(MAX_ENTRADAS);
    }
    archivo.close();
}

void GestorRanking::guardar() const {
    std::ofstream archivo("ranking.txt");
    if (!archivo.is_open()) return;

    for (const auto& e : _ranking) {
        archivo << std::quoted(e.nombre) << " " << e.puntaje << "\n";
    }
    archivo.close();
}

void GestorRanking::registrarPuntaje(const std::string& nombre, int puntaje) {
    EntradaRanking nueva{ nombre, puntaje };
    _ranking.push_back(nueva);
    std::sort(_ranking.begin(), _ranking.end());
    if (_ranking.size() > MAX_ENTRADAS) {
        _ranking.resize(MAX_ENTRADAS);
    }
    guardar();
}

void GestorRanking::mostrarRanking(sf::RenderWindow& ventana, const std::string& nombreActual, int puntajeActual, const sf::Font& fuente) const {
    sf::Text titulo, entrada;
    titulo.setFont(fuente);
    titulo.setString("MEJORES PILOTOS ESTELARES");
    titulo.setCharacterSize(40);
    titulo.setFillColor(sf::Color::Yellow);
    titulo.setPosition(300, 300);
    sf::FloatRect bounds = titulo.getLocalBounds();
    titulo.setOrigin(bounds.width / 2, 0);

    entrada.setFont(fuente);
    entrada.setCharacterSize(28);
    entrada.setFillColor(sf::Color::White);

    std::vector<EntradaRanking> temp = _ranking;
    if (!nombreActual.empty() && puntajeActual > 0) {
        bool yaEnRanking = false;
        for (const auto& e : temp) {
            if (e.nombre == nombreActual && e.puntaje == puntajeActual) {
                yaEnRanking = true;
                break;
            }
        }
        if (!yaEnRanking) {
            temp.push_back({ nombreActual, puntajeActual });
            std::sort(temp.begin(), temp.end());
            if (temp.size() > MAX_ENTRADAS) temp.resize(MAX_ENTRADAS);
        }
    }

    for (size_t i = 0; i < temp.size(); ++i) {
        std::ostringstream oss;
        oss << (i + 1) << ". " << temp[i].nombre << " - " << temp[i].puntaje << " pts";
        entrada.setString(oss.str());
        entrada.setPosition(300, 380 + i * 40);
        if (temp[i].nombre == nombreActual && temp[i].puntaje == puntajeActual) {
            entrada.setFillColor(sf::Color::Green);
        }
        else {
            entrada.setFillColor(sf::Color::White);
        }
        ventana.draw(entrada);
    }
    ventana.draw(titulo);
}