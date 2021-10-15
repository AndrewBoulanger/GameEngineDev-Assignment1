#include "MyFrameListener.h"


bool MyFrameListener::frameStarted(Ogre::SceneNode* node) 
{
		const Ogre::Real Radius = 1.0;
		Ogre::Real x = Radius * Ogre::Math::Cos(m_Angle);
		Ogre::Real y = Radius * Ogre::Math::Sin(m_Angle);
		m_Angle += 0.01;
		_node->setPosition(x, y, 0);
		//_node->translate(translate * evt.timeSinceLastFrame);
		translate = Ogre::Vector3(0, 0, 0);
		return true;
}

