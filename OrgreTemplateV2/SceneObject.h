#pragma once
#include "Ogre.h"

///@author Andrew Boulanger 101292574
///@brief parent class to scene objects which need to be updated and checked for collision (i.e. the paddle and ball)
class sceneObject
{
protected:
	Ogre::SceneNode* node;
	
public:
	virtual bool Update(float deltaTime) = 0;

	///returns the world position based on the attached scene node
	virtual Ogre::Vector3 GetPosition()
	{
		return node->getPosition();
	}
	///returns the bounding box half size (of the width, height and depth) held by the scene node. useful for collision detection
	virtual Ogre::Vector3 GetHalfSize()
	{
		return node->_getWorldAABB().getHalfSize();
	}

};