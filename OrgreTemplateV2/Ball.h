#pragma once
#include "Ogre.h"
#include "SceneObject.h"

///@author Andrew Boulanger 101292574
///@brief Sphere mesh and scene node which moves every update based on the x and y velocity. stays within the set bounds of the screen.
///flips direction on collision with walls or paddle.
/// inherits from sceneObject
class Ball : public sceneObject
{
private:
	float xVelocity;
	float yVelocity;
	float maxYVel;
	

	float topScreen, bottonScreen, rightScreen, leftScreen;

public:

	Ball(float xVel, float maxY, Ogre::SceneManager* scnManager);

	bool Update(float deltaTime);

	void FlipXVelocity();
	void flipYvelocity();
	void RandomizeYVelocity();
	void ResetPosition();
	
	bool OnPaddleCollision(Ogre::Vector3 normal);
};