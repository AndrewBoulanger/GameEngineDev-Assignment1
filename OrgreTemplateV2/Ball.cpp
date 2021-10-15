#include <math.h>
#include "Ball.h"
#include <iostream>


///creates a ball. takes an x speed and max possible y speed (actual y speed will be randomized)
///also takes a scene manager so it can be placed in a scene
Ball::Ball(float xVel, float maxY, Ogre::SceneManager* scnMgr)
{

	Ogre::Entity* ent = scnMgr->createEntity("sphere.mesh");
	ent->setCastShadows(true);
	node = scnMgr->createSceneNode("ball");
	node->attachObject(ent);
	scnMgr->getRootSceneNode()->addChild(node);
	node->setPosition(Ogre::Vector3(2.0f, 1.0f, 0.0f));
	node->setScale(0.003f, 0.003f, 0.003f);

	xVelocity = xVel;
	maxYVel = maxY;
	yVelocity = rand() % (int)maxYVel;

	topScreen = 5.8;
	bottonScreen = -5.8;
	leftScreen = -7.8;
	rightScreen = 7.8;
	
}

///moves the ball based on it's x and y velocity and checks for collision with the walls. 
///if it does collide with the walls, reflect the direction. if its the left wall, reset the 
/// ball and return true
bool Ball::Update(float deltaTime)
{
	bool isOffScreen = false;
	Ogre::Vector3 translate = Ogre::Vector3(xVelocity, yVelocity, 0);
	node->translate(translate * deltaTime);

	if(node->getPosition().y < bottonScreen && yVelocity < 0)
		flipYvelocity();
	else if(node->getPosition().y > topScreen && yVelocity > 0)
		flipYvelocity();
	if(node->getPosition().x > rightScreen && xVelocity > 0) 
		FlipXVelocity();
	else if(node->getPosition().x <= leftScreen)
	{
		FlipXVelocity();
		ResetPosition();
		isOffScreen = true;
	}

	return isOffScreen;
}

///reverses the ball's direction on the x axis
void Ball::FlipXVelocity()
{
	xVelocity *= -1;

}

///reverses the ball's direction on the y axis
void Ball::flipYvelocity()
{
	yVelocity *= -1;
}

///adds or reduces random value from ball's y Velocity. at most 25% of max Y velocity
void Ball::RandomizeYVelocity()
{
	float halfVel = maxYVel *0.5;
	int newYSpeed = (rand() % (int)halfVel) - int(halfVel*0.5);
	
	yVelocity += newYSpeed;
	if(yVelocity > maxYVel)
		yVelocity = maxYVel;
	else if(yVelocity < -maxYVel)
		yVelocity = -maxYVel;

}

///called when the ball goes off screen returns ball to the center of the screen
void Ball::ResetPosition()
{
	node->setPosition(Ogre::Vector3(0,1.5,0));
}


///how the ball reacts on collision given the normal of the paddle side. returns whether or not the collision was valid (i.e. if the ball was moving right, it couldn't have been a valid collision).  
///helps prevent confusion caused by overlap and penetration and changes direction given the axis it collided on
bool Ball::OnPaddleCollision(Ogre::Vector3 normal)
{
	if(xVelocity >= 0)
		return false;

	bool validCollision = false;
	if( normal.x > 0) 
	{
		RandomizeYVelocity();
		FlipXVelocity();
		validCollision = true;
	}

	if( normal.y < 0 && yVelocity > 0 || normal.y > 0 && yVelocity < 0 )
	{
		flipYvelocity();
		validCollision = true;
	}
	
	return validCollision;

}

