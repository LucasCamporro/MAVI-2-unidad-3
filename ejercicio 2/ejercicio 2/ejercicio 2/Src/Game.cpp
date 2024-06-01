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
	b2Body* bola2Body;
	b2Body* groundBody;

	Avatar* pelota_Avatar;
	Avatar* pelota2_Avatar;

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
	groundShape.setFillColor(sf::Color::Green);
	groundShape.setPosition(0, 95);
	wnd->draw(groundShape);

	groundShape.setPosition(0, 0);
	wnd->draw(groundShape);

	sf::RectangleShape groundShape2(sf::Vector2f(5, 500));
	groundShape2.setFillColor(sf::Color::Green);
	groundShape2.setPosition(95, 0);
	wnd->draw(groundShape2);

	groundShape2.setPosition(0, 0);
	wnd->draw(groundShape2);

	// Dibujamos el cuerpo de control (círculo)
	sf::CircleShape controlShape(5);
	controlShape.setFillColor(sf::Color::Magenta);
	controlShape.setPosition(bolaBody->GetPosition().x - 5, bolaBody->GetPosition().y - 5);
	wnd->draw(controlShape);

	sf::CircleShape bola2shape(5);
	controlShape.setFillColor(sf::Color::Magenta);
	controlShape.setPosition(bola2Body->GetPosition().x - 5, bola2Body->GetPosition().y - 5);
	wnd->draw(controlShape);

	pelota_Avatar->Actualizar();
	pelota_Avatar->Dibujar(*wnd);
	pelota2_Avatar->Actualizar();
	pelota2_Avatar->Dibujar(*wnd);
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

	// Movemos el cuerpo
	bolaBody->SetAwake(true);
	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		bolaBody->SetLinearVelocity(b2Vec2(-20.0f, 0.0f));
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		bolaBody->SetLinearVelocity(b2Vec2(20.0f, 0.0f));
	}
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		bolaBody->SetLinearVelocity(b2Vec2(0.0f, -20.0f));
	}
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		bolaBody->SetLinearVelocity(b2Vec2(0.0f, 20.0f));
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

	b2Body* groundBody2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
	groundBody2->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

	b2Body* groundBody3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
	groundBody3->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

	b2Body* groundBody4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
	groundBody4->SetTransform(b2Vec2(100.0f, 0.0f), 0.0f);

	// Creamos bola 1
	b2BodyDef bola1Definicion;
	bola1Definicion.type = b2_dynamicBody;
	bola1Definicion.position.Set(50.0f, 90.0f); // Posición del cuerpo
	bolaBody = phyWorld->CreateBody(&bola1Definicion); // Creamos el cuerpo dinámico
	// Creamos una forma (rectangular en este caso) para el cuerpo
	b2PolygonShape controlShape;
	controlShape.SetAsBox(5.0f, 5.0f); // Tamaño del cubo
	// Definimos las propiedades del cuerpo
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &controlShape; // Forma asociada al cuerpo
	fixtureDef.density = 1.0f; // Densidad
	fixtureDef.friction = 0.50f; // Fricción
	fixtureDef.restitution = 0.0f; // Restitución (rebote)
	// Añadimos la forma al cuerpo
	bolaBody->CreateFixture(&fixtureDef);

	// Creamos bola 2
	b2BodyDef bola2Definicion;
	bola2Definicion.type = b2_dynamicBody;
	bola2Definicion.position.Set(50.0f, 50.0f); // Posición del cuerpo
	bola2Body = phyWorld->CreateBody(&bola2Definicion); // Creamos el cuerpo dinámico
	// Creamos una forma (rectangular en este caso) para el cuerpo
	b2PolygonShape bola2shape;
	bola2shape.SetAsBox(5.0f, 5.0f); // Tamaño del cubo
	// Definimos las propiedades del cuerpo
	b2FixtureDef fixturebola2Def;
	fixturebola2Def.shape = &bola2shape; // Forma asociada al cuerpo
	fixturebola2Def.density = 1.0f; // Densidad
	fixturebola2Def.friction = 0.50f; // Fricción
	fixturebola2Def.restitution = 0.0f; // Restitución (rebote)
	// Añadimos la forma al cuerpo
	bola2Body->CreateFixture(&fixturebola2Def);

	float posbola1x = bolaBody->GetPosition().x - 5;
	float posbola1y = bola2Body->GetPosition().y - 5;

	texture.loadFromFile("pelota.png");

	sf::Sprite* sprite = new sf::Sprite(texture);
	pelota_Avatar = new Avatar(bolaBody, sprite);
	pelota2_Avatar = new Avatar(bola2Body, sprite);

	// Creamos el joint que une las dos bolas
	b2DistanceJointDef jointDef;
	jointDef.bodyA = bolaBody;
	jointDef.bodyB = bola2Body;
	jointDef.collideConnected = true;
	jointDef.maxLength = (bolaBody->GetPosition().x / 2, bolaBody->GetPosition().y / 2) - (bola2Body->GetPosition().x / 2, bola2Body->GetPosition().y / 2);
	jointDef.minLength = (bolaBody->GetPosition().x / 2, bolaBody->GetPosition().y / 2) - (bola2Body->GetPosition().x / 2, bola2Body->GetPosition().y / 2);
	jointDef.localAnchorA.Set(0.0f, 0.0f);
	jointDef.localAnchorB.Set(0.0f, 0.0f);

	phyWorld->CreateJoint(&jointDef);
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