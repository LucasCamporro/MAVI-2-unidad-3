#include "Box2DHelper.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <tchar.h>
#include "SFMLRenderer.h"
#include <list>

#include "Avatar.h"

using namespace sf;
class Game
{
private:
	// Propiedades de la ventana
	int alto;
	int ancho;
	RenderWindow* wnd;
	Color clearColor;

	// Objetos de box2d
	b2World* phyWorld;

	//tiempo de frame
	float frameTime;
	int fps;

	// Cuerpo de box2d
	b2Body* bolaBody;

	Avatar* pelota_Avatar;

	sf::Texture texture;

public:

	// Constructores, destructores e inicializadores
	Game(int ancho, int alto, std::string titulo);
	void CreateEnemy(int x, int y);
	~Game(void);
	void InitPhysics();

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
};

Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom();
	InitPhysics();

}

void Game::Loop()
{
	while (wnd->isOpen())
	{
		wnd->clear(clearColor);
		DoEvents();
		UpdatePhysics();
		DrawGame();
		wnd->display();
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8);
	phyWorld->ClearForces();
	phyWorld->DebugDraw();
}

void Game::DrawGame()
{
	// Dibujamos el suelo
	sf::RectangleShape groundShape(sf::Vector2f(500, 5));
	groundShape.setFillColor(sf::Color::Red);
	groundShape.setPosition(0, 95);
	wnd->draw(groundShape);

	groundShape.setPosition(0, 0);
	wnd->draw(groundShape);

	sf::RectangleShape groundShape2(sf::Vector2f(5, 500));
	groundShape2.setFillColor(sf::Color::Red);
	groundShape2.setPosition(95, 0);
	wnd->draw(groundShape2);

	groundShape2.setPosition(0, 0);
	wnd->draw(groundShape2);

	sf::CircleShape controlShape(5);
	controlShape.setFillColor(sf::Color::Magenta);
	controlShape.setPosition(bolaBody->GetPosition().x - 5, bolaBody->GetPosition().y - 5);
	wnd->draw(controlShape);

	pelota_Avatar->Actualizar();
	pelota_Avatar->Dibujar(*wnd);
}

void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt))
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close();
			break;
		}
	}
}


// Definimos el area del mundo que veremos en nuestro juego
// Box2D tiene problemas para simular magnitudes muy grandes
void Game::SetZoom()
{
	View camara;
	// Posicion del view
	camara.setSize(100.0f, 100.0f);
	camara.setCenter(50.0f, 50.0f);
	wnd->setView(camara); //asignamos la camara
}

void Game::InitPhysics()
{
	// Inicializamos el mundo con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	// Creamos un piso y paredes
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Fixture* groundFixture = groundBody->GetFixtureList();
	groundFixture->SetRestitution(1.5f);

	b2Body* groundBody2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
	groundBody2->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

	b2Fixture* groundFixture2 = groundBody2->GetFixtureList();
	groundFixture2->SetRestitution(1.5f);

	b2Body* groundBody3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
	groundBody3->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

	b2Fixture* groundFixture3 = groundBody3->GetFixtureList();
	groundFixture3->SetRestitution(1.5f);

	b2Body* groundBody4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
	groundBody4->SetTransform(b2Vec2(100.0f, 0.0f), 0.0f);

	b2Fixture* groundFixture4 = groundBody4->GetFixtureList();
	groundFixture4->SetRestitution(1.5f);

	// Creamos un cuerpo definido por el usuario
	b2BodyDef bolaDefinicion;
	bolaDefinicion.type = b2_dynamicBody;
	bolaDefinicion.position.Set(50.0f, 30.0f); // Posición del cuerpo
	bolaBody = phyWorld->CreateBody(&bolaDefinicion); // Creamos el cuerpo dinámico

	// Creamos una forma (rectangular en este caso) para el cuerpo
	b2PolygonShape bolaShape;
	bolaShape.SetAsBox(5.0f, 5.0f); // Tamaño del cubo
	// Definimos las propiedades del cuerpo
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &bolaShape; 
	fixtureDef.density = 1.0f; 
	fixtureDef.friction = 0.5f; 
	fixtureDef.restitution = 1.5f; 
	// Asignamos una velocidad inicial al cuerpo
	float velocidadX = -25.0f; // Velocidad horizontal inicial
	float velocidadY = 0.0f; // Velocidad vertical inicial (hacia arriba)
	bolaBody->SetLinearVelocity(b2Vec2(velocidadX, velocidadY));

	bolaBody->CreateFixture(&fixtureDef);

	texture.loadFromFile("pelota.png");

	sf::Sprite* sprite = new sf::Sprite(texture);
	pelota_Avatar = new Avatar(bolaBody, sprite);
}

Game::~Game(void)
{ }

using namespace sf;

int _tmain(int argc, _TCHAR* argv[])
{

	Game* Juego;
	Juego = new Game(800, 600, "ventana eje 1");
	Juego->Loop();

	return 0;
}