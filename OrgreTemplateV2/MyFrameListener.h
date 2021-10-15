#pragma once


class MyFrameListener : public Ogre::FrameListener
{
private:
	Ogre::SceneNode* _node;

	Ogre::Vector3 translate(0, 0, 0);
	Ogre::Real m_Angle = 0.0;
public:

	MyFrameListener(Ogre::SceneNode* node)
	{
		_node = node;
	}

	bool frameStarted(const Ogre::FrameEvent& evt);

	void SetTranslate(Ogre::Vector3 vec)
	{
		translate = vec;
	}

};