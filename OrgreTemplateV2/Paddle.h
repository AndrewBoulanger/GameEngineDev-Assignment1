#pragma once
#include "Ogre.h"
#include "SceneObject.h"

///@author Andrew Boulanger 101292574
///@brief Square mesh and scene node which moves every update based on a passed in vertical input. stays within the set bounds of the screen.
/// inherits from sceneObject
class Paddle : public sceneObject
{
private:
	float speed;
	float direction;
	float topScreen;
	float bottomScreen;
	

public:
	Paddle(Ogre::SceneManager* scnMgr);
	bool Update(float deltaTime);
	void GetInput(float vertical);

};