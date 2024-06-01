#include "Avatar.h"

Avatar::Avatar(b2Body* body, sf::Sprite* sprite) : pelota_body(body), pelota_Sprite(sprite)
{
	b2AABB aabb;
	aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
	aabb.upperBound = b2Vec2(FLT_MIN, FLT_MIN);
	for (b2Fixture* f = pelota_body->GetFixtureList(); f; f = f->GetNext())
		aabb.Combine(f->GetAABB(0));
	float sizex = aabb.GetExtents().x * 2.0f / pelota_Sprite->getTexture()->getSize().x;
	float sizey = aabb.GetExtents().y * 2.0f/ pelota_Sprite->getTexture()->getSize().y;
	pelota_Sprite->setScale(sizex, sizey);
	pelota_Sprite->setOrigin(sf::Vector2f(pelota_Sprite->getTexture()->getSize().x / 2.0f, pelota_Sprite->getTexture()->getSize().y / 2.0f));

	b2Vec2 position = pelota_body->GetPosition();
	pelota_Sprite->setPosition(sf::Vector2f(position.x, position.y));
}

void Avatar::Actualizar()
{
	b2Vec2 position = pelota_body->GetPosition();
	pelota_Sprite->setPosition(sf::Vector2f(position.x, position.y));

	float angulo = pelota_body->GetAngle();
	pelota_Sprite->setRotation(angulo * 180 / 3.1415f);

}


void Avatar::Dibujar(sf::RenderWindow& wnd)
{
	wnd.draw(*pelota_Sprite);
}