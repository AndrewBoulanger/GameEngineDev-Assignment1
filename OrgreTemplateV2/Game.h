#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreTrays.h"
#include <iostream>
#include "Paddle.h"
#include "Ball.h"

using namespace Ogre;
using namespace OgreBites;

///@author Andrew Boulanger 101292574
///@brief the game application, it handles inputs, ui and holds/updates game objects
class Game
	: public ApplicationContext
	, public InputListener
{
private:
	SceneManager* scnMgr;
	Root* root;
	Ogre::PolygonMode polyMode;
	Camera* cam;

	int score = 0;
	int lives = 5;
	int fps, tpu;

	OgreBites::TrayListener myTrayListener;
	OgreBites::TrayManager* mGameOverTray;
	OgreBites::Label* mInfoLabel;

	OgreBites::Label* mFpsLabel;
	OgreBites::Label* mScoreLabel;
	OgreBites::Label* mTpuLabel;
	OgreBites::Label* mLivesLabel;
	OgreBites::Label* mGameOverLabel;
	OgreBites::Label* mFinalScoreLabel;
	OgreBites::Button* mRestartButton;

	Paddle* paddle;
	Ball* ball;

	float verticalInput;
	Ogre::Timer UI_timer;
	bool isGameRunning;

	void setup();
	void createCamera();
	bool keyPressed(const KeyboardEvent& evt);
	bool keyReleased(const KeyboardEvent& evt);
	bool CheckPaddleCollision(Vector3& normal);

	void StopRunning();
	void Reset();
public:
	Game();
	virtual ~Game() {}


	void Update(float deltaTime);

	void UpdateFrameStats(int fps, int tpu);


};