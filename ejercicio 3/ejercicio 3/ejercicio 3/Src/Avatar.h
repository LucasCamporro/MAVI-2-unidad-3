#pragma once
#include <Box2D/Box2D.h>

#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"

class Avatar {

private:
	sf::Sprite* pelota_Sprite;

	b2Body* pelota_body;

public:

	Avatar(b2Body* body, sf::Sprite* sprite);

	void Actualizar();

	void Dibujar(sf::RenderWindow& wnd);

};