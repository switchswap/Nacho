#include <SFML/Graphics.hpp>
#include <iostream>
#include "chip8.h"
int main() {
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}
	std::cout << "Hello" << std::endl;
	// Main loops
		// Keypad interrupt loop
		// Emulate Cycle
		// Draw scene
	return 0;
}