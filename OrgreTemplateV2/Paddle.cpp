#include "Paddle.h"


///constructor - takes a scene manager to allow creation of a scene node. 
/// also creates a cube mesh and sets default values
Paddle::Paddle(Ogre::SceneManager* scnMgr)
{

	Ogre::Entity* ent = scnMgr->createEntity("cube.mesh");
	ent->setCastShadows(true);
	ent->setMaterial(Ogre::MaterialManager::getSingleton().getByName("AndrewsMaterial1"));
	node = scnMgr->createSceneNode("Paddle");
	node->attachObject(ent);
	scnMgr->getRootSceneNode()->addChild(node);
	node->setPosition(Ogre::Vector3(-4.0f, 0.0f, 0.0f));
	node->setScale(0.004f, 0.015f, 0.004f);
	
	speed = 12;
	direction = 0;
	topScreen = 5.5;
	bottomScreen = -5.5;

}

///handles movement if the paddle has received input. clamped within the bounds of the screen. 
///pass in deltaTime for frame independent movement
bool Paddle::Update(float deltaTime)
{
	if(direction == 0)
		return false;
	
	if( direction > 0 && node->getPosition().y > topScreen)
		return false;
	else if( direction < 0 && node->getPosition().y < bottomScreen)
		return false;
	Ogre::Vector3 translate = Ogre::Vector3(0, speed * direction * deltaTime, 0);
	node->translate(translate);
}


///allows other objects to pass in vertical input. If not zero, it'll move during the update
void Paddle::GetInput(float vertical)
{
	direction = vertical;
}
