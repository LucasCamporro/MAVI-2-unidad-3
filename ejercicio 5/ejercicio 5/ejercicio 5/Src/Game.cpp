#include "Box2DHelper.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <tchar.h>
#include "SFMLRenderer.h"
#include <list>

#include "Avatar.h"

using namespace sf;

class Game {
private:
    // Propiedades de la ventana
    int alto;
    int ancho;
    RenderWindow* wnd;
    Color clearColor;

    // Objetos de box2d
    b2World* phyWorld;

    // tiempo de frame
    float frameTime;
    int fps;

    // Cuerpo de box2d
    b2Body* cubocabezaBody;
    b2Body* cubotorsoBody;
    b2Body* cubobrazoizqBody;
    b2Body* cubobrazodereBody;
    b2Body* cubopiernaizqBody;
    b2Body* cubopiernadereBody;


    Avatar* cubo_Avatar;

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

Game::Game(int ancho, int alto, std::string titulo) {
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom();
    InitPhysics();
}

void Game::Loop() {
    while (wnd->isOpen()) {
        wnd->clear(clearColor);
        DoEvents();
        UpdatePhysics();
        DrawGame();
        wnd->display();
    }
}

void Game::UpdatePhysics() {
    phyWorld->Step(frameTime, 8, 8);
    phyWorld->ClearForces();
    phyWorld->DebugDraw();
}

void Game::DrawGame() {
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

    // Dibujamos  cabeza
    sf::RectangleShape cuboShape(sf::Vector2f(3, 4));
    cuboShape.setFillColor(sf::Color::Blue);
    cuboShape.setPosition(cubocabezaBody->GetPosition().x - 1.5, cubocabezaBody->GetPosition().y - 2);
    wnd->draw(cuboShape);

    // Dibujamos torso
    sf::RectangleShape cubo2Shape(sf::Vector2f(6, 10));
    cubo2Shape.setFillColor(sf::Color::Magenta);
    cubo2Shape.setPosition(cubotorsoBody->GetPosition().x - 3.0, cubotorsoBody->GetPosition().y - 5.0);
    wnd->draw(cubo2Shape);

    // Dibujamos brazo izq
    sf::RectangleShape cubobrazoIzqShape(sf::Vector2f(3, 7));
    cubobrazoIzqShape.setFillColor(sf::Color::Magenta);
    cubobrazoIzqShape.setPosition(cubobrazoizqBody->GetPosition().x - 1.5, cubobrazoizqBody->GetPosition().y - 3.5);
    wnd->draw(cubobrazoIzqShape);

    // Dibujamos brazo dere
    sf::RectangleShape cubobrazoDereShape(sf::Vector2f(3, 7));
    cubobrazoDereShape.setFillColor(sf::Color::Magenta);
    cubobrazoDereShape.setPosition(cubobrazodereBody->GetPosition().x - 1.5, cubobrazodereBody->GetPosition().y - 3.5);
    wnd->draw(cubobrazoDereShape);

    // Dibujamos pierna izq
    sf::RectangleShape cubopiernaIzqShape(sf::Vector2f(3, 7));
    cubopiernaIzqShape.setFillColor(sf::Color::Magenta);
    cubopiernaIzqShape.setPosition(cubopiernaizqBody->GetPosition().x - 1.5, cubopiernaizqBody->GetPosition().y - 3.5);
    wnd->draw(cubopiernaIzqShape);

    // Dibujamos pierna dere
    sf::RectangleShape cubopiernaDereShape(sf::Vector2f(3, 7));
    cubopiernaDereShape.setFillColor(sf::Color::Magenta);
    cubopiernaDereShape.setPosition(cubopiernadereBody->GetPosition().x - 1.5, cubopiernadereBody->GetPosition().y - 3.5);
    wnd->draw(cubopiernaDereShape);

    cubo_Avatar->Actualizar();
    cubo_Avatar->Dibujar(*wnd);
}

void Game::DoEvents() {
    Event evt;
    while (wnd->pollEvent(evt)) {
        switch (evt.type) {
        case Event::Closed:
            wnd->close();
            break;
        }
    }

    // Aplicamos fuerza al cuerpo
    cubocabezaBody->SetAwake(true);
    b2Vec2 force(0.0f, 0.0f);
    float forceMagnitude = 500.0f;

    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        force.x -= forceMagnitude;
    }
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
        force.x += forceMagnitude;
    }
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
        force.y -= forceMagnitude * 2;
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
        force.y += forceMagnitude;
    }

    cubocabezaBody->ApplyForceToCenter(force, true);
    cubotorsoBody->ApplyForceToCenter(force, true);
}

// Definimos el área del mundo que veremos en nuestro juego
// Box2D tiene problemas para simular magnitudes muy grandes
void Game::SetZoom() {
    View camara;
    // Posición del view
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); // asignamos la cámara
}

void Game::InitPhysics() {
    // Inicializamos el mundo con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Creamos un piso y paredes
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
    groundBody->SetTransform(b2Vec2(0.0f, 100.0f), 0.0f);

    b2Body* groundBody2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
    groundBody2->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

    b2Body* groundBody3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
    groundBody3->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

    b2Body* groundBody4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
    groundBody4->SetTransform(b2Vec2(100.0f, 0.0f), 0.0f);

    // Creamos la cabeza
    b2BodyDef cuboDefinicion;
    cuboDefinicion.type = b2_dynamicBody;
    cuboDefinicion.position.Set(50.0f, 70.0f); // Posición del cuerpo
    cubocabezaBody = phyWorld->CreateBody(&cuboDefinicion); // Creamos el cuerpo dinámico
    // Creamos una forma (rectangular en este caso) para el cuerpo
    b2PolygonShape cuboShape;
    cuboShape.SetAsBox(1.5f, 2.0f); // Tamaño del cubo
    // Definimos las propiedades del cuerpo
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &cuboShape; // Forma asociada al cuerpo
    fixtureDef.density = 1.0f; // Densidad
    fixtureDef.friction = 0.5f; // Fricción
    fixtureDef.restitution = 0.0f; // Restitución (rebote)
    // Añadimos la forma al cuerpo
    cubocabezaBody->CreateFixture(&fixtureDef);
    cubocabezaBody->SetAngularDamping(100.0f);
    cubocabezaBody->SetFixedRotation(true);

    // Creamos el torso
    b2BodyDef cubo2Definicion;
    cubo2Definicion.type = b2_dynamicBody;
    cubo2Definicion.position.Set(50.0f, 90.0f); // Posición del cuerpo
    cubotorsoBody = phyWorld->CreateBody(&cubo2Definicion); // Creamos el cuerpo dinámico
    // Creamos una forma (rectangular en este caso) para el cuerpo
    b2PolygonShape cubo2Shape;
    cubo2Shape.SetAsBox(3.0f, 5.0f); // Tamaño del cubo
    // Definimos las propiedades del cuerpo
    b2FixtureDef fixture2Def;
    fixture2Def.shape = &cubo2Shape; // Forma asociada al cuerpo
    fixture2Def.density = 1.0f; // Densidad
    fixture2Def.friction = 0.5f; // Fricción
    fixture2Def.restitution = 0.0f; // Restitución (rebote)
    // Añadimos la forma al cuerpo
    cubotorsoBody->CreateFixture(&fixture2Def);
    cubotorsoBody->SetAngularDamping(100.0f);
    cubotorsoBody->SetFixedRotation(true);

    // Creamos el brazo 1
    b2BodyDef cubobrazoDereDefinicion;
    cubobrazoDereDefinicion.type = b2_dynamicBody;
    cubobrazoDereDefinicion.position.Set(50.0f, 30.0f); // Posición del cuerpo
    cubobrazodereBody = phyWorld->CreateBody(&cubobrazoDereDefinicion); // Creamos el cuerpo dinámico
    // Creamos una forma (rectangular en este caso) para el cuerpo
    b2PolygonShape cubobrazoDereShape;
    cubobrazoDereShape.SetAsBox(1.5f, 3.5f); // Tamaño del cubo
    // Definimos las propiedades del cuerpo
    b2FixtureDef fixturebrazoDereDef;
    fixturebrazoDereDef.shape = &cubobrazoDereShape; // Forma asociada al cuerpo
    fixturebrazoDereDef.density = 1.0f; // Densidad
    fixturebrazoDereDef.friction = 0.5f; // Fricción
    fixturebrazoDereDef.restitution = 0.0f; // Restitución (rebote)
    // Añadimos la forma al cuerpo
    cubobrazodereBody->CreateFixture(&fixturebrazoDereDef);
    cubobrazodereBody->SetAngularDamping(100.0f);
    cubobrazodereBody->SetFixedRotation(true);

    // Creamos el brazo 2
    b2BodyDef cubobrazoIzqDefinicion;
    cubobrazoIzqDefinicion.type = b2_dynamicBody;
    cubobrazoIzqDefinicion.position.Set(50.0f, 90.0f); // Posición del cuerpo
    cubobrazoizqBody = phyWorld->CreateBody(&cubobrazoIzqDefinicion); // Creamos el cuerpo dinámico
    // Creamos una forma (rectangular en este caso) para el cuerpo
    b2PolygonShape cubobrazoIzqShape;
    cubobrazoIzqShape.SetAsBox(1.5f, 3.5f); // Tamaño del cubo
    // Definimos las propiedades del cuerpo
    b2FixtureDef fixturebrazoIzqDef;
    fixturebrazoIzqDef.shape = &cubobrazoIzqShape; // Forma asociada al cuerpo
    fixturebrazoIzqDef.density = 1.0f; // Densidad
    fixturebrazoIzqDef.friction = 0.5f; // Fricción
    fixturebrazoIzqDef.restitution = 0.0f; // Restitución (rebote)
    // Añadimos la forma al cuerpo
    cubobrazoizqBody->CreateFixture(&fixturebrazoIzqDef);
    cubobrazoizqBody->SetAngularDamping(100.0f);
    cubobrazoizqBody->SetFixedRotation(true);

    // Creamos la pierna 1
    b2BodyDef cubopiernaIzqDefinicion;
    cubopiernaIzqDefinicion.type = b2_dynamicBody;
    cubopiernaIzqDefinicion.position.Set(50.0f, 30.0f); // Posición del cuerpo
    cubopiernaizqBody = phyWorld->CreateBody(&cubopiernaIzqDefinicion); // Creamos el cuerpo dinámico
    // Creamos una forma (rectangular en este caso) para el cuerpo
    b2PolygonShape cubopiernaIzqShape;
    cubopiernaIzqShape.SetAsBox(1.5f, 3.5f); // Tamaño del cubo
    // Definimos las propiedades del cuerpo
    b2FixtureDef fixturepiernaIzqDef;
    fixturepiernaIzqDef.shape = &cubopiernaIzqShape; // Forma asociada al cuerpo
    fixturepiernaIzqDef.density = 1.0f; // Densidad
    fixturepiernaIzqDef.friction = 0.5f; // Fricción
    fixturepiernaIzqDef.restitution = 0.0f; // Restitución (rebote)
    // Añadimos la forma al cuerpo
    cubopiernaizqBody->CreateFixture(&fixturepiernaIzqDef);
    cubopiernaizqBody->SetAngularDamping(100.0f);
    cubopiernaizqBody->SetFixedRotation(true);

    // Creamos la pierna 2
    b2BodyDef cubopiernaDereDefinicion;
    cubopiernaDereDefinicion.type = b2_dynamicBody;
    cubopiernaDereDefinicion.position.Set(50.0f, 30.0f); // Posición del cuerpo
    cubopiernadereBody = phyWorld->CreateBody(&cubopiernaDereDefinicion); // Creamos el cuerpo dinámico
    // Creamos una forma (rectangular en este caso) para el cuerpo
    b2PolygonShape cubopiernaDereShape;
    cubopiernaDereShape.SetAsBox(1.5f, 3.5f); // Tamaño del cubo
    // Definimos las propiedades del cuerpo
    b2FixtureDef fixturepiernaDereDef;
    fixturepiernaDereDef.shape = &cubopiernaDereShape; // Forma asociada al cuerpo
    fixturepiernaDereDef.density = 1.0f; // Densidad
    fixturepiernaDereDef.friction = 0.5f; // Fricción
    fixturepiernaDereDef.restitution = 0.0f; // Restitución (rebote)
    // Añadimos la forma al cuerpo
    cubopiernadereBody->CreateFixture(&fixturepiernaDereDef);
    cubopiernadereBody->SetAngularDamping(100.0f);
    cubopiernadereBody->SetFixedRotation(true);

    // Creamos el joint que une la cabeza y el torso
    b2WeldJointDef jointDef;
    jointDef.bodyA = cubocabezaBody;
    jointDef.bodyB = cubotorsoBody;
    jointDef.collideConnected = false;
    // Configuramos el anclaje para que sea el punto en el cual se unen los cuerpos
    jointDef.localAnchorA.Set(0.0f, 2.0f);  // Parte inferior de la cabeza
    jointDef.localAnchorB.Set(0.0f, -4.5f); // Parte superior del torso
    phyWorld->CreateJoint(&jointDef);
   
    
    // Creamos el joint que une el brazo izq y el torso
    b2WeldJointDef jointbrazoIzqTorsoDef;
    jointbrazoIzqTorsoDef.bodyA = cubotorsoBody;
    jointbrazoIzqTorsoDef.bodyB = cubobrazoizqBody;
    jointbrazoIzqTorsoDef.collideConnected = false;
    // Configuramos el anclaje para que sea el punto en el cual se unen los cuerpos
    jointbrazoIzqTorsoDef.localAnchorA.Set(0.0f, 0.0f);  // Parte inferior de la cabeza
    jointbrazoIzqTorsoDef.localAnchorB.Set(4.6f, 1.5f); // Parte superior del torso
    phyWorld->CreateJoint(&jointbrazoIzqTorsoDef);

    // Creamos el joint que une el brazo dere y el torso
    b2WeldJointDef jointbrazoDereTorsoDef;
    jointbrazoDereTorsoDef.bodyA = cubotorsoBody;
    jointbrazoDereTorsoDef.bodyB = cubobrazodereBody;
    jointbrazoDereTorsoDef.collideConnected = false;
    // Configuramos el anclaje para que sea el punto en el cual se unen los cuerpos
    jointbrazoDereTorsoDef.localAnchorA.Set(4.6f, 0.0f);  // Parte inferior de la cabeza
    jointbrazoDereTorsoDef.localAnchorB.Set(0.0f, 1.5f); // Parte superior del torso
    phyWorld->CreateJoint(&jointbrazoDereTorsoDef);

    // Creamos el joint que une el torso y la pierna izquierda
    b2WeldJointDef jointPiernaIzqTorsoDef;
    jointPiernaIzqTorsoDef.bodyA = cubotorsoBody;
    jointPiernaIzqTorsoDef.bodyB = cubopiernaizqBody;
    jointPiernaIzqTorsoDef.collideConnected = false;
    // Configuramos el anclaje para que sea el punto en el cual se unen los cuerpos
    jointPiernaIzqTorsoDef.localAnchorA.Set(0.0f, 8.0f);  // Parte inferior izquierda del torso
    jointPiernaIzqTorsoDef.localAnchorB.Set(3.9f, 0.0f); // Parte superior de la pierna izquierda
    phyWorld->CreateJoint(&jointPiernaIzqTorsoDef);

    // Creamos el joint que une el torso y la pierna derecha
    b2WeldJointDef jointPiernaDereTorsoDef;
    jointPiernaDereTorsoDef.bodyA = cubotorsoBody;
    jointPiernaDereTorsoDef.bodyB = cubopiernadereBody;
    jointPiernaDereTorsoDef.collideConnected = false;
    // Configuramos el anclaje para que sea el punto en el cual se unen los cuerpos
    jointPiernaDereTorsoDef.localAnchorA.Set(0.0f, 8.0f);  // Parte inferior derecha del torso
    jointPiernaDereTorsoDef.localAnchorB.Set(-3.9f, 0.0f); // Parte superior de la pierna derecha
    phyWorld->CreateJoint(&jointPiernaDereTorsoDef);



    texture.loadFromFile("tetrisblocks.png");
    sf::Sprite* sprite = new sf::Sprite(texture);
    cubo_Avatar = new Avatar(cubocabezaBody, sprite);
}

Game::~Game(void) {
    delete wnd;
    delete phyWorld;
}

using namespace sf;

int _tmain(int argc, _TCHAR* argv[]) {
    Game* Juego;
    Juego = new Game(800, 600, "ventana eje 1");
    Juego->Loop();
    delete Juego;
    return 0;
}