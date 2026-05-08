#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "jugar.hpp"
#include "Menu.hpp"
#include "jugar.hpp"
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    sf::RenderWindow ventana(sf::VideoMode(1200, 900), "Batalla Estelar", sf::Style::Close);
    ventana.setFramerateLimit(60);
    ventana.setKeyRepeatEnabled(false);

    Menu miMenu(ventana);
    std::string nombreJugador = miMenu.obtenerNombre();
    if (nombreJugador.empty()) {
        return 0;
    }

    while (ventana.isOpen()) {
        if (!miMenu.mostrar()) break;
        jugar(ventana, nombreJugador);
    }

    return 0;
}